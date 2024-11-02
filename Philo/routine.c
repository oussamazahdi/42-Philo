/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozahdi <ozahdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 16:41:01 by ozahdi            #+#    #+#             */
/*   Updated: 2024/10/31 12:01:45 by ozahdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	printer(t_philo *philo, long long time, char *msg)
{
	pthread_mutex_lock(&philo->data->death_flag_lock);
	pthread_mutex_lock(&philo->data->n_meals_lock);
	if (!philo->data->death_flag && !philo->data->meals_complet)
	{
		pthread_mutex_lock(&philo->data->print_lock);
		printf ("%lld %d %s\n", time, philo->id, msg);
	}
	pthread_mutex_unlock(&philo->data->death_flag_lock);
	pthread_mutex_unlock(&philo->data->n_meals_lock);
	pthread_mutex_unlock(&philo->data->print_lock);
}

void	ft_add_meal_to_philo(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->meals_lock);
	philo->ph_number_of_meals++;
	pthread_mutex_unlock(&philo->data->meals_lock);
}

int	ft_check_death(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->death_flag_lock);
	pthread_mutex_lock(&philo->data->n_meals_lock);
	if (philo->data->death_flag || philo->data->meals_complet)
	{
		pthread_mutex_unlock(&philo->data->n_meals_lock);
		pthread_mutex_unlock(&philo->data->death_flag_lock);
		return (1);
	}
	pthread_mutex_unlock(&philo->data->death_flag_lock);
	pthread_mutex_unlock(&philo->data->n_meals_lock);
	return (0);
}

void	ft_simulation(t_philo *philo)
{
	while (1)
	{
		if (ft_check_death(philo))
			break ;
		pthread_mutex_lock(&philo->data->fork[philo->ft_fork]);
		printer(philo, get_time_of_day(MILLI) - philo->data->start_time, FORK);
		pthread_mutex_lock(&philo->data->fork[philo->sc_fork]);
		printer(philo, get_time_of_day(MILLI) - philo->data->start_time, FORK);
		printer(philo, get_time_of_day(MILLI) - philo->data->start_time, EAT);
		pthread_mutex_lock(&philo->data->eat_time_lock);
		philo->last_eat_time = get_time_of_day(MILLI);
		pthread_mutex_unlock(&philo->data->eat_time_lock);
		ft_sleep(philo->data->time_to_eat, philo);
		ft_add_meal_to_philo(philo);
		pthread_mutex_unlock(&philo->data->fork[philo->ft_fork]);
		pthread_mutex_unlock(&philo->data->fork[philo->sc_fork]);
		printer(philo, get_time_of_day(MILLI) - philo->data->start_time, SLP);
		ft_sleep(philo->data->time_to_sleep, philo);
		printer(philo, get_time_of_day(MILLI) - philo->data->start_time, TNK);
	}
}

void	*routine(void *arg)
{
	t_philo		*philo;

	philo = (t_philo *)arg;
	if (philo->data->number_of_philosophers == 1)
	{
		printf("%lld %d has taken a fork\n", \
		get_time_of_day(MILLI) - philo->data->start_time, philo->id);
		ft_sleep(philo->data->time_to_die, philo);
		return (NULL);
	}
	if (!(philo->id % 2))
		usleep(100);
	ft_simulation(philo);
	return (NULL);
}
