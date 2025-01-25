
#include "philo.h"

long long	calculate_remaining_time(long long start, long long target_us)
{
	long long now;
	now = current_time_microseconds();
	return (target_us - (now - start));
}

long long current_time_microseconds(void)
{
	struct timeval now;
	gettimeofday(&now, NULL);
	return ((long long)now.tv_sec * 1000000) + now.tv_usec;
}

void	short_sleep_loop(long long start, long long target_us, t_philosophers *philo)
{
	int local_finished;

	while (1)
	{
		check_finished(philo, &local_finished);
		if (local_finished)
			break;
		if (current_time_microseconds() - start >= target_us)
			break;
	}
}

void philo_usleep(long long target_us, t_philosophers *philo)
{
	long long start;
	int local_finished;
	long long remaining;

	start = current_time_microseconds();
	while (1)
	{
		check_finished(philo, &local_finished);
		if (local_finished)
			break;
		remaining = calculate_remaining_time(start, target_us);
		if (remaining <= 0)
			break;
		if (remaining > 1000)
			usleep(remaining / 2);
		else
		{
			short_sleep_loop(start, target_us, philo);
			break;
		}
	}
}