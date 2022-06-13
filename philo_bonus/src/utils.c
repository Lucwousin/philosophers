#include "philo.h"
#include <signal.h>

void	kill_all_children(t_sim *sim)
{
	uint32_t	i;

	i = 0;
	while (i < sim->settings[N_PHILO])
		kill(sim->philos[i++], SIGKILL);
}