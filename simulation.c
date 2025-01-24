
#include "philo.h"


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