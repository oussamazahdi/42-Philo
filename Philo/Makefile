CC			=	cc
CFLAGS		=	-Wall -Wextra -Werror
# CFLAGS		=	-Wall -Wextra -Werror -g -fsanitize=thread
# CFLAGS		=	-Wall -Wextra -Werror -fsanitize=address
NAME		=	philo
SRC			=	philo.c utils.c\

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