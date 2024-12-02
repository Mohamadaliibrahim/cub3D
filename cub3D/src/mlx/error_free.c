/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjoundi <mjoundi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 19:32:13 by mjoundi           #+#    #+#             */
/*   Updated: 2024/12/02 19:49:35 by mjoundi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

void	free_config_1(t_config *config)
{
	if (config->no_texture)
	{
		free(config->no_texture);
		config->no_texture = NULL;
	}
	if (config->so_texture)
	{
		free(config->so_texture);
		config->so_texture = NULL;
	}
	if (config->we_texture)
	{
		free(config->we_texture);
		config->we_texture = NULL;
	}
	if (config->ea_texture)
	{
		free(config->ea_texture);
		config->ea_texture = NULL;
	}
}

void	free_config(t_config *config)
{
	int	i;

	free_config_1(config);
	if (config->map)
	{
		i = 0;
		while (config->map[i])
		{
			free(config->map[i]);
			config->map[i] = NULL;
			i++;
		}
		free(config->map);
		config->map = NULL;
	}
}

void	write_error(char *str)
{
	while (*str)
		write(2, str++, 1);
}

void	validate_config(t_config *confige)
{
	if (!confige->no_texture || !confige->so_texture || !confige->we_texture
		|| !confige->ea_texture)
	{
		write_error("Error: Missing texture path(s)\n");
		free_config(confige);
		exit(EXIT_FAILURE);
	}
	if (confige->f_r == -1 || confige->c_r == -1)
	{
		write_error("Error: Missing floor or ceiling color\n");
		free_config(confige);
		exit(EXIT_FAILURE);
	}
	if (!confige->map)
	{
		write_error("Error: Map is missing\n");
		free_config(confige);
		exit(EXIT_FAILURE);
	}
}

t_config	check_map(char *av)
{
	t_config	confige;

	open_map_and_else(av, &confige);
	convert_map_list_to_array(&confige);
	validate_config(&confige);
	validate_map(&confige);
	return (confige);
}
