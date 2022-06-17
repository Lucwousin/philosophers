/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/13 15:36:23 by lsinke        #+#    #+#                 */
/*   Updated: 2022/06/13 15:36:23 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <fcntl.h>
#include <stdlib.h>

bool	alloc_pid_arr(t_sim *sim)
{
	sim->philos = malloc(sim->settings[N_PHILO] * sizeof(pid_t));
	return (sim->philos != NULL);
}

bool	init_semaphores(t_sim *sim)
{
	bool	err;
	int16_t	mode;

	err = false;
	mode = O_CREAT | O_EXCL;
	sem_unlink(FORKS_SEM);
	sem_unlink(START_SEM);
	sem_unlink(PRINT_SEM);
	sem_unlink(DIET_SEM);
	sim->forks = sem_open(FORKS_SEM, mode, 000644, sim->settings[N_PHILO]);
	sim->start = sem_open(START_SEM, mode, 000644, 0);
	sim->print = sem_open(PRINT_SEM, mode, 000644, 1);
	sim->enough = sem_open(DIET_SEM, mode, 000644, 0);
	err |= sim->forks == SEM_FAILED;
	err |= sim->start == SEM_FAILED;
	err |= sim->print == SEM_FAILED;
	err |= sim->enough == SEM_FAILED;
	return (err == false);
}
