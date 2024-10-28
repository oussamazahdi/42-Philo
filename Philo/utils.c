/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozahdi <ozahdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 12:37:35 by ozahdi            #+#    #+#             */
/*   Updated: 2024/10/28 18:25:07 by ozahdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_sleep(long long time, t_philo *philo)
{
	long long	start;

	start = get_time_of_day(MILLI);
	while (get_time_of_day(MILLI) - start < time)
	{
		pthread_mutex_lock(&philo->data->death_flag_lock);
		if (philo->data->death_flag)
		{
			pthread_mutex_unlock(&philo->data->death_flag_lock);
			return ;
		}
		pthread_mutex_unlock(&philo->data->death_flag_lock);
		usleep(100);
	}
}

long long	ft_atol(char *src)
{
	long long	result;
	long long	i;

	result = 0;
	i = 0;
	while (src[i])
	{
		if (src[i] >= '0' && src[i] <= '9')
			i++;
		else
			return (-1);
	}
	while (*src && *src >= '0' && *src <= '9')
	{
		result *= 10;
		result += *src++ - '0';
	}
	if (result > 2147483647)
		return (-1);
	return (result);
}

void	ft_putstr_fd(char *s, int fd)
{
	if (!s)
		return ;
	while (*s)
	{
		write(fd, s, 1);
		s++;
	}
}

long long	get_time_of_day(int type)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	if (type == MILLI)
		return (tv.tv_sec * 1000 + (tv.tv_usec) / 1000);
	else if (type == MACRO)
		return (tv.tv_sec * 1000000 + tv.tv_usec);
	return (0);
}
