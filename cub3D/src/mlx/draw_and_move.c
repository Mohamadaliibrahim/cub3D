/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_and_move.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjoundi <mjoundi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 19:40:48 by mjoundi           #+#    #+#             */
/*   Updated: 2024/12/02 19:49:22 by mjoundi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

void	draw_floor(t_game *game)
{
	int	x;
	int	y;
	int	color;

	color = (game->config.f_r << 16) | (game->config.f_g << 8)
		| game->config.f_b;
	y = game->window_height / 2;
	while (y < game->window_height)
	{
		x = 0;
		while (x < game->window_width)
		{
			set_pixel(game, x, y, color);
			x++;
		}
		y++;
	}
}

void	update_direction(t_game *game)
{
	game->dir_x = cos(game->player_angle);
	game->dir_y = sin(game->player_angle);
	game->plane_x = -sin(game->player_angle) * 0.66;
	game->plane_y = cos(game->player_angle) * 0.66;
}

void	handle_mov2(double new_x, double new_y, t_game *game)
{
	if (game->config.map[(int)(new_y)][(int)(game->player_x)] != '1')
		game->player_y = new_y;
	if (game->config.map[(int)(game->player_y)][(int)(new_x)] != '1')
		game->player_x = new_x;
}

void	handle_movement(int keycode, t_game *game, double move_speed)
{
	double	new_x;
	double	new_y;

	if (keycode == 119)
	{
		new_x = game->player_x + game->dir_x * move_speed;
		new_y = game->player_y + game->dir_y * move_speed;
	}
	else if (keycode == 115)
	{
		new_x = game->player_x - game->dir_x * move_speed;
		new_y = game->player_y - game->dir_y * move_speed;
	}
	else if (keycode == 97)
	{
		new_x = game->player_x - game->plane_x * move_speed;
		new_y = game->player_y - game->plane_y * move_speed;
	}
	else if (keycode == 100)
	{
		new_x = game->player_x + game->plane_x * move_speed;
		new_y = game->player_y + game->plane_y * move_speed;
	}
	handle_mov2(new_x, new_y, game);
}

int	key_press(int keycode, t_game *game)
{
	double	move_speed;

	move_speed = MOVE_SPEED;
	if (keycode == 65307)
		close_window(game);
	else if (keycode == 119
		|| keycode == 115 || keycode == 97 || keycode == 100)
		handle_movement(keycode, game, move_speed);
	else if (keycode == 65361)
	{
		game->player_angle -= ROT_SPEED;
		update_direction(game);
	}
	else if (keycode == 65363)
	{
		game->player_angle += ROT_SPEED;
		update_direction(game);
	}
	if (game->player_angle < 0)
		game->player_angle += 2 * M_PI;
	else if (game->player_angle >= 2 * M_PI)
		game->player_angle -= 2 * M_PI;
	render_b(game);
	return (0);
}
