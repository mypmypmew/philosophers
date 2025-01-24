
#include "philo.h"


long get_current_time_ms(void)
{
	struct timeval  tv;
	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void death_checker(t_info *rules)
{
	while (!rules->finished)
	{
		for (int i = 0; i < rules->philosophers_number && !rules->finished; i++)
		{
			pthread_mutex_lock(&rules->meal_check);
			long now = get_current_time_ms();
			long elapsed = now - rules->philosophers[i].last_meal;
			pthread_mutex_unlock(&rules->meal_check);

			if (elapsed > rules->time_to_die)
			{
				pthread_mutex_lock(&rules->print_mutex);
				printf("%ld %d died\n", now - rules->start_time,
										rules->philosophers[i].philosophers_id);
				pthread_mutex_unlock(&rules->print_mutex);

				rules->finished = 1;
			}
		}
		if (rules->meals_amount != -1)
{
	int i;
	int count = 0;

	for (i = 0; i < rules->philosophers_number; i++)
	{
		// We might need to lock meal_check before reading philosophers[i].meals_amount
		pthread_mutex_lock(&rules->meal_check);
		if (rules->philosophers[i].meals_amount >= rules->meals_amount)
			count++;
		pthread_mutex_unlock(&rules->meal_check);
	}

	if (count == rules->philosophers_number && !rules->finished)
	{
		pthread_mutex_lock(&rules->print_mutex);
		// Optionally print something or just mark finished
		rules->finished = 1;
		pthread_mutex_unlock(&rules->print_mutex);
	}
}
		usleep(1000);
	}
}

int launcher(t_info *rules)
{
	int i;

	rules->start_time = get_current_time_ms();

	i = 0;
	while (i < rules->philosophers_number)
	{
		rules->philosophers[i].last_meal = rules->start_time;

		if (pthread_create(
			&rules->philosophers[i].thread_id,
			NULL,
			philo_routine,
			&rules->philosophers[i]) != 0)
		{
			printf("Error creating thread %d\n", i);
			return (1);
		}
		i++;
	}

	death_checker(rules);

	i = 0;
	while (i < rules->philosophers_number)
	{
		pthread_join(rules->philosophers[i].thread_id, NULL);
		i++;
	}

	return (0);
}