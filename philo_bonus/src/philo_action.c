#include "philo.h"

void	go_think(t_philo *philo)
{
	send_message(philo->sim, philo->id, THINK, get_time());
}

void	go_eat(t_philo *philo)
{
	sem_wait(philo->sim->forks);
	send_message(philo->sim, philo->id, FORK, get_time());
	sem_wait(philo->sim->forks);
	send_message(philo->sim, philo->id, FORK, get_time());
	sem_wait(philo->semaphore);
	philo->last_eaten = get_time();
	send_message(philo->sim, philo->id, EAT, philo->last_eaten);
	sem_post(philo->semaphore);
	smart_sleep(philo->sim->settings[T_EAT]);
	sem_wait(philo->semaphore);
	if (++philo->times_eaten == philo->sim->settings[N_EAT])
		sem_post(philo->sim->enough);
	sem_post(philo->semaphore);
	sem_post(philo->sim->forks);
	sem_post(philo->sim->forks);
}

void	go_sleep(t_philo *philo)
{
	send_message(philo->sim, philo->id, SLEEP, get_time());
	smart_sleep(philo->sim->settings[T_SLEEP]);
}
