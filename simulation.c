
#include "philo.h"

void *debug_routine(void *arg)
{
	t_philosophers *philo = (t_philosophers *)arg;

	pthread_mutex_lock(&philo->info->print_mutex);
	printf("[DEBUG] Hello from philosopher %d\n", philo->philosophers_id);
	printf("[DEBUG]  -> left fork id:  %d\n", philo->left_fork->id);
	printf("[DEBUG]  -> right fork id: %d\n", philo->right_fork->id);
	printf("[DEBUG]  -> time_to_die:   %d\n", philo->info->time_to_die);
	printf("[DEBUG]  -> time_to_eat:   %d\n", philo->info->time_to_eat);
	printf("[DEBUG]  -> time_to_sleep: %d\n", philo->info->time_to_sleep);
	pthread_mutex_unlock(&philo->info->print_mutex);
	usleep(1000 * 1000);
	return (NULL);
}

int start_simulation_debug(t_info *rules)
{
	int i = 0;
	t_philosophers *philos = rules->philosophers;

	while (i < rules->philosophers_number)
	{
		if (pthread_create(
				&philos[i].thread_id,
				NULL,
				debug_routine,
				&philos[i]
			) != 0)
		{
			printf("Error: pthread_create failed for philosopher %d\n", i + 1);
			return (0);
		}
		i++;
	}
	i = 0;
	while (i < rules->philosophers_number)
	{
		pthread_join(philos[i].thread_id, NULL);
		i++;
	}
	return (1);
}