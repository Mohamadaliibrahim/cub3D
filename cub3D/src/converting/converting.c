/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   converting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 20:35:35 by mohamibr          #+#    #+#             */
/*   Updated: 2024/11/29 12:19:06 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

void	convert_map_list_to_array(t_config *config)
{
	t_map_node	*current;
	t_map_node	*temp;
	int			i;
	int			count;
	int			max_width;
	int			len;
	int			j;

	max_width = 0;
	if (!config->map_list)
	{
		write_error("Error: Map list is empty\n");
		exit(1);
	}
	count = 0;
	current = config->map_list;
	while (current)
	{
		if (!current->line)
		{
			write_error("Error: Map line is NULL\n");
			exit(1);
		}
		len = ft_strlen(current->line);
		if (len > max_width)
			max_width = len;
		count++;
		current = current->next;
	}
	config->map = malloc(sizeof(char *) * (count + 1));
	if (!config->map)
	{
		write_error("Error: Memory allocation failed for map array\n");
		exit(1);
	}
	current = config->map_list;
	i = 0;
	while (current)
	{
		if (!current->line)
		{
			write_error("Error: Map line is NULL\n");
			exit(1);
		}
		config->map[i] = malloc(max_width + 1);
		if (!config->map[i])
		{
			write_error("Error: Memory allocation failed for map line\n");
			exit(1);
		}
		len = ft_strlen(current->line);
		strcpy(config->map[i], current->line);
		for (j = len; j < max_width; j++)
			config->map[i][j] = '1';
		config->map[i][max_width] = '\0';
		temp = current;
		current = current->next;
		free(temp->line);
		free(temp);
		i++;
	}
	config->map[i] = NULL;
	config->map_list = NULL;
	config->map_height = i;
	config->map_width = max_width;
}
