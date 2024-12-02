/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   value.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 18:28:42 by mohamibr          #+#    #+#             */
/*   Updated: 2024/12/02 22:36:32 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

void	error_msg(t_config *confige)
{
	write_error("Error: Memory allocation failed\n");
	free_config(confige);
	exit(EXIT_FAILURE);
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
		error_msg(confige);
		free_get_next_line();
	}
	parse_values(trimmed_line, &identifier, &color_values, confige);
	if (!parse_color_values(color_values, rgb))
	{
		write_error("Error: Invalid color values\n");
		free(color_values);
		free(identifier);
		free(trimmed_line);
		free(line);
		free_config(confige);
		free_get_next_line();
		exit(EXIT_FAILURE);
	}
	assign_color(identifier, rgb, confige, trimmed_line);
	free(color_values);
	free(identifier);
	free(trimmed_line);
}

void	map_err(t_map_node *new_node)
{
	if (!new_node->line)
	{
		write_error("Error: Memory allocation failed for map line\n");
		free(new_node);
		exit(EXIT_FAILURE);
	}
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
	map_err(new_node);
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

void	texture_err(int len, char *path, char **parts, t_config *confige)
{
	if (len < 4 || ft_strcmp(&path[len - 4], ".xpm") != 0)
	{
		write_error("Error: Texture file is not a .xpm file\n");
		free(path);
		two_d_free(parts);
		free_config(confige);
		exit(EXIT_FAILURE);
	}
}
