/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geargenc <geargenc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/20 02:26:47 by geargenc          #+#    #+#             */
/*   Updated: 2021/12/23 19:17:37 by geargenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GLOBALS_H

# define GLOBALS_H

# include "malloc.h"

t_root				g_root = (t_root){
	.tiny = NULL,
	.small = NULL,
	.large = NULL,
	.org = NULL,
	.page_size = 0,
	.org_link_size = 0,
	.org_link_nb = 0,
	.org_availability_size = 0,
	.f_malloc = &ft_first_malloc
};

pthread_mutex_t 	g_ft_malloc_mutex = PTHREAD_MUTEX_INITIALIZER;

const t_zone_desc	g_zone_desctab[] =
{
	{"TINY", TINY_SIZE, TINY_ZONE_SIZE, TINY_STEP, &(g_root.tiny), &ft_find_space_zone, ft_realloc_notlarge},
	{"SMALL", SMALL_SIZE, SMALL_ZONE_SIZE, SMALL_STEP, &(g_root.small), &ft_find_space_zone, ft_realloc_notlarge},
	{"LARGE", 0, 0, LARGE_STEP, &(g_root.large), &ft_allocate_zone, &ft_realloc_large},
	{NULL, 0, 0, 0, NULL, NULL, NULL}
};

#endif
