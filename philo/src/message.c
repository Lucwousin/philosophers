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
#include <unistd.h>
#include <stdlib.h>
#include <libc.h>

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

static bool	print_queue(t_state *state, t_msg_queue *queue)
{
	uint32_t	i;
	t_philo		*philo;
	t_msg		msg;

	i = 0;
	while (i < queue->count)
	{
		philo = state->philos + queue->ids[i];
		msg = queue->msgs[i];
		printf(FORMAT_MSG, queue->timestamps[i], philo->id + 1, g_msgs[msg]);
		if (msg == DIE || msg == END)
			return (false);
		++i;
	}
	return (true);
}

static bool	copy_queue(t_msg_queue *queue, t_msg_queue *copy)
{
	copy->count = queue->count;
	free(copy->ids);
	free(copy->msgs);
	free(copy->timestamps);
	copy->ids = malloc(queue->count * sizeof(uint32_t));
	copy->msgs = malloc(queue->count * sizeof(t_msg));
	copy->timestamps = malloc(queue->count * sizeof(uint32_t));
	if (copy->ids == NULL || copy->msgs == NULL || copy->timestamps == NULL)
	{
		printf("Failed to allocate messages! Message printer will exit\n");
		pthread_mutex_unlock(&queue->msg_mutex);
		return (false);
	}
	ft_memcpy(copy->ids, queue->ids, queue->count * sizeof(uint32_t));
	ft_memcpy(copy->msgs, queue->msgs, queue->count * sizeof(t_msg));
	ft_memcpy(copy->timestamps, queue->timestamps, queue->count * sizeof(uint32_t));
	return (true);
}

void	*listen_messages(void *arg)
{
	t_state		*state;
	t_msg_queue	*queue;
	t_msg_queue	copy;

	state = arg;
	queue = &state->msg_queue;
	memset(&copy, 0, sizeof(t_msg_queue));
	pthread_mutex_lock(&queue->msg_mutex);
	queue->count = 0;
	pthread_mutex_unlock(&queue->msg_mutex);
	while (true)
	{
		pthread_mutex_lock(&queue->msg_mutex);
		if (!copy_queue(queue, &copy))
			break ;
		queue->count = 0;
		pthread_mutex_unlock(&queue->msg_mutex);
		if (!print_queue(state, &copy))
			break ;
		usleep(500);
	}
	free(copy.ids);
	free(copy.msgs);
	free(copy.timestamps);
	return (NULL);
}

void	queue_message(t_philo *philo, t_msg msg, uint32_t timestamp)
{
	t_msg_queue	*queue;

	queue = &philo->state->msg_queue;
	pthread_mutex_lock(&queue->msg_mutex);
	if (queue->count == philo->state->settings[N_PHILO] * 4)
	{
		printf("Unable to queue message, queue is full!\n");
		pthread_mutex_unlock(&queue->msg_mutex);
		return ;
	}
	queue->ids[queue->count] = philo->id;
	queue->msgs[queue->count] = msg;
	queue->timestamps[queue->count] = timestamp;
	queue->count++;
	pthread_mutex_unlock(&queue->msg_mutex);
}
