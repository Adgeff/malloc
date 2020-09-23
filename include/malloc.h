/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geargenc <geargenc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/20 02:22:51 by geargenc          #+#    #+#             */
/*   Updated: 2020/09/23 03:59:48 by geargenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H

# define MALLOC_H

# define TINY_SIZE 992
# define SMALL_SIZE 130048

# define TINY_STEP 16
# define SMALL_STEP 512
# define LARGE_STEP 4096

# define TINY_ZONE_SIZE 2097152
# define SMALL_ZONE_SIZE 16777216

# include "libft.h"

# include <stdlib.h>
# include <sys/mman.h>
# include <stdio.h>
# include <unistd.h>

typedef struct			s_alloc
{
	void				*addr_begin;
	size_t				size;
	struct s_alloc		*previous;
	struct s_alloc		*next;
}						t_alloc;

typedef struct			s_zone
{
	void				*addr_begin;
	struct s_alloc		*first;
	struct s_zone		*previous;
	struct s_zone		*next;
}						t_zone;

typedef struct			s_org
{
	struct s_org		*previous;
	struct s_org		*next;
}						t_org;

typedef struct			s_root
{
	struct s_zone		*tiny;
	struct s_zone		*small;
	struct s_zone		*large;
	struct s_org		*org;
	size_t				page_size;
	size_t				org_link_size;
	unsigned int		org_link_nb;
	size_t				org_availability_size;
}						t_root;

typedef struct			s_zone_desc
{
	const char * const	name;
	size_t				alloc_max_size;
	size_t				zone_size;
	size_t				step_size;
	struct s_zone		**zone_ptr;
	void				*(*f_allocate)(
							size_t,
							struct s_zone_desc *);
	void				*(*f_realloc)(
							t_alloc *,
							t_zone *,
							struct s_zone_desc *,
							size_t);
}						t_zone_desc;

extern t_root			g_root;
extern t_zone_desc		g_zone_desctab[];

t_zone_desc				*ft_zone_desctab(
							size_t size);

void					*ft_allocate_zone(
							size_t size,
							t_zone_desc *zone_desc);
void					*ft_find_space_zone(
							size_t size,
							t_zone_desc *zone_desc);
void					*ft_malloc(
							size_t size);

void					ft_free_alloc(
							t_alloc *alloc,
							t_zone *zone,
							t_zone_desc *zone_desc);
void					ft_free(
							void *addr);

void					ft_org_free(
							void *addr);
void					*ft_org_alloc(void);
void					ft_org_init(void) __attribute__((constructor));

void					*ft_realloc_large(
							t_alloc *alloc,
							t_zone *zone,
							t_zone_desc *zone_desc,
							size_t size);
void					*ft_realloc_notlarge(
							t_alloc *alloc,
							t_zone *zone,
							t_zone_desc *zone_desc,
							size_t size);
void					*ft_realloc(
							void *addr,
							size_t size);

t_alloc					*ft_find_alloc(
							void *addr,
							t_zone *zone);
t_zone					*ft_find_zone(
							void *addr,
							t_zone_desc *zone_desc);

void					ft_show_alloc_mem(void);
// t_alloc			*lookforspace(void *current, t_alloc *next, size_t current_len, size_t len)
// {
// 	if (next - current - current_len < len)
// 		return (lookforspace(next, next->next, next->size + sizeof(t_alloc), len)
// }

// place = lookforspace(zone, zone->first, sizeof(t_zone), len);

#endif
