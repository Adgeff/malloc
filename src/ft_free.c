/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geargenc <geargenc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/20 04:34:12 by geargenc          #+#    #+#             */
/*   Updated: 2022/03/15 14:29:02 by geargenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

unsigned int	ft_count_empty(
					t_zone *list)
{
	unsigned int	count;

	count = 0;
	while (list != NULL)
	{
		if (list->first == NULL)
			count++;
		list = list->next;
	}
	return (count);
}

void			ft_free_alloc(
					t_alloc *alloc,
					t_zone *zone,
					const t_zone_desc *zone_desc)
{
	size_t			size;

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
	if (zone->first == NULL && (zone_desc->zone_size == 0
		|| ft_count_empty(*(zone_desc->zone_ptr)) > 1))
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

void			ft_free(
					void *addr)
{
	int				i;
	t_zone			*zone;
	t_alloc			*alloc;

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
}

void			free(
					void *addr)
{
	pthread_mutex_lock(&g_ft_malloc_mutex);
	ft_free(addr);
	pthread_mutex_unlock(&g_ft_malloc_mutex);
}
