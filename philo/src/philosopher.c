/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo_thread.c                                      :+:    :+:            */
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
	print_message(philo, FORK);
	pthread_mutex_lock(forks[1]);
	print_message(philo, FORK);
}

static void	eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->eat_m);
	print_message(philo, EAT);
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
	print_message(philo, SLEEP);
	smart_sleep(philo->state->settings[T_SLEEP]);
}

static void	assign_forks(t_mutex *forks[2], uint32_t id, t_state *state)
{
	uint32_t	l;
	uint32_t	r;

	l = id;
	r = (id + 1) % state->settings[N_PHILO];
	forks[0] = state->forks + l;
	forks[1] = state->forks + r;
}

void	*philo_thread(void *arg)
{
	t_philo	*philo;
	t_mutex	*forks[2];

	philo = (t_philo *) arg;
	assign_forks(forks, philo->id, philo->state);
	pthread_mutex_lock(&philo->state->run_sim);
	pthread_mutex_unlock(&philo->state->run_sim);
	philo->last_eaten = philo->state->start_time;
	if (philo->id % 2)
		usleep(150);
	while (true)
	{
		print_message(philo, THINK);
		get_forks(forks, philo);
		eat(philo);
		return_forks(forks);
		go_sleep(philo);
		pthread_mutex_lock(&philo->state->run_sim);
		if (philo->state->stopped)
			break ;
		pthread_mutex_unlock(&philo->state->run_sim);
	}
	pthread_mutex_unlock(&philo->state->run_sim);
	return (NULL);
}