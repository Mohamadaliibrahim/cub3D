/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 23:16:10 by marvin            #+#    #+#             */
/*   Updated: 2024/11/29 10:46:29 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/cub3d.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void	update_direction(t_game *game);
void	render_b(t_game *game);
int		game_loop(t_game *game);

void	free_config_1(t_config *config)
{
	if (config->no_texture)
	{
		free(config->no_texture);
		config->no_texture = NULL;
	}
	if (config->so_texture)
	{
		free(config->so_texture);
		config->so_texture = NULL;
	}
	if (config->we_texture)
	{
		free(config->we_texture);
		config->we_texture = NULL;
	}
	if (config->ea_texture)
	{
		free(config->ea_texture);
		config->ea_texture = NULL;
	}
}

void	free_config(t_config *config)
{
	int	i;

	free_config_1(config);
	if (config->map)
	{
		i = 0;
		while (config->map[i])
		{
			free(config->map[i]);
			config->map[i] = NULL;
			i++;
		}
		free(config->map);
		config->map = NULL;
	}
}

void	write_error(char *str)
{
	while (*str)
		write(2, str++, 1);
}

void	validate_config(t_config *confige)
{
	if (!confige->no_texture || !confige->so_texture || !confige->we_texture
		|| !confige->ea_texture)
	{
		write_error("Error: Missing texture path(s)\n");
		free_config(confige);
		exit(EXIT_FAILURE);
	}
	if (confige->f_r == -1 || confige->c_r == -1)
	{
		write_error("Error: Missing floor or ceiling color\n");
		free_config(confige);
		exit(EXIT_FAILURE);
	}
	if (!confige->map)
	{
		write_error("Error: Map is missing\n");
		free_config(confige);
		exit(EXIT_FAILURE);
	}
}

t_config	check_map(char *av)
{
	t_config	confige;

	open_map_and_else(av, &confige);
	convert_map_list_to_array(&confige);
	validate_config(&confige);
	validate_map(&confige);
	return (confige);
}

void	file_name(char *str)
{
	int	len;

	len = ft_strlen(str);
	if (len < 4 || ft_strncmp(&str[len - 4], ".cub", 4) != 0)
	{
		write_error("cub3D: Error: Not a .cub file\n");
		exit(1);
	}
}

void	print_config(t_config conf)
{
	int	i;

	i = 0;
	printf("N:%s\n", conf.no_texture);
	printf("S:%s\n", conf.so_texture);
	printf("W:%s\n", conf.we_texture);
	printf("E:%s\n", conf.ea_texture);
	printf("PX: %d\n", conf.player_x);
	printf("PY: %d\n", conf.player_y);
	printf("PO: %d\n", conf.player_orientation);
	printf("FC: %d %d %d\n", conf.f_r, conf.f_g, conf.f_b);
	printf("CC: %d %d %d\n", conf.c_r, conf.c_g, conf.c_b);
	while (conf.map && conf.map[i])
	{
		printf("%s\n", conf.map[i]);
		i++;
	}
}

void	destroing(t_game *game)
{
	if (game->mlx)
	{
		mlx_destroy_display(game->mlx);
	}
	if (game->mlx)
	{
		free(game->mlx);
		game->mlx = NULL;
	}
}

void	exit_program(t_game *game)
{
	int	i;

	if (game->img)
	{
		mlx_destroy_image(game->mlx, game->img);
		game->img = NULL;
	}
	i = 0;
	while (i < 4)
	{
		if (game->textures[i])
		{
			mlx_destroy_image(game->mlx, game->textures[i]);
			game->textures[i] = NULL;
		}
		i++;
	}
	if (game->win)
	{
		mlx_destroy_window(game->mlx, game->win);
		game->win = NULL;
	}
	destroing(game);
	free_config(&game->config);
	exit(0);
}

int	close_window(t_game *game)
{
	write(1, "Window closed\n", 14);
	exit_program(game);
	return (0);
}

void	initialize_mlx(t_game *game)
{
	game->mlx = mlx_init();
	if (!game->mlx)
	{
		write_error("Error: Failed to initialize MLX\n");
		exit_program(game);
	}
}

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

void	set_new(int	keycode, double *x, double *y, double speed, t_game *game)
{
	if (keycode == 119)
	{
		*x = game->player_x + game->dir_x * speed;
		*y = game->player_y + game->dir_y * speed;
	}
	else if (keycode == 115)
	{
		*x = game->player_x - game->dir_x * speed;
		*y = game->player_y - game->dir_y * speed;
	}
	else if (keycode == 97)
	{
		*x = game->player_x - game->plane_x * speed;
		*y = game->player_y - game->plane_y * speed;
	}
	else if (keycode == 100)
	{
		*x = game->player_x + game->plane_x * speed;
		*y = game->player_y + game->plane_y * speed;
	}
}

void	handle_movement(int keycode, t_game *game, double move_speed)
{
	double	new_x;
	double	new_y;

	set_new(keycode, &new_x, &new_y, move_speed, game);
	if (game->config.map[(int)(new_y)][(int)(game->player_x)] != '1')
		game->player_y = new_y;
	if (game->config.map[(int)(game->player_y)][(int)(new_x)] != '1')
		game->player_x = new_x;
}

int	key_press(int keycode, t_game *game)
{
	double	move_speed;

	move_speed = MOVE_SPEED;
	if (keycode == 65307)
		close_window(game);
	else if (keycode == 119 || keycode == 115 || keycode == 97 || keycode == 100)
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

void	set_step_and_side_dist(t_game *game, t_wall_params *wp)
{
	if (wp->ray_dir_x < 0)
	{
		wp->step_x = -1;
		wp->side_dist_x = (game->player_x - wp->map_x) * wp->delta_dist_x;
	}
	else
	{
		wp->step_x = 1;
		wp->side_dist_x = (wp->map_x + 1.0 - game->player_x) * wp->delta_dist_x;
	}
	if (wp->ray_dir_y < 0)
	{
		wp->step_y = -1;
		wp->side_dist_y = (game->player_y - wp->map_y) * wp->delta_dist_y;
	}
	else
	{
		wp->step_y = 1;
		wp->side_dist_y = (wp->map_y + 1.0 - game->player_y) * wp->delta_dist_y;
	}
}

void	perform_dda(t_game *game, t_wall_params *wp)
{
	while (wp->hit == 0)
	{
		if (wp->side_dist_x < wp->side_dist_y)
		{
			wp->side_dist_x += wp->delta_dist_x;
			wp->map_x += wp->step_x;
			wp->side = 0;
		}
		else
		{
			wp->side_dist_y += wp->delta_dist_y;
			wp->map_y += wp->step_y;
			wp->side = 1;
		}
		if (wp->map_x < 0 || wp->map_x >= game->config.map_width
			|| wp->map_y < 0 || wp->map_y >= game->config.map_height)
			wp->hit = 1;
		else if (game->config.map[wp->map_y][wp->map_x] == '1')
			wp->hit = 1;
	}
}

void	calculate_perp_wall_dist(t_game *game, t_wall_params *wp)
{
	if (wp->side == 0)
		wp->perp_wall_dist = (wp->map_x - game->player_x
				+ (1 - wp->step_x) / 2) / wp->ray_dir_x;
	else
		wp->perp_wall_dist = (wp->map_y - game->player_y
				+ (1 - wp->step_y) / 2) / wp->ray_dir_y;
}

void	calculate_line_params(t_game *game, t_draw_wall_params *dwp,
		t_wall_params *wp)
{
	dwp->line_height = (int)(game->window_height / wp->perp_wall_dist);
	dwp->start = -dwp->line_height / 2 + game->window_height / 2;
	if (dwp->start < 0)
		dwp->start = 0;
	dwp->end = dwp->line_height / 2 + game->window_height / 2;
	if (dwp->end >= game->window_height)
		dwp->end = game->window_height - 1;
}

void	calculate_texture_id(t_draw_wall_params *dwp, t_wall_params *wp)
{
	if (wp->side == 0)
	{
		if (wp->ray_dir_x > 0)
			dwp->tex_id = 0;
		else
			dwp->tex_id = 1;
	}
	else
	{
		if (wp->ray_dir_y > 0)
			dwp->tex_id = 2;
		else
			dwp->tex_id = 3;
	}
}

void	calculate_texture_coords(t_game *game, t_draw_wall_params *dwp,
		t_wall_params *wp)
{
	if (wp->side == 0)
		dwp->wall_x = game->player_y + wp->perp_wall_dist * wp->ray_dir_y;
	else
		dwp->wall_x = game->player_x + wp->perp_wall_dist * wp->ray_dir_x;
	dwp->wall_x -= floor(dwp->wall_x);
	dwp->tex_x = (int)(dwp->wall_x * (double)game->tex_width);
	if ((wp->side == 0 && wp->ray_dir_x > 0)
		|| (wp->side == 1 && wp->ray_dir_y < 0))
		dwp->tex_x = game->tex_width - dwp->tex_x - 1;
	dwp->tex_step = 1.0 * game->tex_height / dwp->line_height;
}

void	calculate_draw_params(t_game *game, t_draw_wall_params *dwp,
		t_wall_params *wp, int x)
{
	calculate_line_params(game, dwp, wp);
	calculate_texture_id(dwp, wp);
	calculate_texture_coords(game, dwp, wp);
	dwp->x = x;
}

void	render_walls(t_game *game)
{
	int					x;
	t_wall_params		wp;
	t_draw_wall_params	dwp;

	x = 0;
	while (x < game->window_width)
	{
		calculate_wall_params(game, &wp, x);
		set_step_and_side_dist(game, &wp);
		perform_dda(game, &wp);
		calculate_perp_wall_dist(game, &wp);
		calculate_draw_params(game, &dwp, &wp, x);
		draw_wall(game, &dwp);
		x++;
	}
}

void	render_b(t_game *game)
{
	draw_ceiling(game);
	draw_floor(game);
	render_walls(game);
	mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
}

int	game_loop(t_game *game)
{
	render_b(game);
	return (0);
}

int	main(int ac, char **av)
{
	t_game	game;

	if (ac == 2)
	{
		file_name(av[1]);
		game.config = check_map(av[1]);
		print_config(game.config);
		game.player_angle = M_PI / 2;
		initialize_game_window(&game);
		load_textures(&game);
		render_b(&game);
		mlx_hook(game.win, KeyPress, KeyPressMask, key_press, &game);
		mlx_hook(game.win, 17, 0, close_window, &game);
		mlx_loop(game.mlx);
	}
	else
		write_error("Error: Wrong input\n");
	return (0);
}
