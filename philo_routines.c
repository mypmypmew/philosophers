#include "philo.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

void action_print(t_info *rules, int philo_id, const char *msg)
{
	long now;
	pthread_mutex_lock(&rules->dead_lock);
	if (!rules->finished)
	{
		pthread_mutex_unlock(&rules->dead_lock);

		pthread_mutex_lock(&rules->print_mutex);

		now = get_current_time_ms() - rules->start_time;
		printf("%ld %d %s\n", now, philo_id, msg);

		pthread_mutex_unlock(&rules->print_mutex);
	}
	else
	{
		pthread_mutex_unlock(&rules->dead_lock);
	}
}

static long long current_time_microseconds(void)
{
	struct timeval now;
	gettimeofday(&now, NULL);
	return ((long long)now.tv_sec * 1000000) + now.tv_usec;
}

void philo_usleep(long long target_us, t_philosophers *philo)
{
	long long start;
	long long now;
	long long elapsed;
	long long remaining;

	start = current_time_microseconds();
	while (1)
	{
		// 1) Check if simulation finished
		pthread_mutex_lock(&philo->info->dead_lock);
		int local_finished = philo->info->finished;
		pthread_mutex_unlock(&philo->info->dead_lock);

		if (local_finished)
			break;

		// 2) Check how long we've slept so far
		now = current_time_microseconds();
		elapsed = now - start;
		if (elapsed >= target_us)
			break;
		remaining = target_us - elapsed;
		if (remaining > 1000)
			usleep(remaining / 2);
		else
		{
			while (1)
			{
				pthread_mutex_lock(&philo->info->dead_lock);
				local_finished = philo->info->finished;
				pthread_mutex_unlock(&philo->info->dead_lock);
				if (local_finished)
					break;

				now = current_time_microseconds();
				if ((now - start) >= target_us)
					break;
			}
			break;
		}
	}
}

void philo_eats(t_philosophers *philo)
{
	t_info *rules = philo->info;

	if (philo->left_fork->id < philo->right_fork->id)
	{
		pthread_mutex_lock(&philo->left_fork->fork);
		action_print(rules, philo->philosophers_id, "has taken a fork");

		pthread_mutex_lock(&philo->right_fork->fork);
		action_print(rules, philo->philosophers_id, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&philo->right_fork->fork);
		action_print(rules, philo->philosophers_id, "has taken a fork");

		pthread_mutex_lock(&philo->left_fork->fork);
		action_print(rules, philo->philosophers_id, "has taken a fork");
	}
	pthread_mutex_lock(&rules->meal_check);
	philo->last_meal = get_current_time_ms();
	pthread_mutex_unlock(&rules->meal_check);

	action_print(rules, philo->philosophers_id, "is eating");
	philo_usleep((long long)rules->time_to_eat * 1000, philo);

	pthread_mutex_lock(&rules->meal_check);
	philo->meals_amount++;
	pthread_mutex_unlock(&rules->meal_check);

	if (philo->left_fork->id < philo->right_fork->id)
	{
		pthread_mutex_unlock(&philo->right_fork->fork);
		pthread_mutex_unlock(&philo->left_fork->fork);
	}
	else
	{
		pthread_mutex_unlock(&philo->left_fork->fork);
		pthread_mutex_unlock(&philo->right_fork->fork);
	}
}

void *philo_routine(void *arg)
{
	t_philosophers *philo = (t_philosophers *)arg;
	t_info         *rules = philo->info;

	if (rules->philosophers_number == 1)
	{
		pthread_mutex_lock(&philo->left_fork->fork);
		action_print(rules, philo->philosophers_id, "has taken a fork");

		long long start = get_current_time_ms();
		while (1)
		{
			pthread_mutex_lock(&rules->dead_lock);
			int local_finished = rules->finished;
			pthread_mutex_unlock(&rules->dead_lock);
			if (local_finished)
				break;

			if ((get_current_time_ms() - start) >= rules->time_to_die)
				break;
			usleep(50);
		}
		pthread_mutex_unlock(&philo->left_fork->fork);
		return NULL;
	}
	if (philo->philosophers_id % 2 == 0)
		usleep(500);
	while (1)
	{
		pthread_mutex_lock(&rules->dead_lock);
		int local_finished = rules->finished;
		pthread_mutex_unlock(&rules->dead_lock);

		if (local_finished)
			break;

		// 1) Eat
		philo_eats(philo);

		// 2) Check if finished again
		pthread_mutex_lock(&rules->dead_lock);
		local_finished = rules->finished;
		pthread_mutex_unlock(&rules->dead_lock);
		if (local_finished)
			break;

		// 3) Sleep
		action_print(rules, philo->philosophers_id, "is sleeping");
		philo_usleep((long long)rules->time_to_sleep * 1000, philo);

		// 4) Check again
		pthread_mutex_lock(&rules->dead_lock);
		local_finished = rules->finished;
		pthread_mutex_unlock(&rules->dead_lock);
		if (local_finished)
			break;

		// 5) Think
		action_print(rules, philo->philosophers_id, "is thinking");
	}
	return NULL;
}