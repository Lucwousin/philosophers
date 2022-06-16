/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   watcher.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/03 14:31:13 by lsinke        #+#    #+#                 */
/*   Updated: 2022/06/03 14:31:13 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>
#include <stdio.h>

static void	set_stopped(t_sim *sim)
{
	pthread_mutex_lock(&sim->run_sim);
	sim->stopped = true;
	pthread_mutex_unlock(&sim->run_sim);
}

static bool	check_death(t_philo *philo)
{
	uint64_t	time;

	time = get_time();
	if (time - philo->last_eaten <= philo->sim->settings[T_DIE])
		return (false);
	queue_message(philo, DIE, time - philo->sim->start_time);
	pthread_mutex_unlock(&philo->eat_m);
	set_stopped(philo->sim);
	return (true);
}

static bool	check_philos(t_sim *sim)
{
	t_philo		*philo;
	uint32_t	i;
	uint32_t	fed_count;

	i = 0;
	fed_count = 0;
	while (i < sim->settings[N_PHILO])
	{
		philo = sim->philos + i;
		pthread_mutex_lock(&philo->eat_m);
		if (check_death(philo))
			return (true);
		if (sim->settings[N_EAT] != UINT32_MAX && \
			philo->times_ate >= sim->settings[N_EAT])
			++fed_count;
		pthread_mutex_unlock(&philo->eat_m);
		++i;
	}
	if (fed_count != sim->settings[N_PHILO])
		return (false);
	queue_message(philo, END, get_time() - sim->start_time);
	set_stopped(sim);
	return (true);
}

void	*watch_thread(void *arg)
{
	t_sim	*sim;

	sim = (t_sim *) arg;
	pthread_mutex_lock(&sim->run_sim);
	pthread_mutex_unlock(&sim->run_sim);
	smart_sleep(sim->settings[T_DIE] - 1);
	while (true)
	{
		if (check_philos(sim))
			return (NULL);
		smart_sleep(1);
	}
}
