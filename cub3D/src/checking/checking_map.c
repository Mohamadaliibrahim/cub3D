/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checking_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjoundi <mjoundi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 20:35:35 by mohamibr          #+#    #+#             */
/*   Updated: 2024/12/01 19:29:38 by mjoundi          ###   ########.fr       */
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

void	two_d_free(char **str)
{
	int	i;

	if (!str)
		return ;
	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
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

int	parse_color_values(char *str, int rgb[3])
{
	char	**colors;
	int		result;

	colors = ft_split(str, ',');
	if (!colors || !colors[0] || !colors[1] || !colors[2] || colors[3])
	{
		two_d_free(colors);
		return (0);
	}
	result = trim_and_convert_color(colors[0], &rgb[0])
		&& trim_and_convert_color(colors[1], &rgb[1])
		&& trim_and_convert_color(colors[2], &rgb[2]);
	two_d_free(colors);
	return (result);
}

static void	assign_color(char *identifier, int rgb[3],
			t_config *confige, char *trimmed_line)
{
	if (ft_strcmp(identifier, "F") == 0)
	{
		if (confige->f_r != -1)
		{
			write_error("Error: Duplicate floor color definition\n");
			free(identifier);
			free(trimmed_line);
			free_config(confige);
			exit(EXIT_FAILURE);
		}
		confige->f_r = rgb[0];
		confige->f_g = rgb[1];
		confige->f_b = rgb[2];
	}
	else if (ft_strcmp(identifier, "C") == 0)
	{
		if (confige->c_r != -1)
		{
			write_error("Error: Duplicate ceiling color definition\n");
			free(identifier);
			free(trimmed_line);
			free_config(confige);
			exit(EXIT_FAILURE);
		}
		confige->c_r = rgb[0];
		confige->c_g = rgb[1];
		confige->c_b = rgb[2];
	}
	else
	{
		write_error("Error: Unknown color identifier\n");
		free(identifier);
		free(trimmed_line);
		free_config(confige);
		exit(EXIT_FAILURE);
	}
}

static void	parse_identifier_and_values(char *trimmed_line,
				char **identifier, char **color_values,
				t_config *confige)
{
	char	*space_ptr;
	int		identifier_len;

	space_ptr = ft_strchr(trimmed_line, ' ');
	if (!space_ptr)
	{
		write_error("Error: Invalid color line format\n");
		free(trimmed_line);
		free_config(confige);
		exit(EXIT_FAILURE);
	}
	identifier_len = space_ptr - trimmed_line;
	*identifier = ft_substr(trimmed_line, 0, identifier_len);
	if (!*identifier)
	{
		write_error("Error: Memory allocation failed\n");
		free(trimmed_line);
		free_config(confige);
		exit(EXIT_FAILURE);
	}
	*color_values = ft_strtrim(space_ptr + 1, " \t");
	if (!*color_values)
	{
		write_error("Error: Memory allocation failed\n");
		free(*identifier);
		free(trimmed_line);
		free_config(confige);
		exit(EXIT_FAILURE);
	}
}

void	parse_color_line(char *line, t_config *confige)
{
	char	*trimmed_line;
	char	*identifier;
	char	*color_values;
	int		rgb[3];

	trimmed_line = ft_strtrim(line, " \t");
	if (!trimmed_line)
	{
		write_error("Error: Memory allocation failed\n");
		free_config(confige);
		exit(EXIT_FAILURE);
	}
	parse_identifier_and_values(trimmed_line, &identifier,
		&color_values, confige);
	if (!parse_color_values(color_values, rgb))
	{
		write_error("Error: Invalid color values\n");
		free(color_values);
		free(identifier);
		free(trimmed_line);
		free_config(confige);
		exit(EXIT_FAILURE);
	}
	assign_color(identifier, rgb, confige, trimmed_line);
	free(color_values);
	free(identifier);
	free(trimmed_line);
}

void	add_to_map(char *line, t_config *config)
{
	t_map_node	*new_node;
	t_map_node	*current;

	new_node = malloc(sizeof(t_map_node));
	if (!new_node)
	{
		write_error("Error: Memory allocation failed for map node\n");
		exit(EXIT_FAILURE);
	}
	new_node->line = ft_strdup(line);
	if (!new_node->line)
	{
		write_error("Error: Memory allocation failed for map line\n");
		free(new_node);
		exit(EXIT_FAILURE);
	}
	new_node->next = NULL;
	if (!config->map_list)
		config->map_list = new_node;
	else
	{
		current = config->map_list;
		while (current->next)
			current = current->next;
		current->next = new_node;
	}
}

static char	*get_texture_path(char **parts, t_config *confige)
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
	if (len < 4 || ft_strcmp(&texture_path[len - 4], ".xpm") != 0)
	{
		write_error("Error: Texture file is not a .xpm file\n");
		free(texture_path);
		two_d_free(parts);
		free_config(confige);
		exit(EXIT_FAILURE);
	}
	return (texture_path);
}

