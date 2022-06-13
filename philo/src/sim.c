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

#include <unistd.h>
#include "philo.h"

static bool	create_threads(t_state *state)
{
	uint32_t	i;
	t_philo		*philos;

	i = 0;
	philos = state->philos;
	while (i < state->settings[N_PHILO])
	{
		if (pthread_create(&philos[i].thread, NULL, philo_thread, philos + i))
			return (false);
		++i;
		usleep(500);
	}
	if (pthread_create(&state->watcher, NULL, watch_thread, state) || \
		pthread_create(&state->printer, NULL, listen_messages, state))
		return (false);
	return (true);
}

static void	join_threads(t_state *state)
{
	uint32_t	i;

	i = 0;
	while (i < state->settings[N_PHILO])
	{
		pthread_join(state->philos[i].thread, NULL);
		++i;
	}
	pthread_join(state->watcher, NULL);
	pthread_join(state->printer, NULL);
}

bool	simulate(t_state *state)
{
	state->stopped = false;
	pthread_mutex_lock(&state->run_sim);
	if (!create_threads(state))
		return (false);
	usleep(1000);
	state->start_time = get_time();
	pthread_mutex_unlock(&state->run_sim);
	join_threads(state);
	return (true);
}
