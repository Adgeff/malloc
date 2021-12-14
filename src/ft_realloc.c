/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geargenc <geargenc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/20 04:02:58 by geargenc          #+#    #+#             */
/*   Updated: 2021/12/14 04:50:17 by geargenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void		*ft_realloc_large(
				t_alloc *alloc,
				t_zone *zone,
				t_zone_desc *zone_desc,
				size_t size)
{
	void		*addr;

	size = size ? size : 1;
	size = size + zone_desc->step_size - 1 / zone_desc->step_size *
		zone_desc->step_size;
	if (size > alloc->size)
	{
		if ((addr = ft_malloc(size)) == NULL)
			return (NULL);
		ft_memcpy(addr, alloc->addr_begin, alloc->size);
		ft_free_alloc(alloc, zone, zone_desc);
		return (addr);
	}
	else if (size < alloc->size)
	{
		munmap(alloc->addr_begin + size, alloc->size - size);
		alloc->size = size;
	}
	return (alloc->addr_begin);
}

void		*ft_realloc_notlarge(
				t_alloc *alloc,
				t_zone *zone,
				t_zone_desc *zone_desc,
				size_t size)
{
	void		*addr;

	size = size ? size : 1;
	size = ((size - 1) / zone_desc->step_size + 1) * zone_desc->step_size;
	if (alloc->next ?
		(size_t)(alloc->next->addr_begin - alloc->addr_begin) >= size :
		(size_t)(zone->addr_begin + zone_desc->zone_size -
			alloc->addr_begin) >= size)
	{
		alloc->size = size;
		return (alloc->addr_begin);
	}
	if ((addr = ft_malloc(size)) == NULL)
		return (NULL);
	ft_memcpy(addr, alloc->addr_begin, alloc->size);
	ft_free_alloc(alloc, zone, zone_desc);
	return (addr);
}

void		*ft_realloc_change_zone(
				t_alloc *alloc,
				t_zone *zone,
				t_zone_desc *zone_desc,
				size_t size)
{
	void		*addr;

	if ((addr = ft_malloc(size)) == NULL)
		return (NULL);
	ft_memcpy(addr, alloc->addr_begin, alloc->size);
	ft_free_alloc(alloc, zone, zone_desc);
	return (addr);
}

void		*ft_realloc(
				void *addr,
				size_t size)
{
	int			i;
	t_zone		*zone;
	t_alloc		*alloc;

	if (addr == NULL)
		return (ft_malloc(size));
	i = 0;
	zone = NULL;
	while (g_zone_desctab[i].zone_ptr &&
		(zone = ft_find_zone(addr, &(g_zone_desctab[i]))) == NULL)
		i++;
	alloc = NULL;
	if (zone != NULL)
		alloc = ft_find_alloc(addr, zone);
	if (alloc != NULL)
	{
		if (ft_zone_desctab(size) == &(g_zone_desctab[i]))
			return (g_zone_desctab[i].f_realloc(alloc, zone,
				&(g_zone_desctab[i]), size));
		else
			return (ft_realloc_change_zone(alloc, zone,
				&(g_zone_desctab[i]), size));
	}
	return (NULL);
}

void		*realloc(
				void *addr,
				size_t size)
{
	return (ft_realloc(addr, size));
}
