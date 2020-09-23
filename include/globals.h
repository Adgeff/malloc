/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geargenc <geargenc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/20 02:26:47 by geargenc          #+#    #+#             */
/*   Updated: 2020/09/23 04:14:38 by geargenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GLOBALS_H

# define GLOBALS_H

# include "malloc.h"

t_root				g_root;

t_zone_desc			g_zone_desctab[] =
{
	{"TINY", TINY_SIZE, TINY_ZONE_SIZE, TINY_STEP, &(g_root.tiny), &ft_find_space_zone, ft_realloc_notlarge},
	{"SMALL", SMALL_SIZE, SMALL_ZONE_SIZE, SMALL_STEP, &(g_root.small), &ft_find_space_zone, ft_realloc_notlarge},
	{"LARGE", 0, 0, LARGE_STEP, &(g_root.large), &ft_allocate_zone, &ft_realloc_large},
	{NULL, 0, 0, 0, NULL, NULL, NULL}
};

#endif
