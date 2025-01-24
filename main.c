
#include "philo.h"

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

static void print_usage(void)
{
	printf("Usage: ./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [meals_amount]\n");
}

static int parse_args(int argc, char **argv, t_info **rules)
{
	if (argc != 5 && argc != 6)
	{
		print_usage();
		return 0;
	}
	if (!check_args(argv + 1, argc - 1))
	{
		printf("wrong arguments\n");
		return 0;
	}

	*rules = init_rules(NULL, argv + 1, argc - 1);
	if (!*rules)
	{
		printf("init_rules failed\n");
		return 0;
	}
	return 1;
}

static int init_all(t_info *rules)
{
	if (!init_philosophers(rules))
	{
		printf("init_philosophers failed\n");
		return 0;
	}
	return 1;
}

static int run_simulation(t_info *rules)
{
	if (launcher(rules) != 0)
	{
		// Adjust message if needed
		printf("launcher failed\n");
		return 0;
	}
	return 1;
}

static void cleanup(t_info *rules)
{
	if (rules->forks)
	{
		for (int i = 0; i < rules->philosophers_number; i++)
			pthread_mutex_destroy(&rules->forks[i].fork);
		free(rules->forks);
	}

	if (rules->philosophers)
		free(rules->philosophers);

	pthread_mutex_destroy(&rules->print_mutex);

	free(rules);
}

int main(int argc, char **argv)
{
	t_info *rules = NULL;

	if (!parse_args(argc, argv, &rules))
		return 1;

	if (!init_all(rules))
	{
		free(rules);
		return 1;
	}
	if (!run_simulation(rules))
	{
		cleanup(rules);
		return 1;
	}
	cleanup(rules);
	return 0;
}