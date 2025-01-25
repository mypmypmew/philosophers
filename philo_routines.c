#include "philo.h"

void	handle_single_philosopher(t_philosophers *philo)
{
	t_info	*rules;
	int		local_finished;

	rules = philo->info;
	pthread_mutex_lock(&philo->left_fork->fork);
	action_print(rules, philo->philosophers_id, "has taken a fork");
	long long start = get_current_time_ms();
	while (1)
	{
		pthread_mutex_lock(&rules->dead_lock);
		local_finished = rules->finished;
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