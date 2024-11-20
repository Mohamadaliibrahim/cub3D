/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checking_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 20:35:35 by mohamibr          #+#    #+#             */
/*   Updated: 2024/11/20 20:19:06 by mohamibr         ###   ########.fr       */
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

int	parse_color_values(char *str, int *red, int *green, int *blue)
{
	char	**colors;
	char	*r_str;
	char	*g_str;
	char	*b_str;

	colors = ft_split(str, ',');
	if (!colors || !colors[0] || !colors[1] || !colors[2] || colors[3])
	{
		two_d_free(colors);
		return (0);
	}
	// Trim spaces from each color component
	r_str = ft_strtrim(colors[0], " \t");
	g_str = ft_strtrim(colors[1], " \t");
	b_str = ft_strtrim(colors[2], " \t");
	if (!r_str || !g_str || !b_str)
	{
		free(r_str);
		free(g_str);
		free(b_str);
		two_d_free(colors);
		return (0);
	}
	*red = ft_atoi(r_str);
	*green = ft_atoi(g_str);
	*blue = ft_atoi(b_str);
	free(r_str);
	free(g_str);
	free(b_str);
	two_d_free(colors);
	if (*red < 0 || *red > 255 || *green < 0 || *green > 255 || *blue < 0
		|| *blue > 255)
		return (0);
	return (1);
}

void	parse_color_line(char *line, t_config *confige)
{
	char	*trimmed_line;
	char	*identifier;
	char	*color_values;
	int		red, green, blue;

	// Trim leading and trailing spaces
	trimmed_line = ft_strtrim(line, " \t");
	if (!trimmed_line)
	{
		write_error("Error: Memory allocation failed\n");
		free_config(confige);
		exit(EXIT_FAILURE);
	}

	// Find the first space character to split the identifier and the values
	char *space_ptr = ft_strchr(trimmed_line, ' ');
	if (!space_ptr)
	{
		write_error("Error: Invalid color line format\n");
		free(trimmed_line);
		free_config(confige);
		exit(EXIT_FAILURE);
	}

	// Extract the identifier
	int identifier_len = space_ptr - trimmed_line;
	identifier = ft_substr(trimmed_line, 0, identifier_len);
	if (!identifier)
	{
		write_error("Error: Memory allocation failed\n");
		free(trimmed_line);
		free_config(confige);
		exit(EXIT_FAILURE);
	}

	// Extract and trim the color values string
	color_values = ft_strtrim(space_ptr + 1, " \t");
	if (!color_values)
	{
		write_error("Error: Memory allocation failed\n");
		free(identifier);
		free(trimmed_line);
		free_config(confige);
		exit(EXIT_FAILURE);
	}

	// Parse the color values
	if (!parse_color_values(color_values, &red, &green, &blue))
	{
		write_error("Error: Invalid color values\n");
		free(color_values);
		free(identifier);
		free(trimmed_line);
		free_config(confige);
		exit(EXIT_FAILURE);
	}

	// Assign the color values
	if (ft_strcmp(identifier, "F") == 0)
	{
		if (confige->floor_color != -1)
		{
			write_error("Error: Duplicate floor color definition\n");
			free(color_values);
			free(identifier);
			free(trimmed_line);
			free_config(confige);
			exit(EXIT_FAILURE);
		}
		confige->floor_color = (red << 16) | (green << 8) | blue;
	}
	else if (ft_strcmp(identifier, "C") == 0)
	{
		if (confige->ceiling_color != -1)
		{
			write_error("Error: Duplicate ceiling color definition\n");
			free(color_values);
			free(identifier);
			free(trimmed_line);
			free_config(confige);
			exit(EXIT_FAILURE);
		}
		confige->ceiling_color = (red << 16) | (green << 8) | blue;
	}
	else
	{
		write_error("Error: Unknown color identifier\n");
		free(color_values);
		free(identifier);
		free(trimmed_line);
		free_config(confige);
		exit(EXIT_FAILURE);
	}

	// Clean up
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
		write_error("Error: Memory allocation failed\n");
		exit(1);
	}
	new_node->line = ft_strdup(line);
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

void	parse_texture_line(char *line, t_config *confige)
{
	char	**parts;
	char	*texture_path;
	int		fd;
	int		len;

	parts = ft_split(line, ' ');
	if (!parts || !parts[0] || !parts[1] || parts[2])
	{
		write_error("Error: Invalid texture line format\n");
		two_d_free(parts);
		free_config(confige);
		exit(EXIT_FAILURE);
	}
	texture_path = ft_strdup(parts[1]);
	if (!texture_path)
	{
		write_error("Error: Memory allocation failed\n");
		two_d_free(parts);
		free_config(confige);
		exit(EXIT_FAILURE);
	}
	// Check if the texture file exists
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
	// Check if the texture file has .xpm extension
	len = ft_strlen(texture_path);
	if (len < 4 || ft_strcmp(&texture_path[len - 4], ".xpm") != 0)
	{
		write_error("Error: Texture file is not a .xpm file\n");
		free(texture_path);
		two_d_free(parts);
		free_config(confige);
		exit(EXIT_FAILURE);
	}
	// Check for duplicates before assignment
	if (ft_strcmp(parts[0], "NO") == 0)
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
	else if (ft_strcmp(parts[0], "SO") == 0)
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
	else if (ft_strcmp(parts[0], "WE") == 0)
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
	else if (ft_strcmp(parts[0], "EA") == 0)
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

void	open_map_and_else(char *av, t_config *confige)
{
	int		fd;
	char	*line;
	char	*trimmed_line;
	int		map_started;

	map_started = 0;
	ft_memset(confige, 0, sizeof(t_config));
	confige->floor_color = -1;
	confige->ceiling_color = -1;
	fd = open(av, O_RDONLY);
	trimmed_line = NULL;
	if (fd == -1)
	{
		write_error("Error: Cannot open map file\n");
		exit(EXIT_FAILURE);
	}
	while ((line = get_next_line(fd)) != NULL)
	{
		trimmed_line = ft_strtrim(line, " \t\n");
		free(line);
		if (trimmed_line && *trimmed_line != '\0')
		{
			if (is_map_line(trimmed_line))
			{
				map_started = 1;
				add_to_map(trimmed_line, confige);
			}
			else if (!map_started)
			{
				check_type(&trimmed_line, confige);
			}
			else
			{
				write_error("Error: Invalid line in map file after map data\n");
				free(trimmed_line);
				free_config(confige);
				exit(EXIT_FAILURE);
			}
		}
		free(trimmed_line);
	}
	close(fd);
}
