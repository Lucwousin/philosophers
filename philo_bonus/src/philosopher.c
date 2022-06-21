/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philosopher.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/16 17:05:55 by lsinke        #+#    #+#                 */
/*   Updated: 2022/06/16 17:05:55 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <fcntl.h>
#include <stdlib.h>

static void	init_philo(t_philo *this, uint32_t id, t_sim *sim)
{
	char	sem_name[16];

	this->id = id + 1;
	this->sim = sim;
	this->last_eaten = sim->start_time;
	this->times_eaten = 0;
	get_sem_name(this->id, sem_name);
	sem_unlink(sem_name);
	this->semaphore = sem_open(sem_name, O_CREAT | O_EXCL, 000644, 1);
	if (this->semaphore == SEM_FAILED)
		exit(EXIT_SEMAPHORE);
	if (sim->settings[N_PHILO] % 2)
		this->next_long_think = (sim->settings[N_PHILO] / 2) - (this->id / 2);
}

static void	*reaper(void *arg)
{
	t_philo		*p;
	uint64_t	time;
	uint64_t	last_eaten;

	p = arg;
	smart_sleep(p->sim->start_time + p->sim->settings[T_DIE] - get_time() - 1);
	while (true)
	{
		sem_wait(p->semaphore);
		time = get_time();
		last_eaten = p->last_eaten;
		if (time - last_eaten >= p->sim->settings[T_DIE])
			break ;
		sem_post(p->semaphore);
		smart_sleep(last_eaten + p->sim->settings[T_DIE] - time);
	}
	send_message(p->sim, p->id, DIE, time);
	free(p->sim->p_pids);
	exit(EXIT_DEATH);
}

_Noreturn static void	philo_loop(t_philo *this)
{
	t_status	status;

	status = EATING;
	while (true)
	{
		if (status == THINKING)
			go_think(this);
		else if (status == EATING)
			go_eat(this);
		else if (status == SLEEPING)
			go_sleep(this);
		status = (status + 1) % NO_STATUS;
	}
}

void	philosopher(uint32_t id, t_sim *sim)
{
	t_philo		this;

	init_philo(&this, id, sim);
	if (!create_and_detach(reaper, &this))
		exit(EXIT_THREAD);
	smart_sleep(sim->start_time - get_time());
	go_think(&this);
	if (this.id % 2 == 0)
		smart_sleep(sim->settings[T_EAT] / 2);
	philo_loop(&this);
}

const char	*get_philo_err(uint8_t exit_code)
{
	if (exit_code == EXIT_SEMAPHORE)
		return (P_SEM_ERR_MES);
	if (exit_code == EXIT_THREAD)
		return (P_THD_ERR_MES);
	return ("Exit code is not an error! (errorception)");
}
