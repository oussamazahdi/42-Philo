CC			=	cc
CFLAGS		=	-Wall -Wextra -Werror
NAME		=	philo
SRC			=	philo.c utils.c routine.c parsing.c utiles2.c\

OBJS		=	$(SRC:.c=.o)

all			:	$(NAME)

$(NAME)		:	$(OBJS)
			$(CC) -lpthread $(CFLAGS) -o $(NAME) $(OBJS)

%.o			:	%.c philo.h
			$(CC) $(CFLAGS) -c $< -o $@

clean		:
			rm -rf $(OBJS)

fclean		:	clean
			rm -rf $(NAME)

make re		:	fclean all

.PHONY		:	clean