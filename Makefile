NAME=pipex
DIR_LIBFT=libft
LIBFT=$(DIR_LIBFT)/libft.a
SOURCE= src/pipex.c src/pipex_utils.c src/input_handling.c src/output_handling.c
OBJ=$(SOURCE:.c=.o)
FLAGS= -Wall -Wextra -Werror
CC= cc

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $^ -I$(DIR_LIBFT) -o $(NAME)

%.o:%.c
	$(CC) $(FLAGS) -I$(DIR_LIBFT) -c $< -o $@

$(LIBFT):
	make -C $(DIR_LIBFT) bonus

clean:
	rm -f src/*.o
	make -C $(DIR_LIBFT) clean

fclean: clean
	rm -f $(NAME)
	make -C $(DIR_LIBFT) fclean

re: fclean all

.PHONY: all clean fclean re
