/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozahdi <ozahdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 12:33:58 by ozahdi            #+#    #+#             */
/*   Updated: 2024/10/28 18:24:06 by ozahdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_check_meals(t_data *data, int i)
{
	int		n;

	n = -1;
	pthread_mutex_lock(&data->meals_lock);
	if (data->number_of_meals > 0 && \
		data->philo[i].ph_number_of_meals >= data->number_of_meals)
	{
		data->philo[i].death_flag = 1;
		while (++n < data->number_of_philosophers)
			if (data->philo[n].death_flag != 1)
				break ;
		if (n == data->number_of_philosophers)
		{
			pthread_mutex_unlock(&data->meals_lock);
			return (data->meals_complet = 1, 1);
		}
	}
	pthread_mutex_unlock(&data->meals_lock);
	return (0);
}

void	ft_convert_death_flag(t_data *data)
{
	pthread_mutex_lock(&data->death_flag_lock);
	data->death_time = get_time_of_day(MILLI) - data->start_time;
	data->death_flag = 1;
	pthread_mutex_unlock(&data->death_flag_lock);
}

void	*monitor_routine(void *arg)
{
	int		i;
	t_data	*data;

	data = (t_data *)arg;
	while (1)
	{
		i = -1;
		while (++i < data->number_of_philosophers)
		{
			if (ft_check_meals(data, i) == 1)
				return (NULL);
			pthread_mutex_lock(&data->eat_time_lock);
			if ((data->philo[i].last_eat_time != 0 && get_time_of_day(MILLI) \
				- data->philo[i].last_eat_time > data->time_to_die - 1))
			{
				ft_convert_death_flag(data);
				data->death_id = data->philo[i].id;
				printf("%lld %d died\n", data->death_time, data->death_id);
				pthread_mutex_unlock(&data->eat_time_lock);
				return (NULL);
			}
			pthread_mutex_unlock(&data->eat_time_lock);
		}
	}
	return (NULL);
}

int	monitor_thread(t_data **data)
{
	pthread_t	monitor;

	if (pthread_create(&monitor, NULL, &monitor_routine, *data))
		return (ft_putstr_fd("Philo: Thread creation failed!\n", 2), 1);
	pthread_join(monitor, NULL);
	return (0);
}

int	main(int ac, char **av)
{
	t_data		*data;
	int			i;

	if (ac != 5 && ac != 6)
		return (ft_putstr_fd("Error: Wrong number of arguments\n", 2), 1);
	data = malloc(sizeof(t_data));
	if (!data)
		return (ft_putstr_fd("Error: Memory allocation error\n", 2), 1);
	if (ft_parsing(&data, av))
		return (1);
	i = -1;
	data->start_time = get_time_of_day(MILLI);
	while (++i < data->number_of_philosophers)
		if (pthread_create(&data->philo[i].philo, NULL, \
			&routine, &data->philo[i]))
			return (ft_putstr_fd("Error: Thread creation error\n", 2), 1);
	i = -1;
	monitor_thread(&data);
	while (++i < data->number_of_philosophers)
		pthread_join(data->philo[i].philo, NULL);
}
