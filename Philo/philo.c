/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozahdi <ozahdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 12:33:58 by ozahdi            #+#    #+#             */
/*   Updated: 2024/10/28 12:02:14 by ozahdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


int ft_synchronization(t_data **data)
{
	int		i;

	i = -1;
	(*data)->philo = malloc(sizeof(t_philo) * (*data)->number_of_philosophers);
	(*data)->fork = malloc(sizeof(pthread_mutex_t) * (*data)->number_of_philosophers);
	if (!(*data)->philo || !(*data)->fork)
		return (ft_putstr_fd("Error: Memory allocation error\n", 2), 1);
	(*data)->death_flag = 0;
	(*data)->death_time = 0;
	(*data)->eat_time = 0;
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
	i = -1;
	while (++i < (*data)->number_of_philosophers)
		if (pthread_mutex_init(&(*data)->fork[i], NULL))
			return (ft_putstr_fd("Error: Mutex initialization error\n", 2), 1);
	pthread_mutex_init(&(*data)->print_lock, NULL);
	pthread_mutex_init(&(*data)->death_flag_lock, NULL);
	pthread_mutex_init(&(*data)->eat_time_lock, NULL);
	pthread_mutex_init(&(*data)->meals_lock, NULL);
	return (0);
}

int	ft_parsing(t_data **data, char **av)
{
	(*data)->number_of_philosophers = ft_atol(av[1]);
	if ((*data)->number_of_philosophers < 1 || (*data)->number_of_philosophers > 200)
		return (ft_putstr_fd("Error: Wrong number of philosophers\n", 2), 1);
	(*data)->time_to_die = ft_atol(av[2]);
	if ((*data)->time_to_die < 60)
		return (ft_putstr_fd("Error: Wrong time to die\n", 2), 1);
	(*data)->time_to_eat = ft_atol(av[3]);
	if ((*data)->time_to_eat < 60)
		return (ft_putstr_fd("Error: Wrong time to eat\n", 2), 1);
	(*data)->time_to_sleep = ft_atol(av[4]);
	if ((*data)->time_to_sleep < 60)
		return (ft_putstr_fd("Error: Wrong time to sleep\n", 2), 1);
	(*data)->number_of_meals = -2;
	if (av[5])
		(*data)->number_of_meals = ft_atol(av[5]);
	if (ft_synchronization(data))
		return (1);
	(*data)->start_time = get_time_of_day(MILLI);
	return (0);
}
void ft_sleep(long long time, t_philo *philo)
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

void	ft_print(t_philo *philo, long long time, int id, char *state)
{
	pthread_mutex_lock(&philo->data->death_flag_lock);
	if (!philo->data->death_flag)
	{
		pthread_mutex_lock(&philo->data->print_lock);
		printf("%lld %d %s\n", time, id, state);
	}
	pthread_mutex_unlock(&philo->data->death_flag_lock);
	pthread_mutex_unlock(&philo->data->print_lock);
}


void printer(t_philo *philo, long long time, char *msg)
{
	pthread_mutex_lock(&philo->data->death_flag_lock);
	// pthread_mutex_lock(&philo->data->print_lock);
	if (!philo->data->death_flag)
		printf ("%lld %d %s\n", time, philo->id, msg);
	pthread_mutex_unlock(&philo->data->death_flag_lock);
	// pthread_mutex_unlock(&philo->data->print_lock);
}

// void ft_simulation(t_philo *philo)
// {
// 	while (1)
// 	{
// 		pthread_mutex_lock(&philo->data->death_flag_lock);
// 		if (philo->data->death_flag)
// 		{
// 			pthread_mutex_unlock(&philo->data->death_flag_lock);
// 			break ;
// 		}
// 		pthread_mutex_unlock(&philo->data->death_flag_lock);
// 		pthread_mutex_lock(&philo->data->fork[philo->ft_fork]);
// 		printer(philo, get_time_of_day(MILLI) - philo->data->start_time, FORK);
		
// 		// pthread_mutex_lock(&philo->data->death_flag_lock);
// 		// if (!philo->data->death_flag)
// 		// 	printf("%lld %d has taken a fork\n", get_time_of_day(MILLI) - philo->data->start_time, philo->id);
// 		// pthread_mutex_unlock(&philo->data->death_flag_lock);
// 		pthread_mutex_lock(&philo->data->fork[philo->sc_fork]);
// 		printer(philo, get_time_of_day(MILLI) - philo->data->start_time, FORK);
// 			// if (!philo->data->death_flag)
// 			// 	printf("%lld %d has taken a fork\n", get_time_of_day(MILLI) - philo->data->start_time, philo->id);
// 		printer(philo, get_time_of_day(MILLI) - philo->data->start_time, EAT);
// 		// if (!philo->data->death_flag )
// 		// 	printf("%lld %d is eating\n", get_time_of_day(MILLI) - philo->data->start_time, philo->id);
// 		ft_sleep(philo->data->time_to_eat, philo);
// 		pthread_mutex_lock(&philo->data->eat_time_lock);
// 		philo->last_eat_time = get_time_of_day(MILLI);
// 		pthread_mutex_unlock(&philo->data->eat_time_lock);
// 		pthread_mutex_unlock(&philo->data->fork[philo->ft_fork]);
// 		pthread_mutex_unlock(&philo->data->fork[philo->sc_fork]);
// 		printer(philo, get_time_of_day(MILLI) - philo->data->start_time, SLP);
// 		// if (!philo->data->death_flag)
// 		// 	printf("%lld %d is sleeping\n", get_time_of_day(MILLI) - philo->data->start_time, philo->id);
// 		ft_sleep(philo->data->time_to_sleep, philo);
// 		printer(philo, get_time_of_day(MILLI) - philo->data->start_time, TNK);
// 		// if (!philo->data->death_flag)
// 		// 	printf("%lld %d is thinking\n", get_time_of_day(MILLI) - philo->data->start_time, philo->id);
// 	}
// }

void	ft_add_meal_to_philo(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->meals_lock);
	philo->ph_number_of_meals++;
	pthread_mutex_unlock(&philo->data->meals_lock);
}

void	ft_simulation(t_philo *philo)
{
	while (1)
	{
		pthread_mutex_lock(&philo->data->death_flag_lock);
		if (philo->data->death_flag)
		{
			pthread_mutex_unlock(&philo->data->death_flag_lock);
			break ;
		}
		pthread_mutex_unlock(&philo->data->death_flag_lock);
		pthread_mutex_lock(&philo->data->fork[philo->ft_fork]);
		printer(philo, get_time_of_day(MILLI) - philo->data->start_time, FORK);
		pthread_mutex_lock(&philo->data->fork[philo->sc_fork]);
		printer(philo, get_time_of_day(MILLI) - philo->data->start_time, FORK);
		printer(philo, get_time_of_day(MILLI) - philo->data->start_time, EAT);
		ft_sleep(philo->data->time_to_eat, philo);
		pthread_mutex_lock(&philo->data->eat_time_lock);
		ft_add_meal_to_philo(philo);
		philo->last_eat_time = get_time_of_day(MILLI);
		pthread_mutex_unlock(&philo->data->eat_time_lock);
		pthread_mutex_unlock(&philo->data->fork[philo->ft_fork]);
		pthread_mutex_unlock(&philo->data->fork[philo->sc_fork]);
		printer(philo, get_time_of_day(MILLI) - philo->data->start_time, SLP);
		ft_sleep(philo->data->time_to_sleep, philo);
		printer(philo, get_time_of_day(MILLI) - philo->data->start_time, TNK);
	}
}
// void ft_simulation(t_philo *philo)
// {
// 	while (1)
// 	{
// 		pthread_mutex_lock(&philo->data->death_flag_lock);
// 		if (philo->data->death_flag)
// 		{
// 			pthread_mutex_unlock(&philo->data->death_flag_lock);
// 			break ;
// 		}
// 		pthread_mutex_unlock(&philo->data->death_flag_lock);
// 		pthread_mutex_lock(&philo->data->fork[philo->ft_fork]);
// 		if (!philo->data->death_flag)
// 			printf("%lld %d has taken a fork\n", get_time_of_day(MILLI) - philo->data->start_time, philo->id);
// 		pthread_mutex_lock(&philo->data->fork[philo->sc_fork]);
// 			if (!philo->data->death_flag)
// 				printf("%lld %d has taken a fork\n", get_time_of_day(MILLI) - philo->data->start_time, philo->id);
// 		if (!philo->data->death_flag )
// 			printf("%lld %d is eating\n", get_time_of_day(MILLI) - philo->data->start_time, philo->id);
// 		ft_sleep(philo->data->time_to_eat, philo);
// 		pthread_mutex_lock(&philo->data->eat_time_lock);
// 		philo->last_eat_time = get_time_of_day(MILLI);
// 		pthread_mutex_unlock(&philo->data->eat_time_lock);
// 		pthread_mutex_unlock(&philo->data->fork[philo->ft_fork]);
// 		pthread_mutex_unlock(&philo->data->fork[philo->sc_fork]);
// 		if (!philo->data->death_flag)
// 			printf("%lld %d is sleeping\n", get_time_of_day(MILLI) - philo->data->start_time, philo->id);
// 		ft_sleep(philo->data->time_to_sleep, philo);
// 		if (!philo->data->death_flag)
// 			printf("%lld %d is thinking\n", get_time_of_day(MILLI) - philo->data->start_time, philo->id);
// 	}
// }

void *routine(void *arg)
{
	t_philo		*philo;

	philo = (t_philo *)arg;
	if (philo->data->number_of_philosophers == 1)
	{
		printf("%lld %d has taken a fork\n", get_time_of_day(MILLI) - philo->data->start_time, philo->id);
		ft_sleep(philo->data->time_to_die, philo);
		return	NULL;
	}
	if (!(philo->id % 2))
		usleep(100);
	ft_simulation(philo);
	return (NULL);
}

void	*monitor_routine(void *arg)
{
	int		i;
	int		z;
	t_data	*data;

	data = (t_data *)arg;
	while (1)
	{
		i = -1;
		while (++i < data->number_of_philosophers)
		{
			pthread_mutex_lock(&data->meals_lock);
			if (data->number_of_meals > 0 && data->philo[i].ph_number_of_meals >= data->number_of_meals)
				data->philo[i].death_flag = 1;
			z = -1;
			while (++z < data->number_of_philosophers)
				if (data->philo[z].death_flag != 1)
					break ;
			pthread_mutex_lock(&data->death_flag_lock);
			if (z == data->number_of_philosophers)
				data->death_flag = 1;
			pthread_mutex_unlock(&data->meals_lock);
			pthread_mutex_unlock(&data->death_flag_lock);
			pthread_mutex_lock(&data->eat_time_lock);
			if ((data->philo[i].last_eat_time != 0 && get_time_of_day(MILLI) - data->philo[i].last_eat_time >= data->time_to_die) || data->death_flag == 1)
			{
				pthread_mutex_lock(&data->death_flag_lock);
				data->death_time = get_time_of_day(MILLI) - data->start_time;
				data->death_flag = 1;
				pthread_mutex_unlock(&data->death_flag_lock);
				data->death_id = data->philo[i].id;
				pthread_mutex_lock(&data->print_lock);
				if (data->number_of_meals < 0)
					printf("%lld %d died\n", data->death_time, data->death_id);
				pthread_mutex_unlock(&data->print_lock);
				pthread_mutex_unlock(&data->eat_time_lock);
				return NULL;
			}
			pthread_mutex_unlock(&data->eat_time_lock);
		}
	}
	return NULL;
}
int	monitor_thread(t_data **data)
{
	pthread_t	monitor;

	if (pthread_create(&monitor, NULL, &monitor_routine, *data))
		return (ft_putstr_fd("Philo: Thread creation failed!\n", 2) ,1);
	pthread_join(monitor, NULL);
	return (0);
}

int main(int ac, char **av)
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
		if (pthread_create(&data->philo[i].philo, NULL, &routine, &data->philo[i]))
			return (ft_putstr_fd("Error: Thread creation error\n", 2), 1);
	i = -1;
	monitor_thread(&data);
	while (++i < data->number_of_philosophers)
		pthread_join(data->philo[i].philo, NULL);
}
