/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_show_alloc_mem.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geargenc <geargenc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/23 01:30:55 by geargenc          #+#    #+#             */
/*   Updated: 2021/12/14 05:43:31 by geargenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

#define HEX_BASE "0123456789ABCDEF"

void		ft_putaddr(
				uintptr_t addr)
{
	int			i;

	ft_putstr("0x");
	i = 2 * sizeof(uintptr_t) - 1;
	while (i > 0 && ((addr >> (i * 4)) & 0x0F) == 0)
		i--;
	while (i >= 0)
	{
		ft_putchar(HEX_BASE[(addr >> (i * 4)) & 0x0F]);
		i--;
	}
}

void		ft_putsize(
				size_t size)
{
	if (size > 9)
		ft_putsize(size / 10);
	ft_putchar(size % 10 + '0');
}

void		ft_show_alloc_zone(
				t_alloc *alloc)
{
	while (alloc)
	{
		ft_putstr("\t\t");
		ft_putaddr((uintptr_t)alloc->addr_begin);
		ft_putstr(" - ");
		ft_putaddr((uintptr_t)(alloc->addr_begin + alloc->size));
		ft_putstr(" :\t");
		ft_putsize(alloc->size);
		ft_putstr(" octets\n");
		alloc = alloc->next;
	}
	ft_putstr("\n");
}

void		ft_show_alloc_zonetype(
				t_zone_desc *zone_desc)
{
	t_zone		*zone;

	ft_putstr(zone_desc->name);
	ft_putstr(" :\n");
	zone = *(zone_desc->zone_ptr);
	while (zone)
	{
		ft_putstr("\t");
		ft_putaddr((uintptr_t)zone->addr_begin);
		ft_putstr(" - ");
		ft_putaddr((uintptr_t)(zone->addr_begin + (zone_desc->zone_size ?
			zone_desc->zone_size : zone->first->size)));
		ft_putstr(" :\t\t*");
		ft_putsize((size_t)((zone_desc->zone_size ?
			zone_desc->zone_size : zone->first->size)));
		ft_putstr(" octets\n");
		if (zone_desc->zone_size)
			ft_show_alloc_zone(zone->first);
		zone = zone->next;
	}
}

void		ft_show_alloc_mem(void)
{
	int			i;

	i = 0;
	while (g_zone_desctab[i].zone_ptr)
	{
		ft_show_alloc_zonetype(&(g_zone_desctab[i]));
		i++;
	}
}

void		show_alloc_mem(void)
{
	ft_show_alloc_mem();
}
