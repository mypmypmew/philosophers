#include "philo.h"
#include <stdio.h>  // printf
#include <unistd.h> // usleep

long get_current_time_ms(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

static void check_death(t_info *rules)
{
	int i;
	long now;
	long elapsed;

	i = 0;
	while (i < rules->philosophers_number && !rules->finished)
	{
		pthread_mutex_lock(&rules->meal_check);
		now = get_current_time_ms();
		elapsed = now - rules->philosophers[i].last_meal;
		pthread_mutex_unlock(&rules->meal_check);

		if (elapsed > rules->time_to_die)
		{
			pthread_mutex_lock(&rules->print_mutex);
			printf("%ld %d died\n", now - rules->start_time,
									rules->philosophers[i].philosophers_id);
			pthread_mutex_unlock(&rules->print_mutex);
			rules->finished = 1;
		}
		i++;
	}
}

static void check_meals(t_info *rules)
{
	int i;
	int count;

	if (rules->meals_amount == -1)
		return;

	i = 0;
	count = 0;
	while (i < rules->philosophers_number)
	{
		pthread_mutex_lock(&rules->meal_check);
		if (rules->philosophers[i].meals_amount >= rules->meals_amount)
			count++;
		pthread_mutex_unlock(&rules->meal_check);
		i++;
	}

	if (count == rules->philosophers_number && !rules->finished)
	{
		pthread_mutex_lock(&rules->print_mutex);
		rules->finished = 1;
		pthread_mutex_unlock(&rules->print_mutex);
	}
}

static void monitor_philosophers(t_info *rules)
{
	while (!rules->finished)
	{
		check_death(rules);
		check_meals(rules);
		usleep(1000);
	}
}

static int create_philosopher_threads(t_info *rules)
{
	int i;

	i = 0;
	while (i < rules->philosophers_number)
	{
		rules->philosophers[i].last_meal = rules->start_time;
		if (pthread_create(&rules->philosophers[i].thread_id,
						   NULL,
						   philo_routine,
						   &rules->philosophers[i]) != 0)
		{
			printf("Error creating thread %d\n", i);
			return (0);
		}
		i++;
	}
	return (1);
}

static void join_philosopher_threads(t_info *rules)
{
	int i;
	i = 0;
	while (i < rules->philosophers_number)
	{
		pthread_join(rules->philosophers[i].thread_id, NULL);
		i++;
	}
}

int launcher(t_info *rules)
{
	rules->start_time = get_current_time_ms();
	if (!create_philosopher_threads(rules))
		return (1);
	monitor_philosophers(rules);
	join_philosopher_threads(rules);
	return (0);
}
