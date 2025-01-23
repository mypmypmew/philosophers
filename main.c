
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

int	ft_isdigit(int c)
{
	return ((c >= '0' && c <= '9'));
}

static long ft_atol(const char *str)
{
	long result;
	int  i;

	i = 0;
	result = 0;

	while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
		i++;
	while (str[i] && ft_isdigit((unsigned char)str[i]))
	{
		long prev = result;
		result = result * 10 + (str[i] - '0');
		if (result < prev || result > 2147483647)
			return (-1);
		i++;
	}
	return (result);
}

static int is_digit_str(const char *str)
{
	int i = 0;

	if (!str || !*str)
		return (0);
	while (str[i])
	{
		if (!ft_isdigit((unsigned char)str[i]))
			return (0);
		i++;
	}
	return (1);
}

static int check_args(char **args, int count)
{
	int i = 0;

	while (i < count)
	{
		if (!is_digit_str(args[i]))
			return (0);
		long val = ft_atol(args[i]);
		if (val <= 0)
			return (0);
		i++;
	}
	return (1);
}

t_info *init_rules(t_info *rules, char **args, int arg_count)
{
	rules = (t_info *)malloc(sizeof(t_info));
	if(!rules)
		return (NULL);

	rules->philosophers_number = (int)ft_atol(args[0]);
	rules->time_to_die = (int)ft_atol(args[1]);
	rules->time_to_eat = (int)ft_atol(args[2]);
	rules->time_to_sleep = (int)ft_atol(args[3]);

	if(arg_count == 5)
		rules->meals_amount = (int)ft_atol(args[4]);
	else
		rules->meals_amount = -1;

	rules->finished = 0;
	rules->forks = NULL;
	rules->philosophers = NULL;
	return rules;
}


int main(int argc, char **argv)
{
	t_info  *rules;

	if (argc != 5 && argc != 6)
	{
		printf("wrong arguments\n");
		return (1);
	}

	if (!check_args(argv + 1, argc - 1))
	{
		printf("wrong arguments\n");
		return (1);
	}

	rules = init_rules(NULL, argv + 1, argc - 1);
	if (!rules)
	{
		printf("init_rules failed\n");
		return (1);
	}

	printf("=== Debug: Values from init_rules ===\n");
	printf("Philosophers number: %d\n", rules->philosophers_number);
	printf("time_to_die:         %d\n", rules->time_to_die);
	printf("time_to_eat:         %d\n", rules->time_to_eat);
	printf("time_to_sleep:       %d\n", rules->time_to_sleep);
	printf("meals_amount (5th):  %d\n", rules->meals_amount);
	printf("-------------------------------------\n\n");

	if (!init_philosophers(rules))
	{
		printf("init_philosophers failed\n");
		free(rules);
		return (1);
	}

	printf("=== Debug: After init_philosophers ===\n");
	printf("Fork array address: %p\n", (void *)rules->forks);
	printf("Philosopher array address: %p\n\n", (void *)rules->philosophers);

	for (int i = 0; i < rules->philosophers_number; i++)
	{
		t_philosophers *p = &rules->philosophers[i];
		printf("Philo #%d\n", p->philosophers_id);
		printf("  left_fork -> id:  %d\n", p->left_fork->id);
		printf("  right_fork -> id: %d\n", p->right_fork->id);
		printf("  last_meal:        %ld\n", p->last_meal);
		printf("  meals_amount:      %d\n", p->meals_amount);
		printf("  full:             %d\n", p->full);
		printf("\n");
	}

	if (rules->forks)
	{
		// Destroy each fork's mutex
		for (int i = 0; i < rules->philosophers_number; i++)
			pthread_mutex_destroy(&rules->forks[i].fork);
		free(rules->forks);
	}
	if (rules->philosophers)
		free(rules->philosophers);

	// If you initialized a print_mutex, destroy it:
	pthread_mutex_destroy(&rules->print_mutex);

	free(rules);

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


	*/

	return (0);
}