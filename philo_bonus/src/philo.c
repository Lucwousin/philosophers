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

static void	cleanup(t_sim *sim)
{

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

}