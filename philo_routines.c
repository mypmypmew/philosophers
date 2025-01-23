#include "philo.h"


void action_print(t_info *rules, int philo_id, const char *msg)
{
	long now;

	pthread_mutex_lock(&rules->print_mutex);

	if (!rules->finished)
	{
		now = get_current_time_ms() - rules->start_time;
		printf("%ld %d %s\n", now, philo_id, msg);
	}

	pthread_mutex_unlock(&rules->print_mutex);
}

void smart_sleep(int duration_ms, t_info *rules)
{
	long start = get_current_time_ms();
	while (!rules->finished && (get_current_time_ms() - start < duration_ms))
		usleep(100);  // Sleep 100 microseconds at a time
}

void *philo_routine(void *arg)
{
	t_philosophers *philo = (t_philosophers *)arg;
	t_info         *rules = philo->info;

	if (philo->philosophers_id % 2 == 0)
		usleep(15000);

	while (!rules->finished)
	{
		philo_eats(philo);

		if (rules->finished)
			break;

		action_print(rules, philo->philosophers_id, "is sleeping");
		smart_sleep(rules->time_to_sleep, rules);
		if (rules->finished)
			break;

		action_print(rules, philo->philosophers_id, "is thinking");
	}

	return (NULL);
}