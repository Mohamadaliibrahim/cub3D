/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 17:47:07 by mohamibr          #+#    #+#             */
/*   Updated: 2024/11/20 19:22:03 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/cub3d.h"

void	free_config(t_config *config)
{
	int	i;

	if (config->no_texture)
		free(config->no_texture);
	if (config->so_texture)
		free(config->so_texture);
	if (config->we_texture)
		free(config->we_texture);
	if (config->ea_texture)
		free(config->ea_texture);
	if (config->map)
	{
		i = 0;
		while (config->map[i])
		{
			free(config->map[i]);
			i++;
		}
		free(config->map);
	}
}

void	write_error(char *str)
{
	while (*str)
		write(2, str++, 1);
}

void	validate_config(t_config *confige)
{
	// Check for missing textures
	if (!confige->no_texture || !confige->so_texture || !confige->we_texture
		|| !confige->ea_texture)
	{
		write_error("Error: Missing texture path(s)\n");
		free_config(confige);
		exit(EXIT_FAILURE);
	}
	// Check for missing colors
	if (confige->floor_color == -1 || confige->ceiling_color == -1)
	{
		write_error("Error: Missing floor or ceiling color\n");
		free_config(confige);
		exit(EXIT_FAILURE);
	}
	// Check for missing map
	if (!confige->map)
	{
		write_error("Error: Map is missing\n");
		free_config(confige);
		exit(EXIT_FAILURE);
	}
}

void	check_map(char *av)
{
	t_config	confige;
	int			i;

	open_map_and_else(av, &confige);
	convert_map_list_to_array(&confige);
	validate_config(&confige);
	validate_map(&confige);
	ft_printf("%s\n", confige.we_texture);
	ft_printf("%s\n", confige.ea_texture);
	ft_printf("%s\n", confige.no_texture);
	ft_printf("%s\n", confige.so_texture);
	ft_printf("%d\n", confige.ceiling_color);
	ft_printf("%d\n", confige.floor_color);
	i = 0;
	while (confige.map && confige.map[i])
	{
		printf("%s\n", confige.map[i]);
		i++;
	}
	free_config(&confige);
}

void	file_name(char *str)
{
	int	len;

	len = ft_strlen(str);
	if (len < 4 || ft_strncmp(&str[len - 4], ".cub", 4) != 0)
	{
		write_error("cub3D: Error: Not a .cub file\n");
		exit(1);
	}
}

int	main(int ac, char **av)
{
	if (ac == 2)
	{
		file_name(av[1]);
		check_map(av[1]);
	}
	else
		write_error("Error: Wrong input\n");
	return (0);
}
