/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 17:47:07 by mohamibr          #+#    #+#             */
/*   Updated: 2024/11/18 19:59:45 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/cub3d.h"

void	free_config(t_config *config)
{
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
		int i = 0;
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

void	check_map(char *av)
{
	t_config	confige;

	open_map_and_else(av, &confige);
	convert_map_list_to_array(&confige);
	validate_map(&confige);
	ft_printf("%s\n", confige.we_texture);
	ft_printf("%s\n", confige.ea_texture);
	ft_printf("%s\n", confige.no_texture);
	ft_printf("%s\n", confige.so_texture);
	ft_printf("%d\n", confige.ceiling_color);
	ft_printf("%d\n", confige.floor_color);
	int	i = 0;
	while (confige.map && confige.map[i])
	{
		printf("%s\n", confige.map[i]);
		i++;
	}
	free_config(&confige);
}

void file_name(char *str)
{
    int len = ft_strlen(str);

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
