#!/bin/sh
if [ $(uname -s) = "Linux" ]
then
	export LD_PRELOAD=./libft_malloc.so
elif [ $(uname -s) = "Darwin" ]
then
	export DYLD_INSERT_LIBRARIES=./libft_malloc.so
	export DYLD_FORCE_FLAT_NAMESPACE=1
fi
exec $@
