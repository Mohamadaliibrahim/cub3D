/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 18:27:51 by mohamibr          #+#    #+#             */
/*   Updated: 2024/12/02 18:41:57 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

int	parse_color_values(char *str, int rgb[3])
{
	char	**colors;
	int		res0;
	int		res1;
	int		res2;

	colors = ft_split(str, ',');
	if (!colors)
		return (0);
	if (!colors[0] || !colors[1] || !colors[2] || colors[3])
	{
		two_d_free(colors);
		return (0);
	}
	res0 = trim_and_convert_color(colors[0], &rgb[0]);
	res1 = trim_and_convert_color(colors[1], &rgb[1]);
	res2 = trim_and_convert_color(colors[2], &rgb[2]);
	two_d_free(colors);
	return (res0 && res1 && res2);
}

void	color_check(t_config *confige, char *id, int rgb[3], char *line)
{
	if (confige->f_r != -1)
	{
		write_error("Error: Duplicate floor color definition\n");
		free(id);
		free(line);
		free_config(confige);
		exit(EXIT_FAILURE);
	}
	confige->f_r = rgb[0];
	confige->f_g = rgb[1];
	confige->f_b = rgb[2];
}

void	assign_color(char *id, int rgb[3], t_config *confige, char *line)
{
	if (ft_strcmp(id, "F") == 0)
		color_check(confige, id, rgb, line);
	else if (ft_strcmp(id, "C") == 0)
	{
		if (confige->c_r != -1)
		{
			write_error("Error: Duplicate ceiling color definition\n");
			free(id);
			free(line);
			free_config(confige);
			exit(EXIT_FAILURE);
		}
		confige->c_r = rgb[0];
		confige->c_g = rgb[1];
		confige->c_b = rgb[2];
	}
	else
	{
		write_error("Error: Unknown color id\n");
		free(id);
		free(line);
		free_config(confige);
		exit(EXIT_FAILURE);
	}
}

void	value_check(char **id, char *line, char **color, t_config *confige)
{
	if (!*color)
	{
		write_error("Error: Memory allocation failed\n");
		free(*id);
		free(line);
		free_config(confige);
		exit(EXIT_FAILURE);
	}
}

void	parse_values(char *line, char **id, char **color, t_config *confige)
{
	char	*space_ptr;
	int		id_len;

	space_ptr = ft_strchr(line, ' ');
	if (!space_ptr)
	{
		write_error("Error: Invalid color line format\n");
		free(line);
		free_config(confige);
		exit(EXIT_FAILURE);
	}
	id_len = space_ptr - line;
	*id = ft_substr(line, 0, id_len);
	if (!*id)
	{
		write_error("Error: Memory allocation failed\n");
		free(line);
		free_config(confige);
		exit(EXIT_FAILURE);
	}
	*color = ft_strtrim(space_ptr + 1, " \t");
	value_check(id, line, color, confige);
}
