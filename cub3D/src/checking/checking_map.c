/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checking_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjoundi <mjoundi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 20:35:35 by mohamibr          #+#    #+#             */
/*   Updated: 2024/12/02 19:18:11 by mjoundi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

int	is_texture_line(char *line)
{
	if (ft_strncmp(line, "NO ", 3) == 0 || ft_strncmp(line, "SO ", 3) == 0
		|| ft_strncmp(line, "WE ", 3) == 0 || ft_strncmp(line, "EA ", 3) == 0)
		return (1);
	return (0);
}

int	is_color_line(char *line)
{
	if (!ft_strncmp(line, "F ", 2) || !ft_strncmp(line, "C ", 2))
		return (1);
	return (0);
}

int	is_map_line(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (!ft_strchr(" 0123456789NSEW", line[i]))
			return (0);
		i++;
	}
	return (1);
}

int	trim_and_convert_color(char *color_str, int *color_value)
{
	char	*trimmed_str;

	trimmed_str = ft_strtrim(color_str, " \t");
	if (!trimmed_str)
		return (0);
	*color_value = ft_atoi(trimmed_str);
	free(trimmed_str);
	if (*color_value < 0 || *color_value > 255)
		return (0);
	return (1);
}

char	*get_texture_path(char **parts, t_config *confige)
{
	char	*texture_path;
	int		fd;
	int		len;

	texture_path = ft_strdup(parts[1]);
	if (!texture_path)
	{
		write_error("Error: Memory allocation failed for texture path\n");
		two_d_free(parts);
		free_config(confige);
		exit(EXIT_FAILURE);
	}
	fd = open(texture_path, O_RDONLY);
	if (fd == -1)
	{
		write_error("Error: Texture file not found\n");
		free(texture_path);
		two_d_free(parts);
		free_config(confige);
		exit(EXIT_FAILURE);
	}
	close(fd);
	len = ft_strlen(texture_path);
	texture_err(len, texture_path, parts, confige);
	return (texture_path);
}
