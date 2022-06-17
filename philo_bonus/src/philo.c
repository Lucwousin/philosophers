/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/13 14:27:09 by lsinke        #+#    #+#                 */
/*   Updated: 2022/06/13 14:27:09 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

static void	cleanup(t_sim *sim)
{
	char		name[16];
	uint32_t	id;

	id = 0;
	while (id++ < sim->settings[N_PHILO])
	{
		get_sem_name(id, name);
		sem_unlink(name);
	}
	sim->done = true;
	sem_post(sim->enough);
	sem_close(sim->forks);
	sem_close(sim->start);
	sem_close(sim->print);
	sem_close(sim->enough);
	sem_unlink(FORKS_SEM);
	sem_unlink(START_SEM);
	sem_unlink(PRINT_SEM);
	sem_unlink(DIET_SEM);
}

static int	exit_msg(t_sim *sim, const char *msg)
{
	if (sim != NULL)
		cleanup(sim);
	printf("Error: %s\n", msg);
	return (EXIT_FAILURE);
}

static void	wait_children(t_sim *sim)
{
	int32_t	status;
	uint8_t	ret;

	while (wait(&status) == -1)
		;
	if (WIFEXITED(status))
	{
		ret = WEXITSTATUS(status);
		kill_all_children(sim);
		if (ret != EXIT_DEATH)
			exit_msg(sim, get_philo_err(ret));
	}
}

int	main(int argc, char **argv)
{
	t_sim	sim;

	if (argc < 5 || argc > 6)
		return (exit_msg(NULL, USAGE_MES));
	if (!parse_args(argc, argv, sim.settings))
		return (exit_msg(NULL, ARGS_MES));
	if (!validate_philo_count(sim.settings))
		return (exit_msg(NULL, PHILO_N_MES));
	if (!alloc_pid_arr(&sim))
		return (exit_msg(&sim, ALLOC_MES));
	if (!init_semaphores(&sim))
		return (exit_msg(&sim, SEM_MES));
	if (!simulate(&sim))
		return (exit_msg(&sim, SIM_MES));
	wait_children(&sim);
	cleanup(&sim);
}
