/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   message.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/16 17:05:28 by lsinke        #+#    #+#                 */
/*   Updated: 2022/06/16 17:05:28 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>

#ifdef DEBUG
# define FORMAT_MSG "%u %u %s\n"
#else
# define FORMAT_MSG "%8ums - %3u %s\n"
#endif

static const char	*g_msgs[] = {
[FORK] = "has taken a fork",
[EAT] = "is eating",
[SLEEP] = "is sleeping",
[THINK] = "is thinking",
[DIE] = "died",
[END] = "philosophers ate enough - simulation ended"
};

void	send_message(t_sim *sim, uint32_t id, t_msg msg, uint64_t time)
{
	sem_wait(sim->print);
	printf(FORMAT_MSG, (uint32_t)(time - sim->start_time), id, g_msgs[msg]);
	if (msg != DIE && msg != END)
		sem_post(sim->print);
}
