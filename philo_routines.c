#include "philo.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>


long get_current_time__ms(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void action_print(t_info *rules, int philo_id, const char *msg)
{
	long now;

	pthread_mutex_lock(&rules->print_mutex);
	if (!rules->finished)
	{
		now = get_current_time__ms() - rules->start_time;
		printf("%ld %d %s\n", now, philo_id, msg);
	}
	pthread_mutex_unlock(&rules->print_mutex);
}

static long long current_time_microseconds(void)
{
	struct timeval now;
	gettimeofday(&now, NULL);
	return ((long long)now.tv_sec * 1000000) + now.tv_usec;
}

/*
 * philo_usleep:
 * Sleeps for 'target_us' microseconds in small chunks.
 * - If there's plenty of time left (> 1000us), usleep half of what's left.
 * - If we're close to the target, do a tight loop to avoid overshoot.
 * - Checks if the simulation is finished to break early.
 */
void philo_usleep(long long target_us, t_philosophers *philo)
{
	long long start;
	long long elapsed;
	long long remaining;
	long long now;

	start = current_time_microseconds();
	while (!philo->info->finished)
	{
		now = current_time_microseconds();
		elapsed = now - start;
		if (elapsed >= target_us)
			break;
		remaining = target_us - elapsed;

		/*
		 * If we still have more than ~1ms to go, sleep half of it
		 * to reduce overshoot. Otherwise, do a quick busy-wait.
		 */
		if (remaining > 1000)
			usleep(remaining / 2);
		else
		{
			// Busy-wait until either we're done or simulation ends
			while (!philo->info->finished)
			{
				now = current_time_microseconds();
				if ((now - start) >= target_us)
					break;
			}
			break; // Once we exit the tight loop, we're done sleeping
		}
	}
}

// static int check_time(t_philosophers *philo, struct timeval *time,
// 					  long long start, long long usecs)
// {
// 	gettimeofday(time, NULL);
// 	if ((((time->tv_sec * 1000000) + time->tv_usec) - start) > usecs
// 		|| philo->info->finished)
// 		return (1);
// 	return (0);
// }

// void ft_usleep(long long usecs, t_philosophers *philo)
// {
// 	struct timeval  time;
// 	long long       start;
// 	long long       rem;

// 	gettimeofday(&time, NULL);
// 	start = ((time.tv_sec * 1000000) + time.tv_usec);

// 	while (1)
// 	{
// 		if (check_time(philo, &time, start, usecs) == 1)
// 			break;
// 		rem = usecs - (((time.tv_sec * 1000000) + time.tv_usec) - start);

// 		if (rem > 1000)
// 			usleep(rem / 2);
// 		else
// 		{
// 			while (1)
// 			{
// 				gettimeofday(&time, NULL);
// 				if ((((time.tv_sec * 1000000) + time.tv_usec) - start) > usecs)
// 					break;
// 			}
// 		}
// 	}
// }

void philo_eats(t_philosophers *philo)
{
	t_info *rules = philo->info;

	pthread_mutex_lock(&philo->left_fork->fork);
	action_print(rules, philo->philosophers_id, "has taken a fork");

	pthread_mutex_lock(&philo->right_fork->fork);
	action_print(rules, philo->philosophers_id, "has taken a fork");

	pthread_mutex_lock(&rules->meal_check);
	philo->last_meal = get_current_time_ms();
	pthread_mutex_unlock(&rules->meal_check);

	action_print(rules, philo->philosophers_id, "is eating");

	philo_usleep((long long)rules->time_to_eat * 1000, philo);

	philo->meals_amount++;

	pthread_mutex_unlock(&philo->right_fork->fork);
	pthread_mutex_unlock(&philo->left_fork->fork);
}

void *philo_routine(void *arg)
{
	t_philosophers *philo = (t_philosophers *)arg;
	t_info         *rules = philo->info;

	if (rules->philosophers_number == 1)
	{
		pthread_mutex_lock(&philo->left_fork->fork);
		action_print(rules, philo->philosophers_id, "has taken a fork");

		{
			long long start = get_current_time_ms();
			while (!rules->finished &&
				   (get_current_time_ms() - start) < rules->time_to_die)
			{
				usleep(100);
			}
		}
		pthread_mutex_unlock(&philo->left_fork->fork);
		return (NULL);
	}

	if (philo->philosophers_id % 2 == 0)
		usleep(15000);

	while (!rules->finished)
	{
		philo_eats(philo);
		if (rules->finished)
			break;
		action_print(rules, philo->philosophers_id, "is sleeping");
		philo_usleep((long long)rules->time_to_sleep * 1000, philo);
		if (rules->finished)
			break;

		action_print(rules, philo->philosophers_id, "is thinking");
	}
	return (NULL);
}
