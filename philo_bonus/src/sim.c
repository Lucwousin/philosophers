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
	while (true)
	{
		sem_wait(sim->enough);
		++enough_count;
		if (enough_count == sim->settings[N_PHILO])
		{
			kill_all_children(sim);
			return (NULL);
		}
	}
}

static bool	create_and_detach(void *(*routine)(void *), void *arg)
{
	pthread_t	thread;

	if (pthread_create(&thread, NULL, routine, arg) != 0)
		return (false);
	if (pthread_detach(thread) != 0)
		return (false);
	return (true);
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
		sim->philos[i] = pid;
	}
	return (true);
}

bool	simulate(t_sim *sim)
{
	if (sim->settings[N_EAT] != UINT32_MAX)
		if (!create_and_detach(dietician, sim))
			return (false);
	if (!birth_children(sim))
		return (false);
}