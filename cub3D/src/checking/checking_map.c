/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checking_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 20:35:35 by mohamibr          #+#    #+#             */
/*   Updated: 2024/12/02 18:46:01 by mohamibr         ###   ########.fr       */
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

// void	two_d_free(char **str)
// {
// 	int	i;

// 	if (!str)
// 		return ;
// 	i = 0;
// 	while (str[i])
// 	{
// 		free(str[i]);
// 		i++;
// 	}
// 	free(str);
// }

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

// int	parse_color_values(char *str, int rgb[3])
// {
// 	char	**colors;
// 	int		res0;
// 	int		res1;
// 	int		res2;

// 	colors = ft_split(str, ',');
// 	if (!colors)
// 		return (0);
// 	if (!colors[0] || !colors[1] || !colors[2] || colors[3])
// 	{
// 		two_d_free(colors);
// 		return (0);
// 	}
// 	res0 = trim_and_convert_color(colors[0], &rgb[0]);
// 	res1 = trim_and_convert_color(colors[1], &rgb[1]);
// 	res2 = trim_and_convert_color(colors[2], &rgb[2]);
// 	two_d_free(colors);
// 	return (res0 && res1 && res2);
// }

// void	color_check(t_config *confige, char *id, int rgb[3], char *line)
// {
// 	if (confige->f_r != -1)
// 	{
// 		write_error("Error: Duplicate floor color definition\n");
// 		free(id);
// 		free(line);
// 		free_config(confige);
// 		exit(EXIT_FAILURE);
// 	}
// 	confige->f_r = rgb[0];
// 	confige->f_g = rgb[1];
// 	confige->f_b = rgb[2];
// }

// void	assign_color(char *id, int rgb[3], t_config *confige, char *line)
// {
// 	if (ft_strcmp(id, "F") == 0)
// 		color_check(confige, id, rgb, line);
// 	else if (ft_strcmp(id, "C") == 0)
// 	{
// 		if (confige->c_r != -1)
// 		{
// 			write_error("Error: Duplicate ceiling color definition\n");
// 			free(id);
// 			free(line);
// 			free_config(confige);
// 			exit(EXIT_FAILURE);
// 		}
// 		confige->c_r = rgb[0];
// 		confige->c_g = rgb[1];
// 		confige->c_b = rgb[2];
// 	}
// 	else
// 	{
// 		write_error("Error: Unknown color id\n");
// 		free(id);
// 		free(line);
// 		free_config(confige);
// 		exit(EXIT_FAILURE);
// 	}
// }

// void	value_check(char **id, char *line, char **color, t_config *confige)
// {
// 	if (!*color)
// 	{
// 		write_error("Error: Memory allocation failed\n");
// 		free(*id);
// 		free(line);
// 		free_config(confige);
// 		exit(EXIT_FAILURE);
// 	}
// }

// void	parse_values(char *line, char **id, char **color, t_config *confige)
// {
// 	char	*space_ptr;
// 	int		id_len;

// 	space_ptr = ft_strchr(line, ' ');
// 	if (!space_ptr)
// 	{
// 		write_error("Error: Invalid color line format\n");
// 		free(line);
// 		free_config(confige);
// 		exit(EXIT_FAILURE);
// 	}
// 	id_len = space_ptr - line;
// 	*id = ft_substr(line, 0, id_len);
// 	if (!*id)
// 	{
// 		write_error("Error: Memory allocation failed\n");
// 		free(line);
// 		free_config(confige);
// 		exit(EXIT_FAILURE);
// 	}
// 	*color = ft_strtrim(space_ptr + 1, " \t");
// 	value_check(id, line, color, confige);
// }

// void	error_msg(t_config *confige)
// {
// 	write_error("Error: Memory allocation failed\n");
// 	free_config(confige);
// 	exit(EXIT_FAILURE);
// }

// void	parse_color_line(char *line, t_config *confige)
// {
// 	char	*trimmed_line;
// 	char	*identifier;
// 	char	*color_values;
// 	int		rgb[3];

// 	trimmed_line = ft_strtrim(line, " \t");
// 	if (!trimmed_line)
// 		error_msg(confige);
// 	parse_values(trimmed_line, &identifier, &color_values, confige);
// 	if (!parse_color_values(color_values, rgb))
// 	{
// 		write_error("Error: Invalid color values\n");
// 		free(color_values);
// 		free(identifier);
// 		free(trimmed_line);
// 		free_config(confige);
// 		exit(EXIT_FAILURE);
// 	}
// 	assign_color(identifier, rgb, confige, trimmed_line);
// 	free(color_values);
// 	free(identifier);
// 	free(trimmed_line);
// }

// void	map_err(t_map_node *new_node)
// {
// 	if (!new_node->line)
// 	{
// 		write_error("Error: Memory allocation failed for map line\n");
// 		free(new_node);
// 		exit(EXIT_FAILURE);
// 	}
// }

// void	add_to_map(char *line, t_config *config)
// {
// 	t_map_node	*new_node;
// 	t_map_node	*current;

// 	new_node = malloc(sizeof(t_map_node));
// 	if (!new_node)
// 	{
// 		write_error("Error: Memory allocation failed for map node\n");
// 		exit(EXIT_FAILURE);
// 	}
// 	new_node->line = ft_strdup(line);
// 	map_err(new_node);
// 	new_node->next = NULL;
// 	if (!config->map_list)
// 		config->map_list = new_node;
// 	else
// 	{
// 		current = config->map_list;
// 		while (current->next)
// 			current = current->next;
// 		current->next = new_node;
// 	}
// }

