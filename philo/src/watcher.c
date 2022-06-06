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

bool	check_death(t_philo *philo, uint64_t time)
{
	if (time - philo->last_eaten <= philo->state->settings[T_DIE])
		return (false);
	send_message(philo, DIE);
	return (true);
}

void	*watch_thread(void *arg)
{
	t_state		*state;
	t_philo		*philo;
	uint32_t	i;
	uint64_t	time;

	state = (t_state *) arg;
	pthread_mutex_lock(&state->run_sim);
	pthread_mutex_unlock(&state->run_sim);
	usleep(state->settings[T_DIE] / 2 * 1000);
	while (true)
	{
		i = 0;
		while (i < state->settings[N_PHILO])
		{
			philo = state->philos + i;
			pthread_mutex_lock(&philo->eat_m);
			time = get_time();
			if (check_death(philo, time))
			{
				pthread_mutex_unlock(&state->end_sim);
				pthread_mutex_unlock(&philo->eat_m);
				return ((void *) 0);
			}
			
			pthread_mutex_unlock(&philo->eat_m);
			++i;
		}
	}
}

void	*end_watcher(void *arg)
{
	t_state		*state;
	t_philo		*philo;
	uint32_t	enough_count;
	uint32_t	i;

	state = (t_state *) arg;
	pthread_mutex_lock(&state->run_sim);
	pthread_mutex_unlock(&state->run_sim);
	while (true)
	{
		enough_count = 0;
		i = 0;
		while (i < state->settings[N_PHILO])
		{
			philo = state->philos + i;
			pthread_mutex_lock(&philo->eat_m);
			if (philo->times_ate >= state->settings[N_EAT])
				++enough_count;
			pthread_mutex_unlock(&philo->eat_m);
			++i;
		}
		if (enough_count == state->settings[N_PHILO])
			break ;
	}
	pthread_mutex_unlock(&state->end_sim);
	return ((void *) 0);
}