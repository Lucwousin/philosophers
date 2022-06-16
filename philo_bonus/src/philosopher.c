#include "philo.h"
#include <stdlib.h>

static void	init_philo(t_philo *this, uint32_t id, t_sim *sim)
{
	char	sem_name[16];

	this->id = id + 1;
	get_sem_name(this->id, sem_name);
	sem_unlink(sem_name);
	this->semaphore = sem_open(sem_name, O_CREAT | O_EXCL, 000644, 1);
	if (this->semaphore == SEM_FAILED)
		exit(EXIT_SEMAPHORE);
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
	if (!create_and_detach(reaper, &this))
		exit(EXIT_THREAD);
	if (id % 2)
		usleep(1000);
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
