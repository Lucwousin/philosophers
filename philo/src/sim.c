/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   sim.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/02 17:16:45 by lsinke        #+#    #+#                 */
/*   Updated: 2022/06/02 17:16:45 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include "philo.h"
#define START_DELAY 500000

bool	start_and_detach(pthread_t *t, void *ign, void *(*fun)(void *), void *arg)
{
	(void) ign;
	if (pthread_create(t, NULL, fun, arg) != 0)
		return (false);
	//if (pthread_detach(*t) != 0)
	//	return (false);
	return (true);
}

static bool	create_threads(t_state *state)
{
	uint32_t	i;
	t_philo		*philos;

	i = 0;
	philos = state->philos;
	while (i < state->settings[N_PHILO])
	{
		if (!start_and_detach(&philos[i].thread, NULL, philosopher, philos + i))
			return (false);
		++i;
	}
	if (!start_and_detach(&state->watcher, NULL, watch_thread, state))
		return (false);
	return (true);
}

bool	simulate(t_state *state)
{
	if (!create_threads(state))
		return (false);
	usleep(START_DELAY);
	state->stopped = false;
	state->start_time = get_time();
	pthread_mutex_unlock(&state->run_sim);
	pthread_mutex_lock(&state->end_sim);
	pthread_mutex_unlock(&state->end_sim);
	printf("Simulation ended\n");
	return (true);
}