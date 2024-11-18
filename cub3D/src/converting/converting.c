/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   converting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 20:35:35 by mohamibr          #+#    #+#             */
/*   Updated: 2024/11/18 15:40:47 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

void	convert_map_list_to_array(t_config *config)
{
	t_map_node	*current;
	t_map_node	*temp;
	int			i;
	int			count;

	count = 0;
	temp = NULL;
	current = config->map_list;
	while (current)
	{
		count++;
		current = current->next;
	}
	config->map = malloc(sizeof(char *) * (count + 1));
	if (!config->map)
	{
		write_error("Error: Memory allocation failed\n");
		exit(1);
	}
	current = config->map_list;
	i = 0;
	while (current)
	{
		config->map[i] = current->line;
		temp = current;
		current = current->next;
		free(temp);
		i++;
	}
	config->map[i] = NULL;
	config->map_list = NULL;
}