static void	assign_texture(char *identifier, char *texture_path,
				t_config *confige, char **parts)
{
	if (ft_strcmp(identifier, "NO") == 0)
	{
		if (confige->no_texture != NULL)
		{
			write_error("Error: Duplicate NO texture definition\n");
			free(texture_path);
			two_d_free(parts);
			free_config(confige);
			exit(EXIT_FAILURE);
		}
		confige->no_texture = texture_path;
	}
	else if (ft_strcmp(identifier, "SO") == 0)
	{
		if (confige->so_texture != NULL)
		{
			write_error("Error: Duplicate SO texture definition\n");
			free(texture_path);
			two_d_free(parts);
			free_config(confige);
			exit(EXIT_FAILURE);
		}
		confige->so_texture = texture_path;
	}
	else if (ft_strcmp(identifier, "WE") == 0)
	{
		if (confige->we_texture != NULL)
		{
			write_error("Error: Duplicate WE texture definition\n");
			free(texture_path);
			two_d_free(parts);
			free_config(confige);
			exit(EXIT_FAILURE);
		}
		confige->we_texture = texture_path;
	}
	else if (ft_strcmp(identifier, "EA") == 0)
	{
		if (confige->ea_texture != NULL)
		{
			write_error("Error: Duplicate EA texture definition\n");
			free(texture_path);
			two_d_free(parts);
			free_config(confige);
			exit(EXIT_FAILURE);
		}
		confige->ea_texture = texture_path;
	}
	else
	{
		write_error("Error: Unknown texture identifier\n");
		free(texture_path);
		two_d_free(parts);
		free_config(confige);
		exit(EXIT_FAILURE);
	}
}

void	parse_texture_line(char *line, t_config *confige)
{
	char	**parts;
	char	*texture_path;

	parts = ft_split(line, ' ');
	if (!parts || !parts[0] || !parts[1] || parts[2])
	{
		write_error("Error: Invalid texture line format\n");
		two_d_free(parts);
		free_config(confige);
		exit(EXIT_FAILURE);
	}
	texture_path = get_texture_path(parts, confige);
	assign_texture(parts[0], texture_path, confige, parts);
	two_d_free(parts);
}

void	check_type(char **trimmed_line, t_config *config)
{
	if (is_texture_line(*trimmed_line))
		parse_texture_line(*trimmed_line, config);
	else if (is_color_line(*trimmed_line))
		parse_color_line(*trimmed_line, config);
	else
	{
		write_error("Error: Invalid line in map file\n");
		free(*trimmed_line);
		free_config(config);
		exit(EXIT_FAILURE);
	}
}

static void	process_line(char *trimmed_line, t_config *confige, int fd)
{
	if (*trimmed_line != '\0')
	{
		if (is_map_line(trimmed_line))
		{
			if (confige->map_ended)
			{
				write_error("Error: Invalid content after map data\n");
				free(trimmed_line);
				free_config(confige);
				close(fd);
				exit(EXIT_FAILURE);
			}
			confige->map_started = 1;
			add_to_map(trimmed_line, confige);
		}
		else if (!confige->map_started)
			check_type(&trimmed_line, confige);
		else
		{
			confige->map_ended = 1;
			write_error("Error: Invalid line in map file after map data\n");
			free(trimmed_line);
			free_config(confige);
			close(fd);
			exit(EXIT_FAILURE);
		}
	}
	else if (confige->map_started)
		confige->map_ended = 1;
	free(trimmed_line);
}

void	open_map_and_else(char *av, t_config *confige)
{
	int		fd;
	char	*line;
	char	*trimmed_line;

	confige->map_started = 0;
	confige->map_ended = 0;
	ft_memset(confige, 0, sizeof(t_config));
	confige->f_r = -1;
	confige->c_r = -1;
	fd = open(av, O_RDONLY);
	if (fd == -1)
	{
		write_error("Error: Cannot open map file\n");
		exit(EXIT_FAILURE);
	}
	line = get_next_line(fd);
	while (line != NULL)
	{
		trimmed_line = ft_strtrim(line, " \t\n");
		free(line);
		if (!trimmed_line)
		{
			write_error("Error: Memory allocation failed for trimmed line\n");
			free_config(confige);
			close(fd);
			exit(EXIT_FAILURE);
		}
		process_line(trimmed_line, confige, fd);
		line = get_next_line(fd);
	}
	close(fd);
}
