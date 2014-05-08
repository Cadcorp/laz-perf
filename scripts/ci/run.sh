#!/bin/bash -e
# Builds and tests laz-perf
#

mkdir -p _build || exit 1
cd _build || exit 1

# g++4.8.1
if [ "$CXX" = "g++" ]; then 
	export CXX="g++-4.8";
fi

# clang 3.4
if [ "$CXX" == "clang++" ]; then 
	export CXX="clang++-3.4";
fi

cmake \
	-DCMAKE_BUILD_TYPE=Release \
	..

make

./test/tlaz_tests
