/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjoundi <mjoundi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 19:43:30 by mjoundi           #+#    #+#             */
/*   Updated: 2024/12/02 19:49:32 by mjoundi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

void	draw_ceiling(t_game *game)
{
	int	x;
	int	y;
	int	color;

	color = (game->config.c_r << 16) | (game->config.c_g << 8)
		| game->config.c_b;
	y = 0;
	while (y < game->window_height / 2)
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

void	render_c(t_game *game)
{
	draw_ceiling(game);
	draw_floor(game);
	mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
}

void	load_textures(t_game *game)
{
	int	i;

	game->textures[0] = mlx_xpm_file_to_image(game->mlx,
			game->config.no_texture, &game->tex_width, &game->tex_height);
	game->textures[1] = mlx_xpm_file_to_image(game->mlx,
			game->config.so_texture, &game->tex_width, &game->tex_height);
	game->textures[2] = mlx_xpm_file_to_image(game->mlx,
			game->config.we_texture, &game->tex_width, &game->tex_height);
	game->textures[3] = mlx_xpm_file_to_image(game->mlx,
			game->config.ea_texture, &game->tex_width, &game->tex_height);
	i = 0;
	while (i < 4)
	{
		if (!game->textures[i])
		{
			write_error("Error: Failed to load a texture\n");
			exit_program(game);
		}
		game->tex_data[i] = mlx_get_data_addr(game->textures[i],
				&game->tex_bpp, &game->tex_line_length, &game->tex_endian);
		i++;
	}
}

void	draw_wall(t_game *game, t_draw_wall_params *params)
{
	int		y;
	int		tex_y;
	double	tex_pos;
	char	*tex_pixel;
	int		color;

	tex_pos = (params->start - game->window_height / 2
			+ (params->end - params->start) / 2) * params->tex_step;
	y = params->start;
	while (y < params->end)
	{
		tex_y = (int)tex_pos & (game->tex_height - 1);
		tex_pos += params->tex_step;
		tex_pixel = game->tex_data[params->tex_id]
			+ (tex_y * game->tex_line_length
				+ params->tex_x * (game->tex_bpp / 8));
		color = *(unsigned int *)tex_pixel;
		set_pixel(game, params->x, y, color);
		y++;
	}
}

void	calculate_wall_params(t_game *game, t_wall_params *wp, int x)
{
	wp->camera_x = 2 * x / (double)game->window_width - 1;
	wp->ray_dir_x = game->dir_x + game->plane_x * wp->camera_x;
	wp->ray_dir_y = game->dir_y + game->plane_y * wp->camera_x;
	wp->map_x = (int)game->player_x;
	wp->map_y = (int)game->player_y;
	wp->delta_dist_x = fabs(1 / wp->ray_dir_x);
	wp->delta_dist_y = fabs(1 / wp->ray_dir_y);
	wp->hit = 0;
}
