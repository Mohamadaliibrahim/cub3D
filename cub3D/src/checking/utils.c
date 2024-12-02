/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 18:29:20 by mohamibr          #+#    #+#             */
/*   Updated: 2024/12/02 22:13:28 by mohamibr         ###   ########.fr       */
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
	free_config(confige);
	exit(EXIT_FAILURE);
}

void	assign_north(char *path, char **parts, t_config *confige)
{
	if (confige->no_texture != NULL)
		assign_err(path, parts, confige);
	confige->no_texture = path;
}

void	check_type(char **trimmed_line, t_config *config)
{
	if (is_texture_line(*trimmed_line))
		parse_texture_line(*trimmed_line, config);
	else if (is_color_line(*trimmed_line))
		parse_color_line(*trimmed_line, config);
	else
	{
		write_error("Error: Invalid line in map file\n");
		free(*trimmed_line);
		free_config(config);
		exit(EXIT_FAILURE);
	}
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
