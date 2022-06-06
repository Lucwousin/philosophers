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
	if (pthread_mutex_init(&state->print_m, NULL) != 0 || \
		pthread_mutex_init(&state->end_sim, NULL) != 0 || \
        pthread_mutex_init(&state->run_sim, NULL) != 0)
		return (false);
	pthread_mutex_lock(&state->end_sim);
	pthread_mutex_lock(&state->run_sim);
	return (true);
}

bool	allocate_arrays(t_state *state)
{
	state->philos = ft_calloc(state->settings[N_PHILO], sizeof(t_philo));
	state->forks = ft_calloc(state->settings[N_PHILO], sizeof(t_mutex));
	return (state->philos != NULL && state->forks != NULL);
}