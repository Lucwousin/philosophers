/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philosopher.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/03 11:58:27 by lsinke        #+#    #+#                 */
/*   Updated: 2022/06/03 11:58:27 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>
#include <printf.h>

static void	get_forks(t_mutex *forks[2], t_philo *philo)
{
	pthread_mutex_lock(forks[0]);
	send_message(philo, FORK);
	pthread_mutex_lock(forks[1]);
	send_message(philo, FORK);
}

static void	smart_sleep(uint64_t duration)
{
	uint64_t	cur_time;
	uint64_t	end_time;
	uint64_t	diff;

	end_time = get_time() + duration;
	while (true)
	{
		cur_time = get_time();
		if (cur_time >= end_time)
			return ;
		diff = end_time - cur_time;
		if (diff > 2)
		{
			usleep((diff / 2) * 1000);
			continue ;
		}
		while (get_time() < end_time)
			continue ;
		return ;
	}
}

static void	eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->eat_m);
	send_message(philo, EAT);
	philo->last_eaten = get_time();
	pthread_mutex_unlock(&philo->eat_m);
	smart_sleep(philo->state->settings[T_EAT]);
	pthread_mutex_lock(&philo->eat_m);
	++philo->times_ate;
	pthread_mutex_unlock(&philo->eat_m);
}

static void	*return_forks(t_mutex *forks[2])
{
	pthread_mutex_unlock(forks[0]);
	pthread_mutex_unlock(forks[1]);
	return ((void *) 0);
}

static void	go_sleep(t_philo *philo)
{
	send_message(philo, SLEEP);
	smart_sleep(philo->state->settings[T_SLEEP]);
}

void	*unlock(t_mutex *mutex)
{
	pthread_mutex_unlock(mutex);
	return (void *) 0;
}

static void	assign_forks(t_mutex *forks[2], uint32_t id, t_state *state)
{
	uint32_t	l;
	uint32_t	r;

	l = id;
	r = (id + 1) % state->settings[N_PHILO];
	if (id % 2)
	{
		forks[0] = state->forks + l;
		forks[1] = state->forks + r;
	}
	else
	{
		forks[0] = state->forks + r;
		forks[1] = state->forks + l;
	}
}

void	*philosopher(void *arg)
{
	t_philo	*philo;
	t_mutex	*forks[2];

	philo = (t_philo *) arg;
	assign_forks(forks, philo->id, philo->state);
	pthread_mutex_lock(&philo->state->run_sim);
	pthread_mutex_unlock(&philo->state->run_sim);
	philo->last_eaten = philo->state->start_time;
	while (true)
	{
		send_message(philo, THINK);
		get_forks(forks, philo);
		eat(philo);
		return_forks(forks);
		go_sleep(philo);
	}
}