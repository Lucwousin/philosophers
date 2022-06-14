#include <stdio.h>
#include "philo.h"

#ifdef DEBUG
# define FORMAT_MSG "%u %u %s\n"
#else
# define FORMAT_MSG "%8ums - %3u %s\n"
#endif

static const char	*g_msgs[] = {
[FORK] = "has taken a fork",
[EAT] = "is eating",
[SLEEP] = "is sleeping",
[THINK] = "is thinking",
[DIE] = "died",
[END] = "philosophers ate enough - simulation ended"
};

void	send_message(uint32_t id, t_msg msg, uint32_t time)
{
	printf(FORMAT_MSG, time, id, g_msgs[msg]);
}
