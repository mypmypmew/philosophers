#include "philo.h"
#include <stdio.h>    // printf
#include <stdlib.h>   // malloc

static int validate_input(t_info *rules)
{
	if (rules->philosophers_number < 1)
	{
		printf("Error: invalid number of philosophers (must be >=1)\n");
		return (0);
	}
	if (rules->time_to_die < 0 || rules->time_to_eat < 0 || rules->time_to_sleep < 0)
	{
		printf("Error: negative time values are not allowed.\n");
		return (0);
	}
	return (1);
}

static int init_forks(t_info *rules)
{
	int i;

	rules->forks = (t_fork *)malloc(sizeof(t_fork) * rules->philosophers_number);
	if (!rules->forks)
		return (0);

	i = 0;
	while (i < rules->philosophers_number)
	{
		if (pthread_mutex_init(&rules->forks[i].fork, NULL) != 0)
		{
			// Could destroy previously-initialized forks if needed
			printf("Error: could not init fork mutex at index %d\n", i);
			return (0);
		}
		rules->forks[i].id = i;
		i++;
	}
	return (1);
}

static int init_philo_array(t_info *rules)
{
	t_philosophers *philo;
	int             i;

	rules->philosophers = (t_philosophers *)malloc(
							sizeof(t_philosophers) * rules->philosophers_number);
	if (!rules->philosophers)
		return (0);

	i = 0;
	while (i < rules->philosophers_number)
	{
		philo = &rules->philosophers[i];
		philo->philosophers_id = i + 1;
		philo->full = 0;
		philo->last_meal = 0;
		philo->meals_amount = 0;
		philo->left_fork = &rules->forks[i];
		philo->right_fork = &rules->forks[(i + 1) % rules->philosophers_number];
		philo->info = rules;
		i++;
	}
	return (1);
}

static int init_mutexes(t_info *rules)
{
	if (pthread_mutex_init(&rules->print_mutex, NULL) != 0)
	{
		printf("Error: could not init print_mutex.\n");
		return (0);
	}
	if (pthread_mutex_init(&rules->meal_check, NULL) != 0)
	{
		printf("Error: could not init meal_check.\n");
		pthread_mutex_destroy(&rules->print_mutex);
		return (0);
	}
	if (pthread_mutex_init(&rules->dead_lock, NULL) != 0)
	{
		printf("Error: could not init dead_lock.\n");
		pthread_mutex_destroy(&rules->dead_lock);
		return (0);
	}

	return (1);
}

int init_philosophers(t_info *rules)
{
	if (!validate_input(rules))
		return (0);
	if (!init_forks(rules))
		return (0);
	if (!init_philo_array(rules))
		return (0);
	if (!init_mutexes(rules))
		return (0);
	return (1);
}