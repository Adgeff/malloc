/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geargenc <geargenc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/20 04:34:45 by geargenc          #+#    #+#             */
/*   Updated: 2021/12/23 19:16:57 by geargenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

const t_zone_desc	*ft_zone_desctab(
						size_t size)
{
	int					i;

	i = 0;
	while (g_zone_desctab[i].alloc_max_size
		&& size > g_zone_desctab[i].alloc_max_size)
		i++;
	return (&(g_zone_desctab[i]));
}

void				*ft_allocate(
						void *addr,
						size_t size,
						t_alloc **place,
						t_alloc *prev)
{
	t_alloc				*alloc;

	if ((alloc = (t_alloc *)ft_org_alloc()) == MAP_FAILED)
		return (MAP_FAILED);
	alloc->addr_begin = addr;
	alloc->size = size;
	alloc->previous = prev;
	alloc->next = *place;
	if (alloc->next)
		alloc->next->previous = alloc;
	*place = alloc;
	return (addr);
}

void				*ft_find_space(
						size_t size,
						t_zone *zone,
						const t_zone_desc *zone_desc)
{
	t_alloc				*alloc;

	if (!zone->first ||
		((size_t)zone->first->addr_begin - (size_t)zone->addr_begin) >= size)
		return (ft_allocate(zone->addr_begin, size, &(zone->first), NULL));
	alloc = zone->first;
	while (alloc->next && ((size_t)alloc->addr_begin + alloc->size) + size >
		(size_t)alloc->next->addr_begin)
		alloc = alloc->next;
	if (alloc->next || ((size_t)alloc->addr_begin + alloc->size) + size
		<= (size_t)zone->addr_begin + zone_desc->zone_size)
	{
		return (ft_allocate((void *)((size_t)alloc->addr_begin + alloc->size),
			size, &(alloc->next), alloc));
	}
	return (NULL);
}

void				*ft_allocate_zone(
						size_t size,
						const t_zone_desc *zone_desc)
{
	t_zone				*zone;
	void				*addr;

	if ((addr = mmap(NULL, zone_desc->zone_size ? zone_desc->zone_size : size,
		PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0)) != MAP_FAILED)
	{
		if ((zone = (t_zone *)ft_org_alloc()) != MAP_FAILED)
		{
			if ((zone->first = (t_alloc *)ft_org_alloc()) != MAP_FAILED)
			{
				zone->addr_begin = addr;
				zone->previous = NULL;
				if (*(zone_desc->zone_ptr))
					(*(zone_desc->zone_ptr))->previous = zone;
				zone->next = *(zone_desc->zone_ptr);
				*(zone_desc->zone_ptr) = zone;
				*(zone->first) = (t_alloc){addr, size, NULL, NULL};
				return (addr);
			}
			ft_org_free(zone);
		}
		munmap(addr, size);
	}
	return (MAP_FAILED);
}

void				*ft_find_space_zone(
						size_t size,
						const t_zone_desc *zone_desc)
{
	t_zone				*zone;
	void				*addr;

	zone = *(zone_desc->zone_ptr);
	addr = NULL;
	while (zone && (addr = ft_find_space(size, zone, zone_desc)) == NULL)
		zone = zone->next;
	if (addr != NULL)
		return (addr);
	return (ft_allocate_zone(size, zone_desc));
}

void				*ft_malloc(
						size_t size)
{
	const t_zone_desc	*zone_desc;
	void				*addr;

	size = size ? size : 1;
	zone_desc = ft_zone_desctab(size);
	size = ((size - 1) / zone_desc->step_size + 1) * zone_desc->step_size;
	if ((addr = zone_desc->f_allocate(size, zone_desc)) == MAP_FAILED)
		return (NULL);
	return (addr);
}

void			*calloc(size_t count, size_t size)
{
	void		*ret;

	ret = malloc(count * size);
	if (ret)
		ft_bzero(ret, count * size);
	return (ret);
}

void			*xmalloc(size_t size)
{
	void		*ret;

	errno = 0;
	ret = malloc(size);
	if (ret == NULL)
	{
		perror("xmalloc");
		exit(EXIT_FAILURE);
	}
	return (ret);
}

void			*xrealloc(void *addr, size_t size)
{
	void		*ret;

	errno = 0;
	ret = realloc(addr, size);
	if (ret == NULL)
	{
		perror("xrealloc");
		exit(EXIT_FAILURE);
	}
	return (ret);
}

void			*malloc(
					size_t size)
{
	void			*ret;

	pthread_mutex_lock(&g_ft_malloc_mutex);
	ret = g_root.f_malloc(size);
	pthread_mutex_unlock(&g_ft_malloc_mutex);
	return (ret);
}
