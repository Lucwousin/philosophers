/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   sim.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: lucas <lucas@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/13 15:22:30 by lucas         #+#    #+#                 */
/*   Updated: 2022/06/13 15:22:30 by lucas         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */
#include "philo.h"
#include <pthread.h>
#include <stdlib.h>

void	*dietician(void *arg)
{
	t_sim		*sim;
	uint32_t	enough_count;

	sim = arg;
	enough_count = 0;
	while (enough_count != sim->settings[N_PHILO])
	{
		sem_wait(sim->enough);
		if (sim->done)
			return (NULL);
		++enough_count;
	}
	send_message(sim, sim->settings[N_PHILO], END, get_time());
	kill_all_children(sim);
	return (NULL);
}

static bool	birth_children(t_sim *sim)
{
	uint32_t	i;
	pid_t		pid;

	i = 0;
	while (i < sim->settings[N_PHILO])
	{
		pid = fork();
		if (pid < 0)
			return (false);
		if (pid == 0)
		{
			philosopher(i, sim);
			exit(EXIT_FAILURE);
		}
		sim->philos[i++] = pid;
	}
	return (true);
}

bool	simulate(t_sim *sim)
{
	sim->done = false;
	if (sim->settings[N_EAT] != UINT32_MAX)
		if (!create_and_detach(dietician, sim))
			return (false);
	sim->start_time = get_time() + (sim->settings[N_PHILO] / 2 + 2);
	if (!birth_children(sim))
		return (false);
	smart_sleep(sim->start_time - get_time());
	sem_post(sim->start);
	return (true);
}