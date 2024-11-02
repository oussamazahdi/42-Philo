/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozahdi <ozahdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 16:43:34 by ozahdi            #+#    #+#             */
/*   Updated: 2024/11/02 12:42:09 by ozahdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	ft_init_philo(t_data **data)
{
	int		i;

	i = -1;
	while (++i < (*data)->number_of_philosophers)
	{
		(*data)->philo[i].id = i + 1;
		(*data)->philo[i].ft_fork = i;
		if (i == (*data)->number_of_philosophers - 1)
			(*data)->philo[i].sc_fork = 0;
		else
			(*data)->philo[i].sc_fork = i + 1;
		(*data)->philo[i].ph_number_of_meals = 0;
		(*data)->philo[i].die_time = 0;
		(*data)->philo[i].death_flag = 0;
		(*data)->philo[i].data = (*data);
		(*data)->philo[i].last_eat_time = get_time_of_day(MILLI);
	}
}

static int	ft_synchronization(t_data **data)
{
	int		i;

	i = -1;
	(*data)->philo = malloc(sizeof(t_philo) * (*data)->number_of_philosophers);
	(*data)->fork = malloc(sizeof(pthread_mutex_t) * \
		(*data)->number_of_philosophers);
	if (!(*data)->philo || !(*data)->fork)
		return (ft_putstr_fd("Error: Memory allocation error\n", 2), 1);
	(*data)->death_flag = 0;
	(*data)->death_time = 0;
	(*data)->eat_time = 0;
	(*data)->meals_complet = 0;
	ft_init_philo(data);
	while (++i < (*data)->number_of_philosophers)
		if (pthread_mutex_init(&(*data)->fork[i], NULL))
			return (ft_putstr_fd("Error: Mutex initialization error\n", 2), 1);
	pthread_mutex_init(&(*data)->death_flag_lock, NULL);
	pthread_mutex_init(&(*data)->eat_time_lock, NULL);
	pthread_mutex_init(&(*data)->meals_lock, NULL);
	pthread_mutex_init(&(*data)->print_lock, NULL);
	pthread_mutex_init(&(*data)->n_meals_lock, NULL);
	return (0);
}

int	ft_parsing(t_data **data, char **av)
{
	(*data)->number_of_philosophers = ft_atoi(av[1]);
	if ((*data)->number_of_philosophers < 1 || \
		(*data)->number_of_philosophers > 200)
		return (ft_putstr_fd("Error: Wrong number of philosophers\n", 2), 1);
	(*data)->time_to_die = ft_atoi(av[2]);
	if ((*data)->time_to_die < 60)
		return (ft_putstr_fd("Error: Wrong time to die\n", 2), 1);
	(*data)->time_to_eat = ft_atoi(av[3]);
	if ((*data)->time_to_eat < 60)
		return (ft_putstr_fd("Error: Wrong time to eat\n", 2), 1);
	(*data)->time_to_sleep = ft_atoi(av[4]);
	if ((*data)->time_to_sleep < 60)
		return (ft_putstr_fd("Error: Wrong time to sleep\n", 2), 1);
	(*data)->number_of_meals = -2;
	if (av[5])
		(*data)->number_of_meals = ft_atoi(av[5]);
	if ((*data)->number_of_meals == 0 || (*data)->number_of_meals == -1)
		return (ft_putstr_fd("Error: Wrong number of meals\n", 2), 1);
	if (ft_synchronization(data))
		return (1);
	(*data)->start_time = get_time_of_day(MILLI);
	return (0);
}
