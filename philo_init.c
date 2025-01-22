#include "philo.h"

int init_philosophers(t_info *rules)
{
	int	i;

	if (rules->philosophers_number < 2 || rules->philosophers_number > 250)
	{
		printf("Error: invalid number of philosophers (must be >=2 and <=250)\n");
		return (0);
	}
	if (rules->time_to_die < 0 || rules->time_to_eat < 0 || rules->time_to_sleep < 0)
	{
		printf("Error: negative time values are not allowed.\n");
		return (0);
	}

	rules->forks = (t_fork *)malloc(sizeof(t_fork) * rules->philosophers_number);
	if (!rules->forks)
		return (NULL);
	i = 0;
	while(i < rules->philosophers_number)
	{
		if(pthread_mutex_init(&rules->forks[i].fork, NULL) != 0)
		{
			//add destroy function
			return (0);
		}
		rules->forks[i].id = i;
		i++;
	}
	rules->philosophers = (t_philosophers *)malloc(
		sizeof(t_philosophers) * rules->philosophers_number);
	if(!rules->philosophers)
		return (0); //error;
	i = 0;
	while(i < rules->philosophers_number)
	{
		rules->philosophers[i].philosophers_id = i+1;
		rules->philosophers[i].full = 0;
		rules->philosophers[i].last_meal = 0;
		rules->philosophers[i].meals_amount = 0;
		rules->philosophers[i].left_fork = &rules->forks[i];
		rules->philosophers[i].right_fork
			= &rules->forks[(i + 1) % rules->philosophers_number];
		rules->philosophers[i].info = rules;
		i++;
	}
	if (pthread_mutex_init(&rules->print_mutex, NULL) != 0)
	{
		printf("Error: could not init print_mutex.\n");
		return (0);
	}
	return (1);
}