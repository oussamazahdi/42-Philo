/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozahdi <ozahdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 12:37:35 by ozahdi            #+#    #+#             */
/*   Updated: 2024/11/02 12:41:01 by ozahdi           ###   ########.fr       */
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

int	ft_close_all(t_data *data)
{
	int		i;

	i = -1;
	if (pthread_mutex_destroy(&data->death_flag_lock))
		return (ft_putstr_fd("Error: Mutex destroying error!\n", 2), 1);
	if (pthread_mutex_destroy(&data->eat_time_lock))
		return (ft_putstr_fd("Error: Mutex destroying error!\n", 2), 1);
	if (pthread_mutex_destroy(&data->meals_lock))
		return (ft_putstr_fd("Error: Mutex destroying error!\n", 2), 1);
	while (++i < data->number_of_philosophers)
		if (pthread_mutex_destroy(&data->fork[i]))
			return (ft_putstr_fd("Error: Mutex destroying error!\n", 2), 1);
	free(data->fork);
	free(data->philo);
	free(data);
	return (0);
}
