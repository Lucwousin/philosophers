/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/02 14:45:14 by lsinke        #+#    #+#                 */
/*   Updated: 2022/06/02 14:45:14 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	create_philosopher(t_philo *p, t_state *state, uint32_t id)
{
	p->id = id;
	p->times_ate = 0;
	p->state = state;
	if (pthread_mutex_init(&p->eat_m, NULL) != 0)
		return (false);
	p->forks[0] = state->forks + p->id;
	p->forks[1] = state->forks + ((p->id + 1) % state->settings[N_PHILO]);
	return (true);
}

bool	init_philosophers(t_state *state)
{
	uint32_t	i;

	i = 0;
	while (i < state->settings[N_PHILO])
	{
		if (!create_philosopher(state->philos + i, state, i))
			return (false);
		++i;
	}
	return (true);
}

bool	init_mutexes(t_state *state)
{
	uint32_t		i;

	i = 0;
	while (i < state->settings[N_PHILO])
	{
		if (pthread_mutex_init(state->forks + i, NULL) != 0)
			return (false);
		++i;
	}
	if (pthread_mutex_init(&state->msg_queue.msg_mutex, NULL) != 0 || \
		pthread_mutex_init(&state->run_sim, NULL) != 0)
		return (false);
	return (true);
}

bool	init_allocated_memory(t_state *state)
{
	uint32_t	num_philos;
	t_msg_queue	*queue;

	num_philos = state->settings[N_PHILO];
	queue = &state->msg_queue;
	state->philos = ft_calloc(num_philos, sizeof(t_philo));
	state->forks = ft_calloc(num_philos, sizeof(t_mutex));
	queue->msgs = ft_calloc(num_philos * 4, sizeof(t_msg));
	queue->ids = ft_calloc(num_philos * 4, sizeof(uint32_t));
	queue->times = ft_calloc(num_philos * 4, sizeof(uint32_t));
	queue->count = 0;
	return (state->philos != NULL && state->forks != NULL && \
		queue->msgs != NULL && queue->ids != NULL && queue->times != NULL);
}
