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

static bool	check_death(t_philo *philo, uint64_t time)
{
	if (time - philo->last_eaten <= philo->state->settings[T_DIE])
		return (false);
	print_message(philo, DIE);
	pthread_mutex_unlock(&philo->eat_m);
	return (true);
}

static bool	check_philos(t_state *state)
{
	t_philo		*philo;
	uint32_t	i;
	uint32_t	fed_count;
	uint64_t	cur_time;

	i = 0;
	fed_count = 0;
	while (i < state->settings[N_PHILO])
	{
		philo = state->philos + i;
		pthread_mutex_lock(&philo->eat_m);
		cur_time = get_time();
		if (check_death(philo, cur_time))
			return (true);
		if (state->settings[N_EAT] != UINT32_MAX && \
			philo->times_ate >= state->settings[N_EAT])
			++fed_count;
		pthread_mutex_unlock(&philo->eat_m);
		++i;
	}
	return (fed_count == state->settings[N_PHILO]);
}

void	*watch_thread(void *arg)
{
	t_state	*state;

	state = (t_state *) arg;
	pthread_mutex_lock(&state->run_sim);
	pthread_mutex_unlock(&state->run_sim);
	usleep(state->settings[T_DIE] / 2 * 1000);
	while (true)
	{
		if (check_philos(state))
		{
			pthread_mutex_lock(&state->run_sim);
			state->stopped = true;
			pthread_mutex_unlock(&state->run_sim);
			pthread_mutex_unlock(&state->end_sim);
			return (NULL);
		}
		usleep(1000);
	}
}
