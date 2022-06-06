/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/02 13:38:37 by lsinke        #+#    #+#                 */
/*   Updated: 2022/06/02 13:38:37 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <stdint.h>
# include <stdbool.h>
# include <pthread.h>

# define USAGE_MES "Usage: ./philo <n philo> <t die> <t eat> <t sleep> <n eat>"
# define ARGS_MES "All arguments should fit in a 32 bit unsigned int"
# define ALLOC_MES "Failed to allocate memory for forks or philosophers"
# define MUTEX_MES "Failed to initialize mutexes"
# define PHILO_MES "Failed to initialize philo mutexes"
# define THREAD_MES "Failed to create threads"

typedef pthread_mutex_t	t_mutex;

enum e_setting {
	N_PHILO,
	T_DIE,
	T_EAT,
	T_SLEEP,
	N_EAT
};

typedef enum e_status {
	THINKING,
	EATING,
	SLEEPING
}	t_status;

typedef enum e_msg {
	FORK, EAT, SLEEP, THINK, DIE
}	t_msg;

typedef struct s_state	t_state;

typedef struct s_philo {
	uint32_t	id;
	pthread_t	thread;
	uint64_t	last_eaten;
	uint32_t	times_ate;
	t_mutex		eat_m;
	t_state		*state;
	t_status	status;
}	t_philo;

typedef struct s_state {
	uint32_t	settings[5];
	t_philo		*philos;
	t_mutex		*forks;
	uint64_t	start_time;
	t_mutex		print_m;
	t_mutex		run_sim;
	pthread_t	watcher;
	t_mutex		end_sim;
}	t_state;

bool	parse_args(int argc, char **argv, uint32_t settings[5]);
bool	allocate_arrays(t_state *state);
bool	init_philosophers(t_state *state);
bool	init_mutexes(t_state *state);

bool	simulate(t_state *state);
void	*philosopher(void *arg);
void	*watch_thread(void *arg);
void	*end_watcher(void *arg);

void	send_message(t_philo *philo, t_msg msg);

uint64_t	get_time(void);
void	*ft_calloc(size_t count, size_t size);

#endif