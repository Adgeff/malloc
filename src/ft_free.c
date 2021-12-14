/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geargenc <geargenc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/20 04:34:12 by geargenc          #+#    #+#             */
/*   Updated: 2021/12/14 04:47:08 by geargenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void		ft_free_alloc(
				t_alloc *alloc,
				t_zone *zone,
				t_zone_desc *zone_desc)
{
	size_t		size;

	if (zone_desc->zone_size == 0)
		size = alloc->size;
	else
		size = zone_desc->zone_size;
	if (alloc->previous)
		alloc->previous->next = alloc->next;
	else
		zone->first = alloc->next;
	if (alloc->next)
		alloc->next->previous = alloc->previous;
	ft_org_free(alloc);
	if (zone->first == NULL)
	{
		if (zone->previous)
			zone->previous->next = zone->next;
		else
			*(zone_desc->zone_ptr) = zone->next;
		if (zone->next)
			zone->next->previous = zone->previous;
		munmap(zone->addr_begin, size);
		ft_org_free(zone);
	}
}

void		ft_free(
				void *addr)
{
	int			i;
	t_zone		*zone;
	t_alloc		*alloc;

	i = 0;
	zone = NULL;
	while (g_zone_desctab[i].zone_ptr &&
		(zone = ft_find_zone(addr, &(g_zone_desctab[i]))) == NULL)
		i++;
	alloc = NULL;
	if (zone != NULL)
		alloc = ft_find_alloc(addr, zone);
	if (alloc != NULL)
		ft_free_alloc(alloc, zone, &(g_zone_desctab[i]));
	else
		printf("STOP DE FREE D'LA MERDE STP (%p)\n", addr);
}

void		free(
				void *addr)
{
	ft_free(addr);
}
