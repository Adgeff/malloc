/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_org_page.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geargenc <geargenc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/20 02:32:21 by geargenc          #+#    #+#             */
/*   Updated: 2021/12/22 07:45:03 by geargenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "globals.h"

t_org				*ft_new_org_page(void)
{
	t_org			*org_page;
	uint8_t			*availability_field;
	size_t			i;

	if ((org_page = mmap(NULL, g_root.page_size,
		PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS,
		-1, 0)) == MAP_FAILED)
		return (MAP_FAILED);
	availability_field = (uint8_t *)org_page;
	availability_field += sizeof(t_org);
	availability_field += g_root.org_link_nb * g_root.org_link_size;
	i = 0;
	while (i < g_root.org_availability_size)
	{
		availability_field[i] = 0;
		i++;
	}
	return (org_page);
}

void				*ft_org_page_alloc(void *org_page)
{
	unsigned int	i;
	uint8_t			j;
	uint8_t			*availability_field;
	void			*addr;

	i = 0;
	availability_field = (uint8_t *)org_page + sizeof(t_org);
	availability_field += g_root.org_link_nb * g_root.org_link_size;
	while (i < g_root.org_availability_size)
	{
		if (availability_field[i] != 0xff)
		{
			addr = org_page + sizeof(t_org);
			addr += i * g_root.org_link_size * 8;
			j = 0x80;
			while (!(j & ~availability_field[i]))
			{
				addr += g_root.org_link_size;
				j = j >> 1;
			}
			availability_field[i] |= j;
			return (addr);
		}
		i++;
	}
	return (NULL);
}

void				*ft_org_alloc(void)
{
	t_org			**parent_next;
	t_org			*parent;
	void			*addr;

	parent_next = &(g_root.org);
	parent = NULL;
	addr = NULL;
	while (*parent_next && (addr = ft_org_page_alloc(*parent_next)) == NULL)
	{
		parent = *parent_next;
		parent_next = &((*parent_next)->next);
	}
	if (addr == NULL)
	{
		if ((*parent_next = ft_new_org_page()) == MAP_FAILED)
			return (MAP_FAILED);
		(*parent_next)->previous = parent;
		addr = ft_org_page_alloc(*parent_next);
	}
	return (addr);
}

void				ft_org_free_page(t_org *page)
{
	if (page->previous)
		page->previous->next = page->next;
	else
		g_root.org = page->next;
	if (page->next)
		page->next->previous = page->previous;
	munmap(page, g_root.page_size);
}

void				ft_org_free(void *addr)
{
	uint8_t			*availability_field;
	void			*data_field;
	unsigned int	i;
	uint8_t			j;

	data_field = (uint8_t *)((size_t)addr / g_root.page_size
		* g_root.page_size + sizeof(t_org));
	availability_field = data_field + g_root.org_link_nb * g_root.org_link_size;
	i = 0;
	j = 0x80;
	while (i < (addr - data_field) / g_root.org_link_size % 8)
	{
		j = j >> 1;
		i++;
	}
	availability_field[(addr - data_field) / g_root.org_link_size / 8] &= ~j;
	i = 0;
	while (i < g_root.org_availability_size && availability_field[i] == 0)
		i++;
	if (i == g_root.org_availability_size)
		ft_org_free_page((t_org *)(data_field - sizeof(t_org)));
}
