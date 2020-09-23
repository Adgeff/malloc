/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_find.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geargenc <geargenc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/21 06:13:04 by geargenc          #+#    #+#             */
/*   Updated: 2020/09/22 05:50:05 by geargenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

t_alloc		*ft_find_alloc(
				void *addr,
				t_zone *zone)
{
	t_alloc		*list;

	list = zone->first;
	while (list && list->addr_begin < addr)
		list = list->next;
	if (list && list->addr_begin == addr)
		return (list);
	return (NULL);
}

t_zone		*ft_find_zone(
				void *addr,
				t_zone_desc *zone_desc)
{
	t_zone		*list;

	list = *(zone_desc->zone_ptr);
	if (zone_desc->zone_size)
		while (list && (addr < list->addr_begin
			|| addr >= list->addr_begin + zone_desc->zone_size))
			list = list->next;
	else
		while (list && list->addr_begin != addr)
			list = list->next;
	return (list);
}
