
#include "philo.h"

// struct timeval {
//     time_t      tv_sec;     // seconds
//     suseconds_t tv_usec;    // microseconds
// };

uint64_t	get_time(void) // returns time in milliseconds
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		return (0);
	return ((tv.tv_sec * (uint64_t)1000) + (tv.tv_usec / 1000));
}

void    example_1()
{
	struct timeval time;

	gettimeofday(&time, NULL);
	printf("%ld seconds\n", time.tv_sec);
	sleep(5);
	gettimeofday(&time, NULL);
	printf("%ld seconds\n", time.tv_sec);

	printf("\n%ld years passed since 1970\n", time.tv_sec / 60 / 60 / 24 / 365);
}

void    example_2()
{
	uint64_t    start_time;
	uint64_t    now;

	start_time = get_time();
	usleep(200000); // accepts in microseconds
	now = get_time();
	printf("%llu milliseconds passed since the start\n", now - start_time);
}

int main(int argc, char **argv)
{
	// 1 - check and read data

	// 2 - create in a loop pthread_create for each philo
	//and pass routine function to every philosopher and pass data

	// 3 - main thread check that every philo is alive

	// 4 - if somebody died -> join all threads, then destroy mutexes

	/*

	void *philo(void *data)
	{
		if(data->id % 2 == 0)
		{
			usleep(2500);
		}
		need a global mutex for printf;

		while (1)
		{
			need to protect printf with mutex;

			printf("thinking");
			lock(left);
			printf("took left fork");
			lock(right);
			printf("took right fork");
			print(philo eating);
			last_eating_time = gettimeofday();
			usleep(time to eat * 1000);
			unlock
			unlock
			printf("sleeping");
			usleep(time to sleep * 1000);
		}
	}


	function is_dead(t_philos *philos)
	{
		struct philo
		{
			time_to_die
			time_to_sleep
			time_to_eat
		mutex	*left fork
		mutex	*right fork;
		struct timeval {
			long	last_meal_time
		}
		must_die; ?
		}


		gettimeoftade - last_meal_
	}


	allowed function

	gettimeofday(struct *timeval, 0)
	{

	}

	stuct timeval {
		long tv_sec; - tv_sec * 1000 + tv_usec/1000
		long tv_usec;
	}


	void ft_usleep(int ms)
	{
		struct timeval start;
		struct timeval curr_time;

		gettimeofday(&start, 0);
		gettimeofday(&curr_time, 0);
		while( (now.tv_sec - start.tv_sec) * 1000 + (now.tvusec - start.tv_usec)/1000) < ms)
		{
			usleep(10);
			gettimeofday(&curr_time);
		}

	}
	*/





	return (0);
}