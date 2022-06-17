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

// TODO: instead of copying in message loop, have 2 arrays and swap pointers (struct?)
// TODO: instead of monitoring thread, do it from main thread?
// TODO: check names of vars and funcs

#ifndef PHILO_H
# define PHILO_H
# include <pthread.h>
# include <stdint.h>
# include <stdbool.h>

# define USAGE_MES "Usage: ./philo <n philo> <t die> <t eat> <t sleep> <n eat>"
# define ARGS_MES "All arguments should fit in a 32 bit unsigned int"
# define PHILO_N_MES "There should be more than one philosopher!"
# define ALLOC_MES "Failed to allocate memory for forks or philosophers"
# define MUTEX_MES "Failed to initialize mutexes"
# define PHILO_MES "Failed to initialize philo mutexes"
# define THREAD_MES "Failed to create threads"

typedef pthread_mutex_t		t_mutex;

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

typedef struct s_simulation	t_sim;

typedef struct s_philo {
	uint32_t	id;
	pthread_t	thread;
	uint64_t	last_eaten;
	uint32_t	times_ate;
	t_mutex		*forks[2];
	t_mutex		eat_m;
	t_sim		*sim;
}	t_philo;

typedef struct s_msg_queue {
	uint32_t	count;
	uint32_t	*ids;
	uint32_t	*times;
	t_msg		*msgs;
	t_mutex		msg_mutex;
}	t_msg_queue;

typedef struct s_simulation {
	uint32_t	settings[5];
	t_philo		*philos;
	t_mutex		*forks;
	uint64_t	start_time;
	t_mutex		run_sim;
	pthread_t	watcher;
	pthread_t	printer;
	t_msg_queue	msg_queue;
	bool		stopped;
}	t_sim;

bool		parse_args(int argc, char **argv, uint32_t settings[5]);
bool		validate_philo_count(const uint32_t settings[5]);
bool		init_allocated_memory(t_sim *sim);
bool		init_philosophers(t_sim *sim);
bool		init_mutexes(t_sim *sim);

bool		simulate(t_sim *sim);
void		*philo_thread(void *arg);
void		*watch_thread(void *arg);

void		*listen_messages(void *arg);
void		queue_message(t_philo *philo, t_msg msg, uint32_t timestamp);

uint64_t	get_time(void);
void		smart_sleep(uint64_t duration);
void		*ft_calloc(size_t count, size_t size);
void		*ft_memcpy(void *dst, const void *src, size_t len);
bool		check_stopped(t_sim *sim);

#endif
