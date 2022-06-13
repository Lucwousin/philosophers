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

static void	go_eat(t_philo *philo)
{
	pthread_mutex_lock(philo->forks[0]);
	queue_message(philo, FORK, get_time() - philo->sim->start_time);
	pthread_mutex_lock(philo->forks[1]);
	queue_message(philo, FORK, get_time() - philo->sim->start_time);
	pthread_mutex_lock(&philo->eat_m);
	queue_message(philo, EAT, get_time() - philo->sim->start_time);
	philo->last_eaten = get_time();
	pthread_mutex_unlock(&philo->eat_m);
	smart_sleep(philo->sim->settings[T_EAT]);
	pthread_mutex_lock(&philo->eat_m);
	++philo->times_ate;
	pthread_mutex_unlock(&philo->eat_m);
	pthread_mutex_unlock(philo->forks[0]);
	pthread_mutex_unlock(philo->forks[1]);
}

static void	go_sleep(t_philo *philo)
{
	queue_message(philo, SLEEP, get_time() - philo->sim->start_time);
	smart_sleep(philo->sim->settings[T_SLEEP]);
}

void	*philo_thread(void *arg)
{
	t_philo		*philo;
	t_status	status;
	bool		running;

	philo = (t_philo *) arg;
	status = THINKING;
	running = true;
	pthread_mutex_lock(&philo->sim->run_sim);
	philo->last_eaten = philo->sim->start_time;
	pthread_mutex_unlock(&philo->sim->run_sim);
	if (philo->id % 2)
		usleep(1000);
	while (running)
	{
		if (status == THINKING)
			queue_message(philo, THINK, get_time() - philo->sim->start_time);
		else if (status == EATING)
			go_eat(philo);
		else if (status == SLEEPING)
			go_sleep(philo);
		status = (status + 1) % NO_STATUS;
		running = (check_stopped(philo->sim) == false);
	}
	return (NULL);
}
