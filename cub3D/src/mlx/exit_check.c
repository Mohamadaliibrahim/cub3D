/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_check.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjoundi <mjoundi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 19:37:56 by mjoundi           #+#    #+#             */
/*   Updated: 2024/12/02 19:49:38 by mjoundi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

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
