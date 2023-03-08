SRCS 	= main.c utils.c process.c

OBJS	= ${SRCS:.c=.o}

NAME	= pipex

CC		= gcc

RM		= rm -f

CFLAGS	= -Wall -Wextra -Werror

.c.o:
			${CC} ${CFLAGS} -Imlx -c $< -o ${<:.c=.o}

${NAME}:	${OBJS}
			make -C libft
			$(CC) $^ -Llibft -lft -o $(NAME)

all:		${NAME}

clean:		
			make clean -C libft
			${RM} ${OBJS}

fclean:		clean
			make fclean -C libft
			${RM} ${NAME}

re:			fclean all

.PHONY:		all clean fclean re