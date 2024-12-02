/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 18:47:56 by mohamibr          #+#    #+#             */
/*   Updated: 2024/12/02 18:49:30 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

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

int	close_check(int *i, char **map, int *j)
{
	while (map[*i][*j])
	{
		if (map[*i][*j] == '0' || ft_strchr("NSEW", map[*i][*j]))
		{
			if (!is_surrounded_by_walls(map, *i, *j))
				return (0);
		}
		(*j)++;
	}
	return (1);
}

int	is_surrounded_by_walls(char **map, int i, int j)
{
	int	height;
	int	line_length;

	height = get_map_height(map);
	line_length = ft_strlen(map[i]);
	if (i == 0 || j >= ft_strlen(map[i - 1]) || map[i - 1][j] == ' ')
		return (0);
	if (i + 1 >= height || j >= ft_strlen(map[i + 1]) || map[i + 1][j] == ' ')
		return (0);
	if (j == 0 || map[i][j - 1] == ' ')
		return (0);
	if (j + 1 >= line_length || map[i][j + 1] == ' ')
		return (0);
	return (1);
}
