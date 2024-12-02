/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjoundi <mjoundi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 19:44:51 by mjoundi           #+#    #+#             */
/*   Updated: 2024/12/02 19:49:17 by mjoundi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

void	set_step_and_side_dist(t_game *game, t_wall_params *wp)
{
	if (wp->ray_dir_x < 0)
	{
		wp->step_x = -1;
		wp->side_dist_x = (game->player_x - wp->map_x) * wp->delta_dist_x;
	}
	else
	{
		wp->step_x = 1;
		wp->side_dist_x = (wp->map_x + 1.0 - game->player_x) * wp->delta_dist_x;
	}
	if (wp->ray_dir_y < 0)
	{
		wp->step_y = -1;
		wp->side_dist_y = (game->player_y - wp->map_y) * wp->delta_dist_y;
	}
	else
	{
		wp->step_y = 1;
		wp->side_dist_y = (wp->map_y + 1.0 - game->player_y) * wp->delta_dist_y;
	}
}

void	perform_dda(t_game *game, t_wall_params *wp)
{
	while (wp->hit == 0)
	{
		if (wp->side_dist_x < wp->side_dist_y)
		{
			wp->side_dist_x += wp->delta_dist_x;
			wp->map_x += wp->step_x;
			wp->side = 0;
		}
		else
		{
			wp->side_dist_y += wp->delta_dist_y;
			wp->map_y += wp->step_y;
			wp->side = 1;
		}
		if (wp->map_x < 0 || wp->map_x >= game->config.map_width
			|| wp->map_y < 0 || wp->map_y >= game->config.map_height)
			wp->hit = 1;
		else if (game->config.map[wp->map_y][wp->map_x] == '1')
			wp->hit = 1;
	}
}

void	calculate_perp_wall_dist(t_game *game, t_wall_params *wp)
{
	if (wp->side == 0)
		wp->perp_wall_dist = (wp->map_x - game->player_x
				+ (1 - wp->step_x) / 2) / wp->ray_dir_x;
	else
		wp->perp_wall_dist = (wp->map_y - game->player_y
				+ (1 - wp->step_y) / 2) / wp->ray_dir_y;
}

void	calculate_line_params(t_game *game, t_draw_wall_params *dwp,
		t_wall_params *wp)
{
	dwp->line_height = (int)(game->window_height / wp->perp_wall_dist);
	dwp->start = -dwp->line_height / 2 + game->window_height / 2;
	if (dwp->start < 0)
		dwp->start = 0;
	dwp->end = dwp->line_height / 2 + game->window_height / 2;
	if (dwp->end >= game->window_height)
		dwp->end = game->window_height - 1;
}

void	calculate_texture_id(t_draw_wall_params *dwp, t_wall_params *wp)
{
	if (wp->side == 0)
	{
		if (wp->ray_dir_x > 0)
			dwp->tex_id = 0;
		else
			dwp->tex_id = 1;
	}
	else
	{
		if (wp->ray_dir_y > 0)
			dwp->tex_id = 2;
		else
			dwp->tex_id = 3;
	}
}
