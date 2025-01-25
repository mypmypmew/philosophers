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

void check_finished(t_philosophers *philo, int *local_finished)
{
	pthread_mutex_lock(&philo->info->dead_lock);
	*local_finished = philo->info->finished;
	pthread_mutex_unlock(&philo->info->dead_lock);
}

long long	calculate_remaining_time(long long start, long long target_us)
{
	long long now;
	now = current_time_microseconds();
	return (target_us - (now - start));
}

void	short_sleep_loop(long long start, long long target_us, t_philosophers *philo)
{
	int local_finished;

	while (1)
	{
		check_finished(philo, &local_finished);
		if (local_finished)
			break;
		if (current_time_microseconds() - start >= target_us)
			break;
	}
}

void philo_usleep(long long target_us, t_philosophers *philo)
{
	long long start;
	int local_finished;
	long long remaining;

	start = current_time_microseconds();
	while (1)
	{
		check_finished(philo, &local_finished);
		if (local_finished)
			break;
		remaining = calculate_remaining_time(start, target_us);
		if (remaining <= 0)
			break;
		if (remaining > 1000)
			usleep(remaining / 2);
		else
		{
			short_sleep_loop(start, target_us, philo);
			break;
		}
	}
}

void lock_forks(t_philosophers *philo)
{
	if (philo->left_fork->id < philo->right_fork->id)
	{
		pthread_mutex_lock(&philo->left_fork->fork);
		action_print(philo->info, philo->philosophers_id, "has taken a fork");

		pthread_mutex_lock(&philo->right_fork->fork);
		action_print(philo->info, philo->philosophers_id, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&philo->right_fork->fork);
		action_print(philo->info, philo->philosophers_id, "has taken a fork");

		pthread_mutex_lock(&philo->left_fork->fork);
		action_print(philo->info, philo->philosophers_id, "has taken a fork");
	}
}

void unlock_forks(t_philosophers *philo)
{
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

void update_last_meal(t_philosophers *philo)
{
	pthread_mutex_lock(&philo->info->meal_check);
	philo->last_meal = get_current_time_ms();
	pthread_mutex_unlock(&philo->info->meal_check);
}

void increment_meals(t_philosophers *philo)
{
	pthread_mutex_lock(&philo->info->meal_check);
	philo->meals_amount++;
	pthread_mutex_unlock(&philo->info->meal_check);
}

void philo_eats(t_philosophers *philo)
{
	lock_forks(philo);

	update_last_meal(philo);

	action_print(philo->info, philo->philosophers_id, "is eating");
	philo_usleep((long long)philo->info->time_to_eat * 1000, philo);

	increment_meals(philo);

	unlock_forks(philo);
}

void handle_single_philosopher(t_philosophers *philo)
{
	t_info *rules = philo->info;

	pthread_mutex_lock(&philo->left_fork->fork);
	action_print(rules, philo->philosophers_id, "has taken a fork");

	long long start = get_current_time_ms();
	while (1)
	{
		pthread_mutex_lock(&rules->dead_lock);
		int local_finished = rules->finished;
		pthread_mutex_unlock(&rules->dead_lock);

		if (local_finished || (get_current_time_ms() - start) >= rules->time_to_die)
			break;

		usleep(50);
	}
	pthread_mutex_unlock(&philo->left_fork->fork);
}

void stagger_start(t_philosophers *philo)
{
	if (philo->philosophers_id % 2 == 0)
		usleep(500);
}

int check_if_finished(t_info *rules)
{
	pthread_mutex_lock(&rules->dead_lock);
	int local_finished = rules->finished;
	pthread_mutex_unlock(&rules->dead_lock);
	return local_finished;
}

void perform_actions(t_philosophers *philo)
{
	t_info *rules;

	rules = philo->info;
	while (1)
	{
		if (check_if_finished(rules))
			break;
		philo_eats(philo);
		if (check_if_finished(rules))
			break;
		action_print(rules, philo->philosophers_id, "is sleeping");
		philo_usleep((long long)rules->time_to_sleep * 1000, philo);

		if (check_if_finished(rules))
			break;
		action_print(rules, philo->philosophers_id, "is thinking");
	}
}

void *philo_routine(void *arg)
{
	t_philosophers *philo = (t_philosophers *)arg;
	t_info *rules = philo->info;

	if (rules->philosophers_number == 1)
	{
		handle_single_philosopher(philo);
		return NULL;
	}

	stagger_start(philo);
	perform_actions(philo);

	return NULL;
}