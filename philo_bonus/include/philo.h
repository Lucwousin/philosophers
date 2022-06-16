#ifndef PHILO_H
# define PHILO_H
# include <stdint.h>
# include <stdbool.h>
# include <unistd.h>
# include <semaphore.h>
# include <sysexits.h>

# define USAGE_MES "Usage: ./philo <n philo> <t die> <t eat> <t sleep> <n eat>"
# define ARGS_MES "All arguments should fit in a 32 bit unsigned int"
# define PHILO_N_MES "There should be more than one philosopher!"
# define ALLOC_MES "Failed to allocate memory for process ids"
# define SEM_MES "Failed to initialize semaphores"
# define SIM_MES "Something went wrong during simulation"
# define FORKS_SEM "/philo_forks"
# define START_SEM "/philo_start"
# define PRINT_SEM "/philo_print"
# define DIET_SEM "/philo_diet"

enum e_exitcode {
	SEM_FAILURE = EX_CONFIG + 1,
	EXIT_DEATH
};

enum e_setting {
	N_PHILO,
	T_DIE,
	T_EAT,
	T_SLEEP,
	N_EAT
};

typedef enum e_msg {
	FORK,
	EAT,
	SLEEP,
	THINK,
	DIE,
	END
}	t_msg;

typedef enum e_status {
	THINKING,
	EATING,
	SLEEPING,
	NO_STATUS
}	t_status;

typedef struct s_simulation {
	uint32_t	settings[5];
	uint64_t	start_time;
	pid_t		*philos;
	sem_t		*forks;
	sem_t		*start;
	sem_t		*print;
	sem_t		*enough;
	bool		done;
}	t_sim;

typedef struct s_philosopher {
	uint32_t	id;
	uint64_t	last_eaten;
	uint32_t	times_eaten;
	sem_t		*semaphore;
	char		sem_name[16];
	t_sim		*sim;
}	t_philo;

bool		parse_args(int argc, char **argv, uint32_t settings[5]);
bool		validate_philo_count(const uint32_t settings[5]);
bool		alloc_pid_arr(t_sim *sim);
bool		init_semaphores(t_sim *sim);
bool		simulate(t_sim *sim);

void		philosopher(uint32_t id, t_sim *sim);
void		go_think(t_philo *philo);
void		go_eat(t_philo *philo);
void		go_sleep(t_philo *philo);

void		send_message(t_sim *sim, uint32_t id, t_msg msg, uint64_t time);

uint64_t	get_time(void);
void		smart_sleep(uint64_t duration);
void		kill_all_children(t_sim *sim);
bool		create_and_detach(void *(*routine)(void *), void *arg);
char		*get_sem_name(uint32_t id, char name[16]);

#endif