/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_org_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geargenc <geargenc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/20 02:33:59 by geargenc          #+#    #+#             */
/*   Updated: 2020/09/23 04:57:02 by geargenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void				__attribute__((constructor))ft_org_init(void)
{
	// size_t				left;

	g_root.tiny = NULL;
	g_root.small = NULL;
	g_root.large = NULL;
	g_root.org = NULL;
	g_root.page_size = getpagesize();
	g_root.org_link_size = sizeof(t_zone) > sizeof(t_alloc)
		? sizeof(t_zone) : sizeof(t_alloc);
	g_root.org_link_nb = (g_root.page_size - sizeof(t_org))
		/ (g_root.org_link_size * 8 + 1) * 8;
	g_root.org_availability_size = g_root.org_link_nb / 8;
	// left = g_root.page_size - sizeof(t_org) - g_root.org_link_nb
	// 	* g_root.org_link_size - g_root.org_availability_size;
}
