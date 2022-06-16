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

#include "philo.h"
#include <unistd.h>

static bool	create_threads(t_sim *sim)
{
	uint32_t	i;
	t_philo		*philos;

	i = 0;
	philos = sim->philos;
	while (i < sim->settings[N_PHILO])
	{
		if (pthread_create(&philos[i].thread, NULL, philo_thread, philos + i))
			return (false);
		++i;
	}
	if (pthread_create(&sim->watcher, NULL, watch_thread, sim) || \
		pthread_create(&sim->printer, NULL, listen_messages, sim))
		return (false);
	return (true);
}

static void	join_threads(t_sim *sim)
{
	uint32_t	i;

	i = 0;
	while (i < sim->settings[N_PHILO])
	{
		pthread_join(sim->philos[i].thread, NULL);
		++i;
	}
	pthread_join(sim->watcher, NULL);
	pthread_join(sim->printer, NULL);
}

bool	simulate(t_sim *sim)
{
	sim->stopped = false;
	pthread_mutex_lock(&sim->run_sim);
	if (!create_threads(sim))
		return (false);
	smart_sleep(1);
	sim->start_time = get_time();
	pthread_mutex_unlock(&sim->run_sim);
	join_threads(sim);
	return (true);
}
