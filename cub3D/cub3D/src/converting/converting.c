/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   converting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 20:35:35 by mohamibr          #+#    #+#             */
/*   Updated: 2024/11/22 00:59:41 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

void convert_map_list_to_array(t_config *config)
{
	t_map_node	*current;
	t_map_node	*temp;
	int			i;
	int			count;
	int			max_width = 0;

	// Step 1: Validate the map_list
	if (!config->map_list)
	{
		write_error("Error: Map list is empty\n");
		exit(1);
	}

	// Step 2: Count the number of nodes in the map_list and find max_width
	count = 0;
	current = config->map_list;
	while (current)
	{
		if (!current->line)
		{
			write_error("Error: Map line is NULL\n");
			exit(1);
		}
		int len = ft_strlen(current->line);
		if (len > max_width)
			max_width = len;
		count++;
		current = current->next;
	}

	// Step 3: Allocate memory for the map array
	config->map = malloc(sizeof(char *) * (count + 1));
	if (!config->map)
	{
		write_error("Error: Memory allocation failed for map array\n");
		exit(1);
	}

	// Step 4: Copy map lines, pad them, and free map_list nodes
	current = config->map_list;
	i = 0;
	while (current)
	{
		if (!current->line)
		{
			write_error("Error: Map line is NULL\n");
			exit(1);
		}
		// Allocate new line with max_width
		config->map[i] = malloc(max_width + 1);
		if (!config->map[i])
		{
			write_error("Error: Memory allocation failed for map line\n");
			exit(1);
		}

		// Copy the line into the map array and pad it
		int len = ft_strlen(current->line);
		strcpy(config->map[i], current->line);
		for (int j = len; j < max_width; j++)
			config->map[i][j] = '1'; // Pad with walls or spaces
		config->map[i][max_width] = '\0';

		// Free the current node and move to the next
		temp = current;
		current = current->next;
		free(temp->line); // Free the original line
		free(temp);       // Free the node itself
		i++;
	}
	config->map[i] = NULL;
	config->map_list = NULL; // Clear the original list
	config->map_height = i;
	config->map_width = max_width;
}
