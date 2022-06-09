/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   message.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/03 14:49:15 by lsinke        #+#    #+#                 */
/*   Updated: 2022/06/03 14:49:15 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>
#include <inttypes.h>
#ifdef DEBUG
# define FORMAT_MSG "%llu %u %s\n"
#else
# define FORMAT_MSG "%8llums - %3u %s\n"
#endif

static const char	*g_msgs[] = {
[FORK] = "has taken a fork",
[EAT] = "is eating",
[SLEEP] = "is sleeping",
[THINK] = "is thinking",
[DIE] = "died"
};

static bool	should_print(t_state *state, t_msg msg)
{
	if (msg == DIE)
		return (true);
	else
		return (check_stopped(state) == false);
}

void	print_message(t_philo *philo, t_msg msg)
{
	uint64_t	timestamp;
	bool		print;

	pthread_mutex_lock(&philo->state->print_m);
	print = should_print(philo->state, msg);
	if (print)
	{
		timestamp = get_time() - philo->state->start_time;
		printf(FORMAT_MSG, timestamp, philo->id + 1, g_msgs[msg]);
	}
	pthread_mutex_unlock(&philo->state->print_m);
}
