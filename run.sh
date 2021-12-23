#!/bin/sh
export HOSTTYPE=${HOSTTYPE:-$(uname -m)_$(uname -s)}
export DYLD_LIBRARY_PATH=$PWD${DYLD_LIBRARY_PATH:+:$DYLD_LIBRARY_PATH}
export DYLD_INSERT_LIBRARIES=libft_malloc.so
export DYLD_FORCE_FLAT_NAMESPACE=1
exec $@
