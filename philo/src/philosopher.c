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

static void	try_message(t_philo *philo, t_msg msg)
{
	if (check_stopped(philo->state))
		return ;
	print_message(philo, msg);
}

static void	go_eat(t_philo *philo)
{
	pthread_mutex_lock(philo->forks[0]);
	try_message(philo, FORK);
	pthread_mutex_lock(philo->forks[1]);
	try_message(philo, FORK);
	pthread_mutex_lock(&philo->eat_m);
	try_message(philo, EAT);
	philo->last_eaten = get_time();
	pthread_mutex_unlock(&philo->eat_m);
	smart_sleep(philo->state->settings[T_EAT]);
	pthread_mutex_lock(&philo->eat_m);
	++philo->times_ate;
	pthread_mutex_unlock(&philo->eat_m);
	pthread_mutex_unlock(philo->forks[0]);
	pthread_mutex_unlock(philo->forks[1]);
}

static void	go_sleep(t_philo *philo)
{
	try_message(philo, SLEEP);
	smart_sleep(philo->state->settings[T_SLEEP]);
}

void	*philo_thread(void *arg)
{
	t_philo		*philo;
	t_status	status;

	philo = (t_philo *) arg;
	status = THINKING;
	pthread_mutex_lock(&philo->state->run_sim);
	philo->last_eaten = philo->state->start_time;
	pthread_mutex_unlock(&philo->state->run_sim);
	if (philo->id % 2)
		usleep(500);
	while (check_stopped(philo->state) == false)
	{
		if (status == THINKING)
			try_message(philo, THINK);
		else if (status == EATING)
			go_eat(philo);
		else if (status == SLEEPING)
			go_sleep(philo);
		status = (status + 1) % NO_STATUS;
	}
	return (NULL);
}
