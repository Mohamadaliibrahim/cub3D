/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 19:52:41 by mohamibr          #+#    #+#             */
/*   Updated: 2024/11/22 00:58:50 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

void	check_map_charecters(t_config *confige)
{
	int	i;
	int	j;

	i = 0;
	while (confige->map[i])
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

int	is_line_closed(char *line)
{
	int	j;

	j = 0;
	while (line[j])
	{
		if (!ft_strchr("1 ", line[j]))
			return (0);
		j++;
	}
	return (1);
}

int	get_map_height(char **map)
{
	int	x;

	x = 0;
	while (map[x])
		x++;
	return (x);
}

int	is_surrounded_by_walls(char **map, int i, int j)
{
	int	height;
	// int	width;
	int	line_length;

	height = get_map_height(map);
	line_length = ft_strlen(map[i]);
	// Check up
	if (i == 0 || j >= ft_strlen(map[i - 1]) || map[i - 1][j] == ' ')
		return (0);
	// Check down
	if (i + 1 >= height || j >= ft_strlen(map[i + 1]) || map[i + 1][j] == ' ')
		return (0);
	// Check left
	if (j == 0 || map[i][j - 1] == ' ')
		return (0);
	// Check right
	if (j + 1 >= line_length || map[i][j + 1] == ' ')
		return (0);
	return (1);
}

int	is_map_closed(char **map)
{
	int	i;
	int	height;
	int	j;
	int	width;

	i = 0;
	height = get_map_height(map);
	if (!is_line_closed(map[0]) || !is_line_closed(map[height - 1]))
		return (0);
	while (map[i])
	{
		j = 0;
		width = ft_strlen(map[i]);
		if (width == 0)
			return (0);
		if (!ft_strchr("1 ", map[i][0]) || !ft_strchr("1 ", map[i][width - 1]))
			return (0);
		while (map[i][j])
		{
			if (map[i][j] == '0' || ft_strchr("NSEW", map[i][j]))
			{
				if (!is_surrounded_by_walls(map, i, j))
					return (0);
			}
			j++;
		}
		i++;
	}
	return (1);
}

void	validate_map(t_config *confige)
{
	check_map_charecters(confige);
	check_player_position(confige);
	if (!is_map_closed(confige->map))
	{
		write_error("Error: Map is not properly closed\n");
		free_config(confige);
		exit(1);
	}
}