
#include "philo.h"

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