/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/02 14:07:40 by lsinke        #+#    #+#                 */
/*   Updated: 2022/06/02 14:07:40 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	is_digit(char c)
{
	return (c >= '0' && c <= '9');
}

static bool	parse_arg(char *arg, uint32_t *setting)
{
	uint32_t	value;
	char		c;

	value = 0;
	while (*arg)
	{
		c = *arg++;
		if (!is_digit(c) || value > UINT32_MAX / 10)
			return (false);
		value *= 10;
		value += c - '0';
	}
	*setting = value;
	return (true);
}

bool	parse_args(int argc, char **argv, uint32_t settings[5])
{
	int32_t	i;

	i = 0;
	while (i < argc - 1)
	{
		if (!parse_arg(argv[i + 1], settings + i))
			return (false);
		++i;
	}
	if (argc != 6)
		settings[N_EAT] = UINT32_MAX;
	return (true);
}

bool	validate_philo_count(const uint32_t settings[5])
{
	return (settings[N_PHILO] > 1);
}
