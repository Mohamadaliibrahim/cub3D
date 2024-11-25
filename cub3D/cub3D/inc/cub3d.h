/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 17:42:07 by mohamibr          #+#    #+#             */
/*   Updated: 2024/11/25 00:28:39 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H
#define MOVE_SPEED 0.1  // Movement speed: Adjust for how fast the player moves
#define ROT_SPEED 0.05

# include "../ft_printf/inc/ft_printf.h"
# include "../lib_ft/inc/libft.h"
# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <string.h>
# include <stddef.h>
# include <stdio.h>
#include <mlx.h>
#include <X11/X.h>
#include <X11/keysym.h>
#include <math.h>

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
	int			f_r;
	int			f_g;
	int			f_b;
	int			c_r;
	int			c_g;
	int			c_b;
	int 		map_width;
    int 		map_height;
	char		**map;
	t_map_node	*map_list;
}				t_config;

typedef struct s_game
{
    void		*mlx;
    void		*win;
    void		*img;
    char		*img_addr;
    int		bpp;
    int		line_length;
    int		endian;
    t_config	config;
    double player_x;
    double player_y;
    double player_angle;
    double dir_x;
    double dir_y;
    double plane_x;
    double plane_y;
	void        *textures[4];       // Array to hold the textures for North, South, West, East
    char        *tex_data[4];       // Data pointers for each texture (color data)
    int         tex_width;          // Width of the textures
    int         tex_height;         // Height of the textures
    int         tex_bpp;            // Bits per pixel for textures
    int         tex_line_length;    // Line length for texture data
    int         tex_endian;      // Player's Y position
     int window_width;    // Add this line
    int window_height;
}				t_game;

typedef struct s_draw_wall_params
{
	int		x;
	int		start;
	int		end;
	int		line_height;
	int		tex_id;
	int		tex_x;
	double	tex_step;
	double	wall_x;
}	t_draw_wall_params;

typedef struct s_wall_params
{
	double	camera_x;
	double	ray_dir_x;
	double	ray_dir_y;
	int		map_x;
	int		map_y;
	double	side_dist_x;
	double	side_dist_y;
	double	delta_dist_x;
	double	delta_dist_y;
	double	perp_wall_dist;
	int		step_x;
	int		step_y;
	int		hit;
	int		side;
}	t_wall_params;

char		*get_next_line(int fd);
void		open_map_and_else(char *av, t_config *confige);
void		write_error(char *str);
void		convert_map_list_to_array(t_config *config);
void		free_config(t_config *config);
void		validate_map(t_config *confige);

#endif