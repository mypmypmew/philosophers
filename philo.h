

#include <pthread.h>
#include <stdio.h>
#include <sys/time.h> // time
#include <stdint.h> // uint64_t
#include <unistd.h>  //usleep

typedef struct s_info t_info;

typedef struct s_fork {
	pthread_mutex_t	fork;
	int				id;
} t_fork;


typedef struct s_philosophers
{
	int	philosophers_id;
	int		meals_amount;
	int		full;
	long	last_meal;
	t_fork	*left_fork;
	t_fork	*right_fork;
	pthread_t	thread_id;
	t_info		*info;

} t_philosophers;

typedef struct	s_info
{
	int		philosophers_number;
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
	int		meals_amount;
	int		finished;
	t_fork	*forks;
	t_philosophers	*philosophers;
} t_info;
