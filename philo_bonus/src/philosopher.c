#include "philo.h"
#include <stdlib.h>

static void	init_philo(t_philo *this, uint32_t id, t_sim *sim)
{
	this->id = id + 1;
	get_sem_name(this->id, this->sem_name);
	sem_unlink(this->sem_name);
	this->semaphore = sem_open(this->sem_name, O_CREAT | O_EXCL, 000644, 1);
	if (this->semaphore == SEM_FAILED)
		exit(SEM_FAILURE);
	this->sim = sim;
	this->times_eaten = 0;
}

static void *reaper(void *arg)
{
	t_philo		*p;
	uint64_t	time;

	p = arg;
	while (true)
	{
		usleep(1000);
		sem_wait(p->semaphore);
		time = get_time();
		if (time - p->last_eaten >= p->sim->settings[T_DIE])
			break ;
		sem_post(p->semaphore);
	}
	sem_close(p->semaphore);
	sem_unlink(p->sem_name);
	send_message(p->sim, p->id, DIE, time);
	exit(EXIT_DEATH);
}

_Noreturn static void	philo_loop(t_philo *this)
{
	t_status	status;

	status = THINKING;
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
	t_philo	this;

	init_philo(&this, id, sim);
	sem_wait(this.sim->start);
	this.last_eaten = this.sim->start_time;
	sem_post(this.sim->start);
	create_and_detach(reaper, &this);
	if (id % 2)
		usleep(1000);
	philo_loop(&this);
}
