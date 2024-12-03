/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 18:29:20 by mohamibr          #+#    #+#             */
/*   Updated: 2024/12/03 13:55:53 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

void	open_map_error(t_config *confige, int fd)
{
	write_error("Error: Memory allocation failed for trimmed line\n");
	free_config(confige);
	close(fd);
	exit(EXIT_FAILURE);
}

void	assign_err(char *path, char **parts, t_config *confige)
{
	write_error("Error: Duplicate NO texture definition\n");
	free(path);
	two_d_free(parts);
	free(confige->temp);
	free_config(confige);
	exit(EXIT_FAILURE);
}

void	assign_north(char *path, char **parts, t_config *confige)
{
	if (confige->no_texture != NULL)
		assign_err(path, parts, confige);
	confige->no_texture = path;
}

int	check_type(char **trimmed_line, t_config *config)
{
	if (is_texture_line(*trimmed_line))
	{
		if (parse_texture_line(*trimmed_line, config))
			return (1);
	}
	else if (is_color_line(*trimmed_line))
	{
		if (parse_color_line(*trimmed_line, config))
			return (1);
	}
	else
	{
		write_error("Error: Invalid line in map file\n");
		return (1);
	}
	return (0);
}

void	two_d_free(char **str)
{
	int	i;

	if (!str)
		return ;
	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}
