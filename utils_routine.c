
#include "philo.h"

void	lock_forks(t_philosophers *philo)
{
	if (philo->left_fork->id < philo->right_fork->id)
	{
		pthread_mutex_lock(&philo->left_fork->fork);
		action_print(philo->info, philo->philosophers_id, "has taken a fork");
		pthread_mutex_lock(&philo->right_fork->fork);
		action_print(philo->info, philo->philosophers_id, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&philo->right_fork->fork);
		action_print(philo->info, philo->philosophers_id, "has taken a fork");
		pthread_mutex_lock(&philo->left_fork->fork);
		action_print(philo->info, philo->philosophers_id, "has taken a fork");
	}
}

void	unlock_forks(t_philosophers *philo)
{
	if (philo->left_fork->id < philo->right_fork->id)
	{
		pthread_mutex_unlock(&philo->right_fork->fork);
		pthread_mutex_unlock(&philo->left_fork->fork);
	}
	else
	{
		pthread_mutex_unlock(&philo->left_fork->fork);
		pthread_mutex_unlock(&philo->right_fork->fork);
	}
}

void	update_last_meal(t_philosophers *philo)
{
	pthread_mutex_lock(&philo->info->meal_check);
	philo->last_meal = get_current_time_ms();
	pthread_mutex_unlock(&philo->info->meal_check);
}

void	increment_meals(t_philosophers *philo)
{
	pthread_mutex_lock(&philo->info->meal_check);
	philo->meals_amount++;
	pthread_mutex_unlock(&philo->info->meal_check);
}

void	philo_eats(t_philosophers *philo)
{
	lock_forks(philo);
	update_last_meal(philo);
	action_print(philo->info, philo->philosophers_id, "is eating");
	philo_usleep((long long)philo->info->time_to_eat * 1000, philo);
	increment_meals(philo);
	unlock_forks(philo);
}
