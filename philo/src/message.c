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
#include <stdlib.h>
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

static bool	print_queue(t_sim *sim, t_msg_queue *queue)
{
	uint32_t	i;
	t_philo		*philo;
	t_msg		msg;

	i = 0;
	while (i < queue->count)
	{
		philo = sim->philos + queue->ids[i];
		msg = queue->msgs[i];
		printf(FORMAT_MSG, queue->times[i], philo->id + 1, g_msgs[msg]);
		if (msg == DIE || msg == END)
			return (false);
		++i;
	}
	return (true);
}

static bool	copy_queue(t_msg_queue *queue, t_msg_queue *copy)
{
	if (copy->count < queue->count)
	{
		free(copy->ids);
		free(copy->msgs);
		free(copy->times);
		copy->ids = malloc(queue->count * sizeof(uint32_t));
		copy->msgs = malloc(queue->count * sizeof(t_msg));
		copy->times = malloc(queue->count * sizeof(uint32_t));
		if (copy->ids == NULL || copy->msgs == NULL || copy->times == NULL)
		{
			printf("Failed to allocate messages! Message printer will exit\n");
			pthread_mutex_unlock(&queue->msg_mutex);
			return (false);
		}
	}
	copy->count = queue->count;
	ft_memcpy(copy->ids, queue->ids, queue->count * sizeof(uint32_t));
	ft_memcpy(copy->msgs, queue->msgs, queue->count * sizeof(t_msg));
	ft_memcpy(copy->times, queue->times, queue->count * sizeof(uint32_t));
	return (true);
}

void	*listen_messages(void *arg)
{
	t_sim		*sim;
	t_msg_queue	*queue;
	t_msg_queue	copy;

	sim = arg;
	queue = &sim->msg_queue;
	memset(&copy, 0, sizeof(t_msg_queue));
	while (true)
	{
		pthread_mutex_lock(&queue->msg_mutex);
		if (!copy_queue(queue, &copy))
			break ;
		queue->count = 0;
		pthread_mutex_unlock(&queue->msg_mutex);
		if (!print_queue(sim, &copy))
			break ;
		usleep(500);
	}
	free(copy.ids);
	free(copy.msgs);
	free(copy.times);
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
	queue->ids[queue->count] = philo->id;
	queue->msgs[queue->count] = msg;
	queue->times[queue->count] = timestamp;
	queue->count++;
	pthread_mutex_unlock(&queue->msg_mutex);
}
