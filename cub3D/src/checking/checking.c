/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checking.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 18:31:09 by mohamibr          #+#    #+#             */
/*   Updated: 2024/12/03 14:06:54 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

void	assign(char *id, char *path, t_config *confige, char **parts)
{
	if (ft_strcmp(id, "NO") == 0)
	{
		assign_north(path, parts, confige);
	}
	else if (ft_strcmp(id, "SO") == 0)
	{
		if (confige->so_texture != NULL)
			assign_err(path, parts, confige);
		confige->so_texture = path;
	}
	else if (ft_strcmp(id, "WE") == 0)
	{
		if (confige->we_texture != NULL)
			assign_err(path, parts, confige);
		confige->we_texture = path;
	}
	else if (ft_strcmp(id, "EA") == 0)
	{
		if (confige->ea_texture != NULL)
			assign_err(path, parts, confige);
		confige->ea_texture = path;
	}
	else
		assign_err(path, parts, confige);
}

int	parse_texture_line(char *line, t_config *confige)
{
	char	**parts;
	char	*texture_path;

	parts = ft_split(line, ' ');
	if (!parts || !parts[0] || !parts[1] || parts[2])
	{
		write_error("Error: Invalid texture line format\n");
		two_d_free(parts);
		return (1);
	}
	confige->temp = line;
	if (get_texture_path(parts, confige, &texture_path))
		return (1);
	assign(parts[0], texture_path, confige, parts);
	two_d_free(parts);
	return (0);
}

int	process_line(char *line, t_config *confige, int *m_start, int *m_end)
{
	if (*line != '\0')
	{
		if (is_map_line(line))
		{
			if (*m_end)
				return (write_error("Error: Invalid content after map data\n")
					, error_free(line, confige), -1);
			*m_start = 1;
			add_to_map(line, confige);
		}
		else if (!*m_start)
		{
			if (check_type(&line, confige))
				return (error_free(line, confige), -1);
		}
		else
			return (*m_end = 1
				, write_error("Error: Invalid line in map file after map data\n")
				, error_free(line, confige), -1);
	}
	else if (*m_start)
		*m_end = 1;
	return (free(line), 0);
}

int	ini_open_mp(int *map_started, int *map_ended, t_config *confige, char *av)
{
	int	fd;

	*map_started = 0;
	*map_ended = 0;
	ft_memset(confige, 0, sizeof(t_config));
	confige->f_r = -1;
	confige->c_r = -1;
	confige->memold = NULL;
	fd = open(av, O_RDONLY);
	if (fd == -1)
	{
		write_error("Error: Cannot open map file\n");
		exit(EXIT_FAILURE);
	}
	return (fd);
}

void	open_map_and_else(char *av, t_config *confige)
{
	int		fd;
	char	*line;
	int		map_started;
	int		map_ended;
	char	*trimmed_line;

	fd = ini_open_mp(&map_started, &map_ended, confige, av);
	line = get_next_line(fd, confige);
	while (line != NULL)
	{
		trimmed_line = ft_strtrim(line, " \t\n");
		free(line);
		if (!trimmed_line)
			open_map_error(confige, fd);
		if (process_line(trimmed_line, confige, &map_started, &map_ended) == -1)
		{
			free_config(confige);
			close(fd);
			exit(EXIT_FAILURE);
		}
		line = get_next_line(fd, confige);
	}
	if (line)
		free(line);
	close(fd);
}
