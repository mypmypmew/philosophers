
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