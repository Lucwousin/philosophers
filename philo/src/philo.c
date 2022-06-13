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

static void	cleanup(t_state *state)
{
	uint32_t	i;

	i = 0;
	while (i < state->settings[N_PHILO])
	{
		if (state->forks)
			pthread_mutex_destroy(state->forks + i);
		if (state->philos)
			pthread_mutex_destroy(&state->philos[i].eat_m);
		++i;
	}
	if (state->forks)
	{
		pthread_mutex_destroy(&state->msg_queue.msg_mutex);
		pthread_mutex_destroy(&state->run_sim);
	}
	free(state->forks);
	free(state->philos);
	free(state->msg_queue.ids);
	free(state->msg_queue.msgs);
	free(state->msg_queue.times);
}

static int	exit_msg(t_state *state, const char *msg)
{
	if (state != NULL)
		cleanup(state);
	printf("Error: %s\n", msg);
	return (EXIT_FAILURE);
}

int	main(int argc, char **argv)
{
	t_state	state;

	if (argc < 5 || argc > 6)
		return (exit_msg(NULL, USAGE_MES));
	if (!parse_args(argc, argv, state.settings))
		return (exit_msg(NULL, ARGS_MES));
	if (!validate_philo_count(state.settings))
		return (exit_msg(NULL, PHILO_N_MES));
	if (!allocate_arrays(&state))
		return (exit_msg(&state, ALLOC_MES));
	if (!init_mutexes(&state))
		return (exit_msg(&state, MUTEX_MES));
	if (!init_philosophers(&state))
		return (exit_msg(&state, PHILO_MES));
	if (!simulate(&state))
		return (exit_msg(&state, THREAD_MES));
	cleanup(&state);
	exit(EXIT_SUCCESS);
}
