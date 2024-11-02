/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utiles2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozahdi <ozahdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 11:09:38 by ozahdi            #+#    #+#             */
/*   Updated: 2024/11/02 12:41:09 by ozahdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_die_print(t_data *data, long long time, int philo)
{
	char		*msg;

	msg = "died";
	pthread_mutex_lock(&data->print_lock);
	printf("%lld %d %s\n", time, philo, msg);
	pthread_mutex_unlock(&data->print_lock);
	pthread_mutex_unlock(&data->eat_time_lock);
}

int	atoi_pars(char *src)
{
	int		i;

	i = 0;
	if (src[i] == '-')
		return (-1);
	if (src[i] == '+')
		i++;
	while (src[i])
	{
		if (src[i] >= '0' && src[i] <= '9')
			i++;
		else
			return (-1);
	}
	return (0);
}

int	ft_atoi(char *src)
{
	long long		result;
	long long		t;
	int				i;

	result = 0;
	i = 0;
	if (atoi_pars(src) == -1)
		return (-1);
	if (*src && *src == '+')
		src++;
	while (*src && *src >= '0' && *src <= '9')
	{
		t = result;
		result *= 10;
		result += *src++ - '0';
		if (result / 10 != t || result > 2147483647)
			return (-1);
	}
	return (result);
}
