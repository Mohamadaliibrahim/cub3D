/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_calc2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjoundi <mjoundi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 19:46:58 by mjoundi           #+#    #+#             */
/*   Updated: 2024/12/02 19:49:45 by mjoundi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

void	calculate_texture_coords(t_game *game, t_draw_wall_params *dwp,
		t_wall_params *wp)
{
	if (wp->side == 0)
		dwp->wall_x = game->player_y + wp->perp_wall_dist * wp->ray_dir_y;
	else
		dwp->wall_x = game->player_x + wp->perp_wall_dist * wp->ray_dir_x;
	dwp->wall_x -= floor(dwp->wall_x);
	dwp->tex_x = (int)(dwp->wall_x * (double)game->tex_width);
	if ((wp->side == 0 && wp->ray_dir_x > 0)
		|| (wp->side == 1 && wp->ray_dir_y < 0))
		dwp->tex_x = game->tex_width - dwp->tex_x - 1;
	dwp->tex_step = 1.0 * game->tex_height / dwp->line_height;
}

void	calculate_draw_params(t_game *game, t_draw_wall_params *dwp,
		t_wall_params *wp, int x)
{
	calculate_line_params(game, dwp, wp);
	calculate_texture_id(dwp, wp);
	calculate_texture_coords(game, dwp, wp);
	dwp->x = x;
}

void	render_walls(t_game *game)
{
	int					x;
	t_wall_params		wp;
	t_draw_wall_params	dwp;

	x = 0;
	while (x < game->window_width)
	{
		calculate_wall_params(game, &wp, x);
		set_step_and_side_dist(game, &wp);
		perform_dda(game, &wp);
		calculate_perp_wall_dist(game, &wp);
		calculate_draw_params(game, &dwp, &wp, x);
		draw_wall(game, &dwp);
		x++;
	}
}

void	render_b(t_game *game)
{
	draw_ceiling(game);
	draw_floor(game);
	render_walls(game);
	mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
}

int	game_loop(t_game *game)
{
	render_b(game);
	return (0);
}
