/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checking_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 14:07:35 by mohamibr          #+#    #+#             */
/*   Updated: 2024/12/03 14:11:52 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

void	free_map_list(t_config *confige)
{
	t_map_node	*current;
	t_map_node	*temp;

	current = confige->map_list;
	while (current)
	{
		free(current->line);
		temp = current;
		current = current->next;
		free(temp);
	}
	confige->map_list = NULL;
}

void	error_free(char *line, t_config *confige)
{
	free(line);
	free_map_list(confige);
}
