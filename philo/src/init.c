/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/02 14:45:14 by lsinke        #+#    #+#                 */
/*   Updated: 2022/06/02 14:45:14 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	create_philosopher(t_philo *p, t_sim *sim, uint32_t id)
{
	p->id = id;
	p->times_ate = 0;
	p->sim = sim;
	if (pthread_mutex_init(&p->eat_m, NULL) != 0)
		return (false);
	p->forks[0] = sim->forks + p->id;
	p->forks[1] = sim->forks + ((p->id + 1) % sim->settings[N_PHILO]);
	return (true);
}

bool	init_philosophers(t_sim *sim)
{
	uint32_t	i;

	i = 0;
	while (i < sim->settings[N_PHILO])
	{
		if (!create_philosopher(sim->philos + i, sim, i))
			return (false);
		++i;
	}
	return (true);
}

bool	init_mutexes(t_sim *sim)
{
	uint32_t		i;

	i = 0;
	while (i < sim->settings[N_PHILO])
	{
		if (pthread_mutex_init(sim->forks + i, NULL) != 0)
			return (false);
		++i;
	}
	if (pthread_mutex_init(&sim->msg_queue.msg_mutex, NULL) != 0 || \
		pthread_mutex_init(&sim->run_sim, NULL) != 0)
		return (false);
	return (true);
}

bool	init_allocated_memory(t_sim *sim)
{
	uint32_t	num_philos;
	t_msg_queue	*queue;

	num_philos = sim->settings[N_PHILO];
	queue = &sim->msg_queue;
	sim->philos = ft_calloc(num_philos, sizeof(t_philo));
	sim->forks = ft_calloc(num_philos, sizeof(t_mutex));
	queue->msgs[0] = ft_calloc(num_philos * 4, sizeof(t_msg));
	queue->ids[0] = ft_calloc(num_philos * 4, sizeof(uint32_t));
	queue->times[0] = ft_calloc(num_philos * 4, sizeof(uint32_t));
	queue->msgs[1] = ft_calloc(num_philos * 4, sizeof(t_msg));
	queue->ids[1] = ft_calloc(num_philos * 4, sizeof(uint32_t));
	queue->times[1] = ft_calloc(num_philos * 4, sizeof(uint32_t));
	queue->count = 0;
	return (sim->philos != NULL && sim->forks != NULL && \
		queue->msgs[0] != NULL && queue->msgs[1] != NULL && \
		queue->ids[0] != NULL && queue->ids[1] != NULL && \
		queue->times[0] != NULL && queue->times[1] != NULL);
}
