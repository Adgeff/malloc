# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: geargenc <geargenc@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/03/14 10:12:40 by geargenc          #+#    #+#              #
#    Updated: 2022/03/15 14:04:29 by geargenc         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

LNAME := libft_malloc.so

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

HOSTTYPE_LAST_FILE := HOSTTYPE_LAST

HOSTTYPE_LAST := $(shell cat $(HOSTTYPE_LAST_FILE) 2>&-)

ifeq ($(HOSTTYPE), $(HOSTTYPE_LAST))
	HOSTTYPE_DEP :=
else
	HOSTTYPE_DEP := $(LNAME)
endif

NAME := libft_malloc_$(HOSTTYPE).so

LIBDIRS := libft

LIBARCHIVES := libft/libft.a

SRCDIR := src

SRC := $(shell find $(SRCDIR) -name "*.c" -and -not -name "__*")

BUILDDIR := build

OBJ := $(SRC:%.c=$(BUILDDIR)/%.c.o)

DEP := $(SRC:%.c=$(BUILDDIR)/%.c.d)

INCDIRS := include libft/includes

INCFLAGS := $(INCDIRS:%=-I%)

CC := gcc

CPPFLAGS := $(INCFLAGS) -MMD -MP

CFLAGS := -Wall -Wextra -Werror -fPIC

LD := gcc

LDFLAGS := -shared -fPIC

all : libs $(LNAME)

libs : $(LIBDIRS)

.PHONY : $(LIBDIRS) libs clean fclean re all $(HOSTTYPE_DEP)

$(LIBDIRS) :
	$(MAKE) -C $@

$(LNAME) : $(NAME)
	ln -fs $< $@
	echo $(HOSTTYPE) > $(HOSTTYPE_LAST_FILE)

$(OBJ) : $(BUILDDIR)/%.c.o : %.c $(BUILDDIR)/%.c.d
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(DEP) :

$(NAME) : $(OBJ) $(LIBARCHIVES)
	$(LD) $(LDFLAGS) $^ -o $@

clean :
	rm -rf $(BUILDDIR)
	for lib in $(LIBDIRS) ; do $(MAKE) -C $$lib clean ; done

fclean :
	rm -rf $(BUILDDIR)
	rm -f $(NAME)
	rm -f $(LNAME)
	rm -f $(HOSTTYPE_LAST_FILE)
	for lib in $(LIBDIRS) ; do $(MAKE) -C $$lib fclean ; done

re : fclean all

-include $(DEP)
