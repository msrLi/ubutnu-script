#!/bin/sh

if [ "$0" != "-bash" ];then
	echo "Runing with:"
	echo "            . env.mk"
fi

PATH_BASE=`pwd`
LIB_PATH=${PATH_BASE}/toolchain/lib:${PATH_BASE}/lib
export LD_LIBRARY_PATH=${LIB_PATH}:${LD_LIBRARY_PATH}
BIN_PATH=${PATH_BASE}/test
export PATH=${BIN_PATH}:${PATH}

