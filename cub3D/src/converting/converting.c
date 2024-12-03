/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   converting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 20:35:35 by mohamibr          #+#    #+#             */
/*   Updated: 2024/12/03 03:20:51 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

static int	get_map_dimensions(t_config *config, int *max_width)
{
	t_map_node	*current;
	int			count;
	int			len;

	current = config->map_list;
	*max_width = 0;
	count = 0;
	while (current)
	{
		if (!current->line)
		{
			write_error("Error: Map line is NULL\n");
			exit(1);
		}
		len = ft_strlen(current->line);
		if (len > *max_width)
			*max_width = len;
		count++;
		current = current->next;
	}
	return (count);
}

static void	fill_map_line(char *map_line, char *source_line, int max_width)
{
	int	len;
	int	j;

	len = ft_strlen(source_line);
	ft_strcpy(map_line, source_line);
	j = len;
	while (j < max_width)
	{
		map_line[j] = '1';
		j++;
	}
	map_line[max_width] = '\0';
}

void	check_filling(t_map_node *current, t_config *config, int i, int width)
{
	if (!current->line)
	{
		write_error("Error: Map line is NULL\n");
		exit(1);
	}
	config->map[i] = malloc(sizeof(char) * (width + 1));
	if (!config->map[i])
	{
		write_error("Error: Memory allocation failed for map line\n");
		exit(1);
	}
}

static void	fill_map_array(t_config *config, int max_width)
{
	t_map_node	*current;
	t_map_node	*temp;
	int			i;

	current = config->map_list;
	i = 0;
	while (current)
	{
		check_filling(current, config, i, max_width);
		fill_map_line(config->map[i], current->line, max_width);
		temp = current;
		current = current->next;
		free(temp->line);
		free(temp);
		i++;
	}
	config->map[i] = NULL;
	config->map_list = NULL;
}

void	convert_map_list_to_array(t_config *config)
{
	int	count;
	int	max_width;

	if (!config->map_list)
	{
		write_error("Error: Map list is empty\n");
		free_config(config);//3th pic
		exit(1);
	}
	count = get_map_dimensions(config, &max_width);
	config->map = malloc(sizeof(char *) * (count + 1));
	if (!config->map)
	{
		write_error("Error: Memory allocation failed for map array\n");
		exit(1);
	}
	fill_map_array(config, max_width);
	config->map_height = count;
	config->map_width = max_width;
}
