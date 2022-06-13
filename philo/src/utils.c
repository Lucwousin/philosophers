/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/02 15:07:18 by lsinke        #+#    #+#                 */
/*   Updated: 2022/06/02 15:07:18 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

uint64_t	get_time(void)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	return (t.tv_sec * 1000 + t.tv_usec / 1000);
}

void	*ft_calloc(size_t count, size_t size)
{
	void	*p;
	size_t	total;

	if (count == 0 || size == 0)
	{
		count = 1;
		size = 1;
	}
	total = count * size;
	p = malloc(total);
	if (p)
		memset(p, 0x00, total);
	return (p);
}

void	*ft_memcpy(void *dst, const void *src, size_t len)
{
	char		*dp;
	const char	*sp;

	if (!dst && !src)
		return (NULL);
	dp = dst;
	sp = src;
	while (len > 0)
	{
		*dp = *sp;
		++dp;
		++sp;
		--len;
	}
	return (dst);
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
				usleep(750);
			return ;
		}
		usleep((dif_time * 2 * 1000) / 3);
	}
}

bool	check_stopped(t_sim *sim)
{
	bool	result;

	pthread_mutex_lock(&sim->run_sim);
	result = sim->stopped;
	pthread_mutex_unlock(&sim->run_sim);
	return (result);
}
