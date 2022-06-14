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
#include <stdlib.h>
#include <stdio.h>

static char	*get_sem_name(uint32_t id, char name[16])
{
	int32_t	i;
	bool	done;

	i = 0;
	while (i < 7)
	{
		name[i] = "/philo_"[i];
		++i;
	}
	done = false;
	while (!done)
	{
		name[i++] = (char)(id % 10u + '0');
		done = (id % 10u) == 0;
		id /= 10;
	}
	name[i] = '\0';
	return (name);
}

static void	cleanup(t_sim *sim)
{
	for (uint32_t i = 0; i < sim->settings[N_PHILO]; i++)
	{
		char name[16];
		get_sem_name(i, name);
		sem_unlink(name);
	}
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
	int32_t	ret;

	while (wait(&status) == -1)
		;
	if (WIFEXITED(status))
		ret = WEXITSTATUS(status);
	else
		ret = -69;
	if (ret != -69)
		kill_all_children(sim);
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