// void	texture_err(int len, char *texture_path,
// 			char **parts, t_config *confige)
// {
// 	if (len < 4 || ft_strcmp(&texture_path[len - 4], ".xpm") != 0)
// 	{
// 		write_error("Error: Texture file is not a .xpm file\n");
// 		free(texture_path);
// 		two_d_free(parts);
// 		free_config(confige);
// 		exit(EXIT_FAILURE);
// 	}
// }

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

// void	assign_err(char *path, char **parts, t_config *confige)
// {
// 	write_error("Error: Duplicate NO texture definition\n");
// 	free(path);
// 	two_d_free(parts);
// 	free_config(confige);
// 	exit(EXIT_FAILURE);
// }

// void	assign_north(char *path, char **parts, t_config *confige)
// {
// 	if (confige->no_texture != NULL)
// 		assign_err(path, parts, confige);
// 	confige->no_texture = path;
// }

// void	assign(char *id, char *path, t_config *confige, char **parts)
// {
// 	if (ft_strcmp(id, "NO") == 0)
// 	{
// 		assign_north(path, parts, confige);
// 	}
// 	else if (ft_strcmp(id, "SO") == 0)
// 	{
// 		if (confige->so_texture != NULL)
// 			assign_err(path, parts, confige);
// 		confige->so_texture = path;
// 	}
// 	else if (ft_strcmp(id, "WE") == 0)
// 	{
// 		if (confige->we_texture != NULL)
// 			assign_err(path, parts, confige);
// 		confige->we_texture = path;
// 	}
// 	else if (ft_strcmp(id, "EA") == 0)
// 	{
// 		if (confige->ea_texture != NULL)
// 			assign_err(path, parts, confige);
// 		confige->ea_texture = path;
// 	}
// 	else
// 		assign_err(path, parts, confige);
// }

// void	parse_texture_line(char *line, t_config *confige)
// {
// 	char	**parts;
// 	char	*texture_path;

// 	parts = ft_split(line, ' ');
// 	if (!parts || !parts[0] || !parts[1] || parts[2])
// 	{
// 		write_error("Error: Invalid texture line format\n");
// 		two_d_free(parts);
// 		free_config(confige);
// 		exit(EXIT_FAILURE);
// 	}
// 	texture_path = get_texture_path(parts, confige);
// 	assign(parts[0], texture_path, confige, parts);
// 	two_d_free(parts);
// }

// void	check_type(char **trimmed_line, t_config *config)
// {
// 	if (is_texture_line(*trimmed_line))
// 		parse_texture_line(*trimmed_line, config);
// 	else if (is_color_line(*trimmed_line))
// 		parse_color_line(*trimmed_line, config);
// 	else
// 	{
// 		write_error("Error: Invalid line in map file\n");
// 		free(*trimmed_line);
// 		free_config(config);
// 		exit(EXIT_FAILURE);
// 	}
// }

// int	process_line(char *line, t_config *confige, int *m_start, int *m_end)
// {
// 	if (*line != '\0')
// 	{
// 		if (is_map_line(line))
// 		{
// 			if (*m_end)
// 			{
// 				write_error("Error: Invalid content after map data\n");
// 				return (free(line), -1);
// 			}
// 			*m_start = 1;
// 			add_to_map(line, confige);
// 		}
// 		else if (!*m_start)
// 			check_type(&line, confige);
// 		else
// 		{
// 			*m_end = 1;
// 			write_error("Error: Invalid line in map file after map data\n");
// 			free(line);
// 			return (-1);
// 		}
// 	}
// 	else if (*m_start)
// 		*m_end = 1;
// 	return (free(line), 0);
// }

// 	*map_started = 0;
// 	*map_ended = 0;
// 	ft_memset(confige, 0, sizeof(t_config));
// 	confige->f_r = -1;
// 	confige->c_r = -1;
// 	fd = open(av, O_RDONLY);
// 	if (fd == -1)
// 	{
// 		write_error("Error: Cannot open map file\n");
// 		exit(EXIT_FAILURE);
// 	}
// 	return (fd);
// }

// void	open_map_error(t_config *confige, int fd)
// {
// 	write_error("Error: Memory allocation failed for trimmed line\n");
// 	free_config(confige);
// 	close(fd);
// 	exit(EXIT_FAILURE);
// }

// void	open_map_and_else(char *av, t_config *confige)
// {
// 	int		fd;
// 	char	*line;
// 	int		map_started;
// 	int		map_ended;
// 	char	*trimmed_line;

// 	fd = ini_open_mp(&map_started, &map_ended, confige, av);
// 	line = get_next_line(fd);
// 	while (line != NULL)
// 	{
// 		trimmed_line = ft_strtrim(line, " \t\n");
// 		free(line);
// 		if (!trimmed_line)
// 		{
// 			open_map_error(confige, fd);
// 		}
// 		if (process_line(trimmed_line, confige, &map_started, &map_ended) == -1)
// 		{
// 			free_config(confige);
// 			close(fd);
// 			exit(EXIT_FAILURE);
// 		}
// 		line = get_next_line(fd);
// 	}
// 	close(fd);
// }
