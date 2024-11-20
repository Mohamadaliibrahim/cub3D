/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 17:42:07 by mohamibr          #+#    #+#             */
/*   Updated: 2024/11/20 16:58:51 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H


# include "../ft_printf/inc/ft_printf.h"
# include "../lib_ft/inc/libft.h"
# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <string.h>
# include <stddef.h>
# include <stdio.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

typedef struct s_map_node
{
	char				*line;
	struct s_map_node	*next;
}						t_map_node;

typedef struct s_config
{
	char		*no_texture;
	char		*so_texture;
	char		*we_texture;
	char		*ea_texture;
	int			player_x;
	int			player_y;
	char		player_orientation;
	int			floor_color;
	int			ceiling_color;
	char		**map;
	t_map_node	*map_list;
}				t_config;

char		*get_next_line(int fd);
void		open_map_and_else(char *av, t_config *confige);
void		write_error(char *str);
void		convert_map_list_to_array(t_config *config);
void		free_config(t_config *config);
void		validate_map(t_config *confige);

#endif