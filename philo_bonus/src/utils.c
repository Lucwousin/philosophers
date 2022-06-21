/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/16 17:06:13 by lsinke        #+#    #+#                 */
/*   Updated: 2022/06/16 17:06:13 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <pthread.h>
#include <signal.h>
#include <sys/time.h>

void	kill_all_children(t_sim *sim)
{
	uint32_t	i;

	i = 0;
	while (i < sim->settings[N_PHILO])
		kill(sim->p_pids[i++], SIGKILL);
}

bool	create_and_detach(void *(*routine)(void *), void *arg)
{
	pthread_t	thread;

	if (pthread_create(&thread, NULL, routine, arg) != 0)
		return (false);
	if (pthread_detach(thread) != 0)
		return (false);
	return (true);
}

uint64_t	get_time(void)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	return (t.tv_sec * 1000 + t.tv_usec / 1000);
}

void	smart_sleep(uint64_t duration)
{
	uint64_t	end_time;
	uint64_t	cur_time;
	uint64_t	dif_time;

	end_time = get_time() + duration;
	while (true)
	{
		cur_time = get_time();
		if (cur_time >= end_time)
			return ;
		dif_time = end_time - cur_time;
		if (dif_time <= 1)
		{
			while (get_time() < end_time)
				usleep(200);
			return ;
		}
		usleep((dif_time * 2 * 1000) / 3);
	}
}

char	*get_sem_name(uint32_t id, char name[16])
{
	int32_t	i;

	i = 0;
	while (i < 7)
	{
		name[i] = "/philo_"[i];
		++i;
	}
	while (id != 0)
	{
		name[i++] = (char)(id % 10 + '0');
		id /= 10;
	}
	name[i] = '\0';
	return (name);
}
