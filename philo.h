

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
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
	long	start_time;
	t_fork	*forks;
	t_philosophers	*philosophers;

	pthread_mutex_t print_mutex;
	pthread_mutex_t meal_check;
	pthread_mutex_t dead_lock;
} t_info;

int init_philosophers(t_info *rules);
int start_simulation_debug(t_info *rules);
void *philo_routine(void *arg);
int launcher(t_info *rules);
long get_current_time_ms(void);

void		action_print(t_info *rules, int philo_id, const char *msg);
long long	calculate_remaining_time(long long start, long long target_us);
long long	current_time_microseconds(void);
void		philo_usleep(long long target_us, t_philosophers *philo);
void		check_finished(t_philosophers *philo, int *local_finished);
void		philo_usleep(long long target_us, t_philosophers *philo);

void		lock_forks(t_philosophers *philo);
void		unlock_forks(t_philosophers *philo);
void		update_last_meal(t_philosophers *philo);
void		update_last_meal(t_philosophers *philo);
void		increment_meals(t_philosophers *philo);
void		philo_eats(t_philosophers *philo);