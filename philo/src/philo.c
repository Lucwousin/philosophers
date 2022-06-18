/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/02 13:36:14 by lsinke        #+#    #+#                 */
/*   Updated: 2022/06/02 13:36:14 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>
#include <stdlib.h>

static void	cleanup(t_sim *sim)
{
	uint32_t	i;

	i = 0;
	while (i < sim->settings[N_PHILO])
	{
		if (sim->forks)
			pthread_mutex_destroy(sim->forks + i);
		if (sim->philos)
			pthread_mutex_destroy(&sim->philos[i].eat_m);
		++i;
	}
	if (sim->forks)
	{
		pthread_mutex_destroy(&sim->msg_queue.msg_mutex);
		pthread_mutex_destroy(&sim->run_sim);
	}
	free(sim->forks);
	free(sim->philos);
	free(sim->msg_queue.ids[0]);
	free(sim->msg_queue.msgs[0]);
	free(sim->msg_queue.times[0]);
	free(sim->msg_queue.ids[1]);
	free(sim->msg_queue.msgs[1]);
	free(sim->msg_queue.times[1]);
}

static int	exit_msg(t_sim *sim, const char *msg)
{
	if (sim != NULL)
		cleanup(sim);
	printf("Error: %s\n", msg);
	return (EXIT_FAILURE);
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
	if (!init_allocated_memory(&sim))
		return (exit_msg(&sim, ALLOC_MES));
	if (!init_mutexes(&sim))
		return (exit_msg(&sim, MUTEX_MES));
	if (!init_philosophers(&sim))
		return (exit_msg(&sim, PHILO_MES));
	if (!simulate(&sim))
		return (exit_msg(&sim, THREAD_MES));
	cleanup(&sim);
	exit(EXIT_SUCCESS);
}
