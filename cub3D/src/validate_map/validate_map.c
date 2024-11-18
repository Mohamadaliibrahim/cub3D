/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 19:52:41 by mohamibr          #+#    #+#             */
/*   Updated: 2024/11/18 20:15:35 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

void	check_map_charecters(t_config *confige)
{
	int	i;
	int	j;

	i = 0;
	while(confige->map[i])
	{
		j = 0;
		while (confige->map[i][j])
		{
			if (!ft_strchr(" 012NSWE", confige->map[i][j]))
			{
				write_error("Error: Invalid charecter in map\n");
				free_config(confige);
				exit(1);
			}
			j++;
		}
		i++;
	}
}

void	check_player_position(t_config *confige)
{
	int	i;
	int	player_count;
	int	j;

	i = 0;
	player_count = 0;
	while (confige->map[i])
	{
		j = 0;
		while (confige->map[i][j])
		{
			if (ft_strchr("NWSE", confige->map[i][j]))
			{
				player_count++;
				confige->player_x = j;
				confige->player_y = i;
				confige->player_orientation = confige->map[i][j];
			}
			j++;
		}
		i++;
	}
	if (player_count == 0)
    {
        write_error("Error: No player starting position in map\n");
        free_config(confige);
        exit(EXIT_FAILURE);
    }
    else if (player_count > 1)
    {
        write_error("Error: Multiple player starting positions in map\n");
        free_config(confige);
        exit(EXIT_FAILURE);
    }
}
//ma ba2a ele 5el2 kml..
int	is_map_closed(char **map)
{
	int	i;
	int	height;
	int	j;
	int	width;

	i = 0;
	height = get_map_height(map);
	if (!is_line_close)
}

void	validate_map(t_config *confige)
{
	if (!confige->map)
	{
		write_error("Error: Mao is missing\n");
		free_config(confige);
		exit(1);
	}
	check_map_charcters(confige);
	check_player_position(confige);
	if (!is_map_closed(confige->map))
	{
		write_error("Error: Map is not properly closed\n");
		free_config(confige);
		exit(1);
	}
}