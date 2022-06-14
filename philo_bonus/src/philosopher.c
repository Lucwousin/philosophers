#include "philo.h"
#include <stdlib.h>
#include <printf.h>
#include <errno.h>

static char	*get_sem_name(uint32_t id, char name[16])
{
	int32_t	i;
	bool	done;

	i = 0;
	while (i < 7)
	{
		name[i] = "/philo_"[i];
		++i;
	}
	done = false;
	while (!done)
	{
		name[i++] = (char)(id % 10u + '0');
		done = (id % 10u) == 0;
		id /= 10;
	}
	name[i] = '\0';
	return (name);
}

static void	init_philo(t_philo *this, uint32_t id, t_sim *sim)
{
	this->id = id;
	get_sem_name(id, this->sem_name);
	sem_unlink(this->sem_name);
	this->semaphore = sem_open(this->sem_name, O_CREAT, 000644, 1);
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
		{
			sem_close(p->semaphore);
			sem_unlink(p->sem_name);
			send_message(p->id, DIE, time - p->sim->start_time);
			exit(EXIT_DEATH);
		}
		sem_post(p->semaphore);
	}
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
