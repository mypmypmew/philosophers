#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>

void ft_usleep(int ms)
{
	struct timeval start;
	struct timeval curr_time;
	long elapsed_time;

	gettimeofday(&start, NULL);
	elapsed_time = 0;

	while (elapsed_time < ms)
	{
		usleep(50);
		gettimeofday(&curr_time, NULL);

		elapsed_time = (curr_time.tv_sec - start.tv_sec) * 1000 +
					   (curr_time.tv_usec - start.tv_usec) / 1000;
	}
}

long get_time_in_ms()
{
	struct timeval time_now;

	gettimeofday(&time_now, NULL);
	return (time_now.tv_sec * 1000) + (time_now.tv_usec / 1000);
}

// int main()
// {
// 	int delay = 1000;
// 	long start_time, end_time, elapsed_time;

// 	start_time = get_time_in_ms();

// 	ft_usleep(delay);

// 	end_time = get_time_in_ms();

// 	elapsed_time = end_time - start_time;

// 	printf("Requested delay: %d ms\n", delay);
// 	printf("Actual delay: %ld ms\n", elapsed_time);

// 	if (elapsed_time >= delay && elapsed_time < delay + 10)
// 		printf("ft_usleep works correctly.\n");
// 	else
// 		printf("ft_usleep is inaccurate.\n");

// 	return 0;
// }