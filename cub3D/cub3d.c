/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjoundi <mjoundi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 23:16:10 by marvin            #+#    #+#             */
/*   Updated: 2024/12/02 19:46:44 by mjoundi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/cub3d.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int	main(int ac, char **av)
{
	t_game	game;

	if (ac == 2)
	{
		file_name(av[1]);
		game.config = check_map(av[1]);
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
