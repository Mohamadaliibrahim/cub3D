/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 17:47:07 by mohamibr          #+#    #+#             */
/*   Updated: 2024/11/17 22:24:54 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/cub3d.h"

void	write_error(char *str)
{
	while (*str)
		write(2, str++, 1);
}

void	check_map(char *av)
{
	open_map_and_else(av);
}

int	main(int ac, char **av)
{
	if (ac == 2)
	{
		check_map(av[1]);
	}
	else
		write_error("Error: Wrong input\n");
	return (0);
}
