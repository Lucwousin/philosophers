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

static const char	*g_msgs[] = {
[FORK] = "has taken a fork",
[EAT] = "is eating",
[SLEEP] = "is sleeping",
[THINK] = "is thinking",
[DIE] = "died"
};

void	send_message(t_philo *philo, t_msg msg)
{
	static bool	done = false;
	uint64_t	timestamp;
	t_mutex		*mutex;

	mutex = &philo->state->print_m;
	pthread_mutex_lock(mutex);
	if (done)
	{
		pthread_mutex_unlock(mutex);
		pthread_exit((NULL));
	}
	timestamp = get_time() - philo->state->start_time;
#ifdef DEBUG
	printf("%" PRIu64 " %u %s\n", timestamp, philo->id + 1, g_msgs[msg]);
#else
	printf("%-8" PRIu64 "ms - %3u %s\n", timestamp, philo->id + 1, g_msgs[msg]);
#endif
	done = msg == DIE;
	pthread_mutex_unlock(mutex);
}