#ifndef PHILO_H
# define PHILO_H
# include <stdint.h>
# include <stdbool.h>
# include <unistd.h>
# include <semaphore.h>

# define USAGE_MES "Usage: ./philo <n philo> <t die> <t eat> <t sleep> <n eat>"
# define ARGS_MES "All arguments should fit in a 32 bit unsigned int"
# define PHILO_N_MES "There should be more than one philosopher!"
# define ALLOC_MES "Failed to allocate memory for forks or philosophers"
# define MUTEX_MES "Failed to initialize mutexes"
# define PHILO_MES "Failed to initialize philo mutexes"
# define THREAD_MES "Failed to create threads"
# define FORKS_SEM "/philo_forks"
# define RUN_SEM "/philo_run"
# define DIET_SEM "/philo_diet"

enum e_setting {
	N_PHILO,
	T_DIE,
	T_EAT,
	T_SLEEP,
	N_EAT
};

typedef struct s_simulation {
	uint32_t	settings[5];
	uint64_t	start_time;
	pid_t		*philos;
	sem_t		*forks;
	sem_t		*start;
	sem_t		*enough;
}	t_sim;

typedef struct s_philosopher {
	uint32_t	id;
	uint64_t	last_eaten;
	uint32_t	times_eaten;
	sem_t		*semaphore;
	t_sim		*sim;
}	t_philo;

bool	parse_args(int argc, char **argv, uint32_t settings[5]);
bool	validate_philo_count(const uint32_t settings[5]);
void	kill_all_children(t_sim *sim);

#endif