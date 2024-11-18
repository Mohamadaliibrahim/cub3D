/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checking_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 20:35:35 by mohamibr          #+#    #+#             */
/*   Updated: 2024/11/18 16:34:45 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

int	is_texture_line(char *line)
{
	if (ft_strncmp(line, "NO ", 3) == 0
		|| ft_strncmp(line, "SO ", 3) == 0
		|| ft_strncmp(line, "WE ", 3) == 0
		|| ft_strncmp(line, "EA ", 3) == 0)
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

void parse_color_line(char *line, t_config *confige)
{
	char	**parts;
	char	**colors;
	int		red;
	int		green;
	int		blue;

	parts = ft_split(line, ' ');
	if (!parts || !parts[0] || !parts[1])
	{
		write_error("Error: Invalid color line format \n");
		two_d_free(parts);
		free_config(confige);
		exit(EXIT_FAILURE);
	}

	colors = ft_split(parts[1], ',');
	if (!colors || !colors[0] || !colors[1] || !colors[2])
	{
		write_error("Error: Invalid color values\n");
		two_d_free(parts);
		two_d_free(colors);
		free_config(confige);
		exit(EXIT_FAILURE);
	}

	red = ft_atoi(colors[0]);
	green = ft_atoi(colors[1]);
	blue = ft_atoi(colors[2]);

	if (red < 0 || red > 255 || green < 0
		|| green > 255 || blue < 0 || blue > 255)
	{
		write_error("Error: Color values must be between 0 and 255\n");
		two_d_free(parts);
		two_d_free(colors);
		free_config(confige);
		exit(EXIT_FAILURE);
	}

	if (ft_strcmp(parts[0], "F") == 0)
		confige->floor_color = (red << 16) | (green << 8) | blue;
	else if (ft_strcmp(parts[0], "C") == 0)
		confige->ceiling_color = (red << 16) | (green << 8) | blue;
	else
	{
		write_error("Error: Unknown color identifier\n");
		two_d_free(parts);
		two_d_free(colors);
		free_config(confige);
		exit(EXIT_FAILURE);
	}

	two_d_free(colors);
	two_d_free(parts);
}


void	add_to_map(char *line, t_config *config)
{
	t_map_node	*new_node;
	t_map_node	*current;

	new_node = malloc(sizeof(t_map_node));
	if (!new_node)
	{
		write_error("Error: Memort=y allocation failed\n");
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

void parse_texture_line(char *line, t_config *confige)
{
	char	**parts;

	parts = ft_split(line, ' ');
	if (!parts || !parts[0] || !parts[1])
	{
		write_error("Error: Invalid texture line format \n");
		two_d_free(parts);
		free_config(confige);
		exit(EXIT_FAILURE);
	}

	if (ft_strcmp(parts[0], "NO") == 0)
		confige->no_texture = ft_strdup(parts[1]);
	else if (ft_strcmp(parts[0], "SO") == 0)
		confige->so_texture = ft_strdup(parts[1]);
	else if (ft_strcmp(parts[0], "WE") == 0)
		confige->we_texture = ft_strdup(parts[1]);
	else if (ft_strcmp(parts[0], "EA") == 0)
		confige->ea_texture = ft_strdup(parts[1]);
	else
	{
		write_error("Error: Unknown texture identifier\n");
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
	else if (is_map_line(*trimmed_line))
		add_to_map(*trimmed_line, config);
	else
		write_error("Error: Invalid line in map file\n");
}



void	open_map_and_else(char *av, t_config *confige)
{
	int			fd;
	char		*line;
	char		*trimmed_line;

	ft_memset(confige, 0, sizeof(t_config));
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
			check_type(&trimmed_line, confige);
		free(trimmed_line);
	}
	close(fd);
}

