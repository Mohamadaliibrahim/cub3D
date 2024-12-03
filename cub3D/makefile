CC = gcc
CFLAGS = -Wall -Wextra -Werror -g

SRC =	cub3d.c \
		src/checking/checking_map.c \
		src/checking/checking_utils.c\
		src/get_next_line/get_next_line.c\
		src/converting/converting.c\
		src/validate_map/validate_map.c\
		src/validate_map/utils.c\
		src/checking/checking.c\
		src/checking/parsing.c\
		src/checking/utils.c\
		src/checking/value.c\
		src/mlx/calc.c\
		src/mlx/draw_and_move.c\
		src/mlx/draw2.c\
		src/mlx/error_free.c\
		src/mlx/exit_check.c\
		src/mlx/init.c\
		src/mlx/render_calc2.c


OBJ = $(SRC:.c=.o)
NAME = cub3d

# Directories for libraries
FT_PRINTF_DIR = ft_printf
LIBFT_DIR = lib_ft
MLX_DIR = minilibx-linux  # Directory containing MiniLibX

# Libraries
FT_PRINTF_LIB = $(FT_PRINTF_DIR)/libftprintf.a
LIBFT_LIB = $(LIBFT_DIR)/libft.a
MLX_LIB = -L$(MLX_DIR) -lmlx -lXext -lX11 -lm -lz

# Adding the libraries to the final link
LIBS = $(FT_PRINTF_LIB) $(LIBFT_LIB) $(MLX_LIB)

all: $(NAME)

$(NAME): $(OBJ) $(FT_PRINTF_LIB) $(LIBFT_LIB)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -I includes -I $(MLX_DIR) -c $< -o $@

$(FT_PRINTF_LIB):
	$(MAKE) -C $(FT_PRINTF_DIR)

$(LIBFT_LIB):
	$(MAKE) -C $(LIBFT_DIR)

clean:
	rm -f $(OBJ)
	$(MAKE) -C $(FT_PRINTF_DIR) clean
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(FT_PRINTF_DIR) fclean
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re