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


static void	go_eat(t_philo *philo)
{
	pthread_mutex_lock(philo->forks[0]);
	print_message(philo, FORK);
	pthread_mutex_lock(philo->forks[1]);
	print_message(philo, FORK);
	pthread_mutex_lock(&philo->eat_m);
	print_message(philo, EAT);
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
	print_message(philo, SLEEP);
	smart_sleep(philo->state->settings[T_SLEEP]);
}

static void	philo_action(t_philo *philo, t_status status)
{
	if (status == THINKING)
		print_message(philo, THINK);
	else if (status == EATING)
		go_eat(philo);
	else if (status == SLEEPING)
		go_sleep(philo);
}

static bool	check_stopped(t_state *state)
{
	bool	result;

	pthread_mutex_lock(&state->run_sim);
	result = state->stopped;
	pthread_mutex_unlock(&state->run_sim);
	return (result);
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
		philo_action(philo, status);
		status = (status + 1) % NO_STATUS;
	}
	return (NULL);
}