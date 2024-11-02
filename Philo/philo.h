/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozahdi <ozahdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 16:53:34 by ozahdi            #+#    #+#             */
/*   Updated: 2024/11/02 12:09:44 by ozahdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <libc.h>
# include <sys/time.h>

# define FORK "has taken a fork"
# define EAT "is eating"
# define SLP "is sleeping"
# define TNK "is thinking"

typedef enum e_state
{
	MILLI,
	MACRO,
}	t_state;

typedef struct s_philo	t_philo;
typedef struct s_data	t_data;
struct s_data
{
	int					number_of_philosophers;
	int					number_of_meals;
	int					time_to_sleep;
	int					meals_complet;
	int					time_to_die;
	int					time_to_eat;
	int					death_flag;
	long long			death_time;
	long long			start_time;
	int					death_id;
	int					eat_time;
	t_philo				*philo;
	pthread_mutex_t		death_flag_lock;
	pthread_mutex_t		eat_time_lock;
	pthread_mutex_t		meals_lock;
	pthread_mutex_t		print_lock;
	pthread_mutex_t		n_meals_lock;
	pthread_mutex_t		*fork;
};

struct s_philo
{
	int					ph_number_of_meals;
	long long			last_eat_time;
	int					ph_start_time;
	int					death_flag;
	int					die_time;
	int					eat_time;
	int					ft_fork;
	int					sc_fork;
	int					id;
	pthread_t			philo;
	t_data				*data;
};

void			ft_sleep(long long time, t_philo *philo);
int				ft_parsing(t_data **data, char **av);
void			ft_putstr_fd(char *s, int fd);
long long		get_time_of_day(int type);
void			*routine(void *arg);
// long long		ft_atoi(char *src);
int				ft_atoi(char *src);
int				ft_close_all(t_data *data);
void			ft_die_print(t_data *data, long long time, int philo);

#endif