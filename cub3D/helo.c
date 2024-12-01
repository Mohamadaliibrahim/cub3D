/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 20:37:08 by mohamibr          #+#    #+#             */
/*   Updated: 2024/12/01 20:38:34 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	process_line(char *line, t_config *confige, int *m_start, int *m_end)
{
	if (*line != '\0')
	{
		if (is_map_line(line))
		{
			if (*m_end)
			{
				write_error("Error: Invalid content after map data\n");
				free(line);
				return (-1);
			}
			*m_start = 1;
			add_to_map(line, confige);
		}
		else if (!*m_start)
			check_type(&line, confige);
		else
		{
			*m_end = 1;
			write_error("Error: Invalid line in map file after map data\n");
			free(line);
			return (-1);
		}
	}
	else if (*m_start)
		*m_end = 1;
	free(line);
	return (0);
}
