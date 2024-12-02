/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 17:42:07 by mohamibr          #+#    #+#             */
/*   Updated: 2024/12/02 22:19:18 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H
# define MOVE_SPEED 0.1
# define ROT_SPEED 0.05

# include "../ft_printf/inc/ft_printf.h"
# include "../lib_ft/inc/libft.h"
# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <string.h>
# include <stddef.h>
# include <stdio.h>
# include <mlx.h>
# include <X11/X.h>
# include <X11/keysym.h>
# include <math.h>

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
	int			map_width;
	int			map_height;
	char		**map;
	t_map_node	*map_list;
}				t_config;

typedef struct s_game
{
	void		*mlx;
	void		*win;
	void		*img;
	char		*img_addr;
	int			bpp;
	int			line_length;
	int			endian;
	t_config	config;
	double		player_x;
	double		player_y;
	double		player_angle;
	double		dir_x;
	double		dir_y;
	double		plane_x;
	double		plane_y;
	void		*textures[4];
	char		*tex_data[4];
	int			tex_width;
	int			tex_height;
	int			tex_bpp;
	int			tex_line_length;
	int			tex_endian;
	int			window_width;
	int			window_height;
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

int			is_surrounded_by_walls(char **map, int i, int j);
int			close_check(int *i, char **map, int *j);
int			get_map_height(char **map);
int			is_line_closed(char *line);
int			is_map_line(char *line);
int			is_color_line(char *line);
int			is_texture_line(char *line);
int			trim_and_convert_color(char *color_str, int *color_value);
int			is_map_line(char *line);
char		*get_texture_path(char **parts, t_config *confige);
char		*get_next_line(int fd);
void		open_map_and_else(char *av, t_config *confige);
void		write_error(char *str);
void		convert_map_list_to_array(t_config *config);
void		free_config(t_config *config);
void		validate_map(t_config *confige);
void		two_d_free(char **str);
void		texture_err(int len, char *path, char **parts, t_config *confige);
void		add_to_map(char *line, t_config *config);
void		map_err(t_map_node *new_node);
void		parse_color_line(char *line, t_config *confige);
void		error_msg(t_config *confige);
void		open_map_error(t_config *confige, int fd);
void		assign_err(char *path, char **parts, t_config *confige);
void		assign_north(char *path, char **parts, t_config *confige);
void		check_type(char **trimmed_line, t_config *config);
void		parse_values(char *line, char **id, char **color,
				t_config *confige);
void		value_check(char **id, char *line, char **color, t_config *confige);
void		assign_color(char *id, int rgb[3], t_config *confige, char *line);
void		color_check(t_config *confige, char *id, int rgb[3], char *line);
int			parse_color_values(char *str, int rgb[3]);
int			ini_open_mp(int *start, int *end, t_config *confige, char *av);
int			process_line(char *line, t_config *confige,
				int *m_start, int *m_end);
void		parse_texture_line(char *line, t_config *confige);
void		assign(char *id, char *path, t_config *confige, char **parts);
void		update_direction(t_game *game);
void		render_b(t_game *game);
int			game_loop(t_game *game);
void		free_config_1(t_config *config);
void		free_config(t_config *config);
void		write_error(char *str);
void		validate_config(t_config *confige);
t_config	check_map(char *av);
void		file_name(char *str);
void		destroing(t_game *game);
void		exit_program(t_game *game);
int			close_window(t_game *game);
void		initialize_mlx(t_game *game);
void		initialize_window(t_game *game);
void		initialize_image(t_game *game);
void		initialize_player(t_game *game);
void		initialize_game_window(t_game *game);
void		set_pixel(t_game *game, int x, int y, int color);
void		draw_floor(t_game *game);
void		update_direction(t_game *game);
void		handle_mov2(double new_x, double new_y, t_game *game);
void		handle_movement(int keycode, t_game *game, double move_speed);
int			key_press(int keycode, t_game *game);
void		draw_ceiling(t_game *game);
void		render_c(t_game *game);
void		load_textures(t_game *game);
void		draw_wall(t_game *game, t_draw_wall_params *params);
void		calculate_wall_params(t_game *game, t_wall_params *wp, int x);
void		set_step_and_side_dist(t_game *game, t_wall_params *wp);
void		perform_dda(t_game *game, t_wall_params *wp);
void		calculate_perp_wall_dist(t_game *game, t_wall_params *wp);
void		calculate_line_params(t_game *game, t_draw_wall_params *dwp,
				t_wall_params *wp);
void		calculate_texture_id(t_draw_wall_params *dwp, t_wall_params *wp);
void		calculate_texture_coords(t_game *game, t_draw_wall_params *dwp,
				t_wall_params *wp);
void		calculate_draw_params(t_game *game, t_draw_wall_params *dwp,
				t_wall_params *wp, int x);
void		render_walls(t_game *game);
void		render_b(t_game *game);
int			game_loop(t_game *game);
void		free_get_next_line(void);
#endif