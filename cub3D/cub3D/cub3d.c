/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 17:47:07 by mohamibr          #+#    #+#             */
/*   Updated: 2024/11/22 23:14:02 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/cub3d.h"
#include <math.h>    // For cos() and sin()
#include <stdio.h>   // For printf()
#include <stdlib.h>  // For exit()
#include <unistd.h>  // For write()
#include <string.h>  // For strerror()

// Function prototypes
void update_direction(t_game *game);
void render_b(t_game *game);
int game_loop(t_game *game);

void	free_config(t_config *config)
{
	int	i;

	if (config->no_texture)
		free(config->no_texture);
	if (config->so_texture)
		free(config->so_texture);
	if (config->we_texture)
		free(config->we_texture);
	if (config->ea_texture)
		free(config->ea_texture);
	if (config->map)
	{
		i = 0;
		while (config->map[i])
		{
			free(config->map[i]);
			i++;
		}
		free(config->map);
	}
}

void	write_error(char *str)
{
	while (*str)
		write(2, str++, 1);
}

void	validate_config(t_config *confige)
{
	// Check for missing textures
	if (!confige->no_texture || !confige->so_texture || !confige->we_texture
		|| !confige->ea_texture)
	{
		write_error("Error: Missing texture path(s)\n");
		free_config(confige);
		exit(EXIT_FAILURE);
	}
	// Check for missing colors
	if (confige->f_r == -1 || confige->c_r == -1)
	{
		write_error("Error: Missing floor or ceiling color\n");
		free_config(confige);
		exit(EXIT_FAILURE);
	}
	// Check for missing map
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

void	exit_program(t_game *game)
{
	if (game->img)
		mlx_destroy_image(game->mlx, game->img);
	if (game->win)
		mlx_destroy_window(game->mlx, game->win);
	if (game->mlx)
		free(game->mlx);
	exit(0);
}

int	close_window(t_game *game)
{
	write(1, "Window closed\n", 14);
	exit_program(game);
	return (0);
}

void initialize_game_window(t_game *game)
{
    int screen_width;
    int screen_height;

    game->mlx = mlx_init();
    if (!game->mlx)
    {
        write_error("Error: Failed to initialize MLX\n");
        exit(1);
    }

    // Get screen size
    mlx_get_screen_size(game->mlx, &screen_width, &screen_height);

    game->window_width = screen_width;
    game->window_height = screen_height;

    game->win = mlx_new_window(game->mlx, screen_width, screen_height, "Cub3D");
    if (!game->win)
    {
        write_error("Error: Failed to create a window\n");
        free(game->mlx);
        exit(1);
    }
    game->img = mlx_new_image(game->mlx, screen_width, screen_height);
    if (!game->img)
    {
        write_error("Error: Failed to create an image\n");
        exit_program(game);
    }
    game->img_addr = mlx_get_data_addr(game->img, &game->bpp, &game->line_length, &game->endian);
    game->player_x = game->config.player_x + 0.5;
    game->player_y = game->config.player_y + 0.5;
    game->dir_x = cos(game->player_angle);
    game->dir_y = sin(game->player_angle);
    game->plane_x = -sin(game->player_angle) * 0.66;
    game->plane_y = cos(game->player_angle) * 0.66;
}

// Utility to set a pixel in the image
void	set_pixel(t_game *game, int x, int y, int color)
{
	if (x >= 0 && x < game->window_width && y >= 0 && y < game->window_height)
	{
		char	*pixel;

		pixel = game->img_addr + (y * game->line_length + x * (game->bpp / 8));
		*(unsigned int *)pixel = color;
	}
}

// Function to draw the floor
void	draw_floor(t_game *game)
{
	int		x;
	int		y;
	int		color;

	color = (game->config.f_r << 16) | (game->config.f_g << 8) | game->config.f_b;
	for (y = game->window_height / 2; y < game->window_height; y++)
	{
		for (x = 0; x < game->window_width; x++)
		{
			set_pixel(game, x, y, color);
		}
	}
}

// Function to update direction vectors based on player_angle
void update_direction(t_game *game)
{
    game->dir_x = cos(game->player_angle);
    game->dir_y = sin(game->player_angle);
    game->plane_x = -sin(game->player_angle) * 0.66;
    game->plane_y = cos(game->player_angle) * 0.66;
}

int key_press(int keycode, t_game *game)
{
    printf("Keycode: %d\n", keycode); // For debugging

    if (keycode == 65307) // ESC key (Example keycode)
        close_window(game);

    double move_speed = MOVE_SPEED;
    double new_x, new_y;

    if (keycode == 119) // W key - Move forward (Example keycode)
    {
        new_x = game->player_x + game->dir_x * move_speed;
        new_y = game->player_y + game->dir_y * move_speed;
        if (game->config.map[(int)(new_y)][(int)(game->player_x)] != '1')
            game->player_y = new_y;
        if (game->config.map[(int)(game->player_y)][(int)(new_x)] != '1')
            game->player_x = new_x;
    }
    else if (keycode == 115) // S key - Move backward (Example keycode)
    {
        new_x = game->player_x - game->dir_x * move_speed;
        new_y = game->player_y - game->dir_y * move_speed;
        if (game->config.map[(int)(new_y)][(int)(game->player_x)] != '1')
            game->player_y = new_y;
        if (game->config.map[(int)(game->player_y)][(int)(new_x)] != '1')
            game->player_x = new_x;
    }
    else if (keycode == 97) // A key - Strafe left (Example keycode)
    {
        new_x = game->player_x - game->plane_x * move_speed;
        new_y = game->player_y - game->plane_y * move_speed;
        if (game->config.map[(int)(new_y)][(int)(game->player_x)] != '1')
            game->player_y = new_y;
        if (game->config.map[(int)(game->player_y)][(int)(new_x)] != '1')
            game->player_x = new_x;
    }
    else if (keycode == 100) // D key - Strafe right (Example keycode)
    {
        new_x = game->player_x + game->plane_x * move_speed;
        new_y = game->player_y + game->plane_y * move_speed;
        if (game->config.map[(int)(new_y)][(int)(game->player_x)] != '1')
            game->player_y = new_y;
        if (game->config.map[(int)(game->player_y)][(int)(new_x)] != '1')
            game->player_x = new_x;
    }
    else if (keycode == 65361) // Left Arrow - Rotate view left (Example keycode)
    {
        game->player_angle -= ROT_SPEED;
        update_direction(game);
    }
    else if (keycode == 65363) // Right Arrow - Rotate view right (Example keycode)
    {
        game->player_angle += ROT_SPEED;
        update_direction(game);
    }

    // Ensure angle remains within [0, 2 * PI]
    if (game->player_angle < 0)
        game->player_angle += 2 * M_PI;
    else if (game->player_angle >= 2 * M_PI)
        game->player_angle -= 2 * M_PI;

    // Re-render the scene after movement
    render_b(game);
    return (0);
}

void draw_ceiling(t_game *game)
{
	int x;
	int y;
	int color;

	color = (game->config.c_r << 16) | (game->config.c_g << 8) | game->config.c_b; // RGB to color
	for (y = 0; y < game->window_height / 2; y++)
	{
		for (x = 0; x < game->window_width; x++)
		{
			set_pixel(game, x, y, color);
		}
	}
}

// Render the floor and ceiling
void render_c(t_game *game)
{
	draw_ceiling(game); // Draw ceiling first
	draw_floor(game);   // Draw floor
	mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
}

void load_textures(t_game *game)
{
	game->textures[0] = mlx_xpm_file_to_image(game->mlx, game->config.no_texture, &game->tex_width, &game->tex_height);
	game->textures[1] = mlx_xpm_file_to_image(game->mlx, game->config.so_texture, &game->tex_width, &game->tex_height);
	game->textures[2] = mlx_xpm_file_to_image(game->mlx, game->config.we_texture, &game->tex_width, &game->tex_height);
	game->textures[3] = mlx_xpm_file_to_image(game->mlx, game->config.ea_texture, &game->tex_width, &game->tex_height);

	for (int i = 0; i < 4; i++)
	{
		if (!game->textures[i])
		{
			write_error("Error: Failed to load a texture\n");
			exit_program(game);
		}
		game->tex_data[i] = mlx_get_data_addr(game->textures[i], &game->tex_bpp, &game->tex_line_length, &game->tex_endian);
	}
}

void draw_wall(t_game *game, int x, int start, int end, int tex_id, int tex_x)
{
	double tex_step = 1.0 * game->tex_height / (end - start);
	double tex_pos = (start - game->window_height / 2 + (end - start) / 2) * tex_step;

	for (int y = start; y < end; y++)
	{
		int tex_y = (int)tex_pos & (game->tex_height - 1);
		tex_pos += tex_step;
		char *tex_pixel = game->tex_data[tex_id] + (tex_y * game->tex_line_length + tex_x * (game->tex_bpp / 8));
		int color = *(unsigned int *)tex_pixel;
		set_pixel(game, x, y, color);
	}
}

void render_walls(t_game *game)
{
    int map_width = game->config.map_width;
    int map_height = game->config.map_height;

    for (int x = 0; x < game->window_width; x++)
    {
        double camera_x = 2 * x / (double)game->window_width - 1;
        double ray_dir_x = game->dir_x + game->plane_x * camera_x;
        double ray_dir_y = game->dir_y + game->plane_y * camera_x;

        int map_x = (int)game->player_x;
        int map_y = (int)game->player_y;

        double side_dist_x;
        double side_dist_y;

        double delta_dist_x = fabs(1 / ray_dir_x);
        double delta_dist_y = fabs(1 / ray_dir_y);
        double perp_wall_dist;

        int step_x;
        int step_y;

        int hit = 0;
        int side;

        if (ray_dir_x < 0)
        {
            step_x = -1;
            side_dist_x = (game->player_x - map_x) * delta_dist_x;
        }
        else
        {
            step_x = 1;
            side_dist_x = (map_x + 1.0 - game->player_x) * delta_dist_x;
        }
        if (ray_dir_y < 0)
        {
            step_y = -1;
            side_dist_y = (game->player_y - map_y) * delta_dist_y;
        }
        else
        {
            step_y = 1;
            side_dist_y = (map_y + 1.0 - game->player_y) * delta_dist_y;
        }

        while (hit == 0)
        {
            if (side_dist_x < side_dist_y)
            {
                side_dist_x += delta_dist_x;
                map_x += step_x;
                side = 0;
            }
            else
            {
                side_dist_y += delta_dist_y;
                map_y += step_y;
                side = 1;
            }
            // Check if ray has gone outside the bounds of the map
            if (map_x < 0 || map_x >= map_width || map_y < 0 || map_y >= map_height)
            {
                hit = 1; // Stop if outside map bounds
            }
            else if (game->config.map[map_y][map_x] == '1')
            {
                hit = 1;
            }
        }

        if (side == 0)
            perp_wall_dist = (map_x - game->player_x + (1 - step_x) / 2) / ray_dir_x;
        else
            perp_wall_dist = (map_y - game->player_y + (1 - step_y) / 2) / ray_dir_y;

        int line_height = (int)(game->window_height / perp_wall_dist);

        int draw_start = -line_height / 2 + game->window_height / 2;
        if (draw_start < 0)
            draw_start = 0;
        int draw_end = line_height / 2 + game->window_height / 2;
        if (draw_end >= game->window_height)
            draw_end = game->window_height - 1;

        int tex_id;
        if (side == 0)
        {
            if (ray_dir_x > 0)
                tex_id = 0; // East wall
            else
                tex_id = 1; // West wall
        }
        else
        {
            if (ray_dir_y > 0)
                tex_id = 2; // South wall
            else
                tex_id = 3; // North wall
        }

        double wall_x;
        if (side == 0)
            wall_x = game->player_y + perp_wall_dist * ray_dir_y;
        else
            wall_x = game->player_x + perp_wall_dist * ray_dir_x;
        wall_x -= floor(wall_x);

        int tex_x = (int)(wall_x * (double)game->tex_width);
        if ((side == 0 && ray_dir_x > 0) || (side == 1 && ray_dir_y < 0))
            tex_x = game->tex_width - tex_x - 1;

        draw_wall(game, x, draw_start, draw_end, tex_id, tex_x);
    }
}

void render_b(t_game *game)
{
	draw_ceiling(game);    // Draw ceiling
	draw_floor(game);      // Draw floor
	render_walls(game);    // Draw walls
	mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
}

int game_loop(t_game *game)
{
	// Update game state if necessary
	// For now, just render the scene
	render_b(game);
	return (0);
}

int main(int ac, char **av)
{
	t_game game;

	if (ac == 2)
	{
		file_name(av[1]);
		game.config = check_map(av[1]);
		print_config(game.config);

		game.player_angle = M_PI / 2; // Initialize the player's angle to face forward

		// Initialize textures and game variables
		initialize_game_window(&game);
		load_textures(&game);

		// Render the initial frame
		render_b(&game);

		// Hook for key press events
		mlx_hook(game.win, KeyPress, KeyPressMask, key_press, &game);

		// Hook for closing the window
		mlx_hook(game.win, 17, 0, close_window, &game);

		// Start MiniLibX loop
		mlx_loop(game.mlx);
	}
	else
		write_error("Error: Wrong input\n");
	return (0);
}
