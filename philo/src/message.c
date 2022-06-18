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
#include <string.h>
#include <unistd.h>

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

static bool	print_queue(t_sim *sim, t_msg_queue *queue, uint32_t count)
{
	uint32_t	i;
	t_philo		*philo;
	t_msg		msg;

	i = 0;
	while (i < count)
	{
		philo = sim->philos + queue->ids[1][i];
		msg = queue->msgs[1][i];
		printf(FORMAT_MSG, queue->times[1][i], philo->id + 1, g_msgs[msg]);
		if (msg == DIE || msg == END)
			return (false);
		++i;
	}
	return (true);
}

static void	swap_queue(t_msg_queue *queue)
{
	uint32_t	*ids;
	uint32_t	*times;
	t_msg		*msgs;

	ids = queue->ids[0];
	times = queue->times[0];
	msgs = queue->msgs[0];
	queue->ids[0] = queue->ids[1];
	queue->times[0] = queue->times[1];
	queue->msgs[0] = queue->msgs[1];
	queue->ids[1] = ids;
	queue->times[1] = times;
	queue->msgs[1] = msgs;
}

void	*listen_messages(void *arg)
{
	t_sim		*sim;
	t_msg_queue	*queue;
	uint32_t	count;

	sim = arg;
	queue = &sim->msg_queue;
	while (true)
	{
		pthread_mutex_lock(&queue->msg_mutex);
		swap_queue(queue);
		count = queue->count;
		queue->count = 0;
		pthread_mutex_unlock(&queue->msg_mutex);
		if (!print_queue(sim, queue, count))
			break ;
		usleep(500);
	}
	return (NULL);
}

void	queue_message(t_philo *philo, t_msg msg, uint32_t timestamp)
{
	t_msg_queue	*queue;

	queue = &philo->sim->msg_queue;
	pthread_mutex_lock(&queue->msg_mutex);
	if (queue->count == philo->sim->settings[N_PHILO] * 4)
	{
		printf("Unable to queue message, queue is full!\n");
		pthread_mutex_unlock(&queue->msg_mutex);
		return ;
	}
	queue->ids[0][queue->count] = philo->id;
	queue->msgs[0][queue->count] = msg;
	queue->times[0][queue->count] = timestamp;
	queue->count++;
	pthread_mutex_unlock(&queue->msg_mutex);
}
