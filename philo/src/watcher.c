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

#include <unistd.h>
#include "philo.h"

static void	set_stopped(t_state *state)
{
	pthread_mutex_lock(&state->run_sim);
	state->stopped = true;
	pthread_mutex_unlock(&state->run_sim);
}

static bool	check_death(t_philo *philo)
{
	if (get_time() - philo->last_eaten <= philo->state->settings[T_DIE])
		return (false);
	set_stopped(philo->state);
	print_message(philo, DIE);
	pthread_mutex_unlock(&philo->eat_m);
	return (true);
}

static bool	check_philos(t_state *state)
{
	t_philo		*philo;
	uint32_t	i;
	uint32_t	fed_count;

	i = 0;
	fed_count = 0;
	while (i < state->settings[N_PHILO])
	{
		philo = state->philos + i;
		pthread_mutex_lock(&philo->eat_m);
		if (check_death(philo))
			return (true);
		if (state->settings[N_EAT] != UINT32_MAX && \
			philo->times_ate >= state->settings[N_EAT])
			++fed_count;
		pthread_mutex_unlock(&philo->eat_m);
		++i;
	}
	if (fed_count != state->settings[N_PHILO])
		return (false);
	set_stopped(state);
	return (true);
}

void	*watch_thread(void *arg)
{
	t_state	*state;

	state = (t_state *) arg;
	pthread_mutex_lock(&state->run_sim);
	pthread_mutex_unlock(&state->run_sim);
	usleep(state->settings[T_DIE] * 1000 / 2);
	while (true)
	{
		if (check_philos(state))
			return (NULL);
		usleep(750);
	}
}
