/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geargenc <geargenc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/23 03:39:43 by geargenc          #+#    #+#             */
/*   Updated: 2020/09/23 06:55:25 by geargenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

#include <stdbool.h>
#define K 1024
#define M (K * K)

typedef struct		s_mem
{
	unsigned char	*addr;
	size_t			size;
	unsigned char	pattern;
	bool			freed;
	struct s_mem	*next;
}					t_mem;

typedef struct	s_ins
{
	char		ins;
	int			index;
	size_t		size;
	int			mi;
}				t_ins;	

int			main(void)
{
	t_mem	mem[256];
	int		i;
	int		j;
	int		mi;
	void	*addr;
	t_ins	ins[] =
	{
		{'m', 0, 16, 0},
		{'m', 0, 16, 0}
	};

	mi = 0;
	while (mi < 256)
	{
		mem[mi] = (t_mem){NULL, 0, (unsigned char)mi, true, NULL};
		mi++;
	}
	i = 0;
	mi = 1;
	while (mi < 256 && i < (int)(sizeof(ins) / sizeof(t_ins)))
	{
		ins[i].mi = mi;
		if (ins[i].ins == 'm')
		{
			mem[mi].addr = ft_malloc(ins[i].size);
			mem[mi].size = ins[i].size;
			mem[mi].freed = false;
			j = 0;
			while (j < (int)mem[mi].size)
			{
				if (mem[mem[mi].addr[j]].freed == false)
				{
					printf("Error ins %d writes on %hhu\n", i, mem[mi].addr[j]);
					return (1);
				}
				j++;
			}
			memset(mem[mi].addr, mem[mi].pattern, mem[mi].size);
			mi++;
		}
		else if (ins[i].ins == 'f')
		{
			if (mem[ins[ins[i].index].mi].freed == true)
				printf("Error ins %d freed not allocated ins %d\n", i, ins[i].index);
			mem[ins[ins[i].index].mi].freed = true;
			ft_free(mem[ins[ins[i].index].mi].addr);
		}
		else if (ins[i].ins == 'r')
		{
			if (mem[ins[ins[i].index].mi].freed == true)
				printf("Error ins %d reallocated not allocated ins %d\n", i, ins[i].index);
			addr = ft_realloc(mem[ins[ins[i].index].mi].addr, ins[i].size);
			if (addr == mem[ins[ins[i].index].mi].addr)
			{
				mem[ins[ins[i].index].mi].size = ins[i].size;
				j = 0;
				while (j < (int)mem[ins[ins[i].index].mi].size)
				{
					if (mem[mem[ins[ins[i].index].mi].addr[j]].freed == false)
					{
						printf("Error ins %d writes on %hhu\n", i, mem[mi].addr[j]);
						return (1);
					}
					j++;
				}
				memset(mem[ins[ins[i].index].mi].addr,
					mem[ins[ins[i].index].mi].pattern,
					mem[ins[ins[i].index].mi].size);
			}
			else
			{
				
			}
		}
		else if (ins[i].ins == 's')
			ft_show_alloc_mem();
		i++;
	}
	ft_show_alloc_mem();
	return (0);
}
