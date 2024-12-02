/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjoundi <mjoundi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 19:39:14 by mjoundi           #+#    #+#             */
/*   Updated: 2024/12/02 19:49:42 by mjoundi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

void	initialize_window(t_game *game)
{
	int	screen_width;
	int	screen_height;

	mlx_get_screen_size(game->mlx, &screen_width, &screen_height);
	game->window_width = screen_width;
	game->window_height = screen_height;
	game->win = mlx_new_window(game->mlx, screen_width,
			screen_height, "Cub3D");
	if (!game->win)
	{
		write_error("Error: Failed to create a window\n");
		exit_program(game);
	}
}

void	initialize_image(t_game *game)
{
	game->img = mlx_new_image(game->mlx, game->window_width,
			game->window_height);
	if (!game->img)
	{
		write_error("Error: Failed to create an image\n");
		exit_program(game);
	}
	game->img_addr = mlx_get_data_addr(game->img, &game->bpp,
			&game->line_length, &game->endian);
}

void	initialize_player(t_game *game)
{
	game->player_x = game->config.player_x + 0.5;
	game->player_y = game->config.player_y + 0.5;
	if (game->config.player_orientation == 'E')
		game->player_angle = 3 * M_PI / 2;
	else if (game->config.player_orientation == 'W')
		game->player_angle = M_PI / 2;
	else if (game->config.player_orientation == 'N')
		game->player_angle = 0;
	else if (game->config.player_orientation == 'S')
		game->player_angle = M_PI;
	else
		game->player_angle = 0;
	game->dir_x = cos(game->player_angle);
	game->dir_y = sin(game->player_angle);
	game->plane_x = -sin(game->player_angle) * 0.66;
	game->plane_y = cos(game->player_angle) * 0.66;
}

void	initialize_game_window(t_game *game)
{
	initialize_mlx(game);
	initialize_window(game);
	initialize_image(game);
	initialize_player(game);
}

void	set_pixel(t_game *game, int x, int y, int color)
{
	char	*pixel;

	if (x >= 0 && x < game->window_width && y >= 0
		&& y < game->window_height)
	{
		pixel = game->img_addr + (y * game->line_length
				+ x * (game->bpp / 8));
		*(unsigned int *)pixel = color;
	}
}
