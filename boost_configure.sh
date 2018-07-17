#! /bin/bash

# Configure to build boost with Clang instead of gcc

export PREFIX=$HOME/boost

./bootstrap.sh --with-toolset=clang --with-icu=/usr/include/x86_64-linux-gnu --prefix=$PREFIX --exec-prefix=$PREFIX

./b2 stage -j10

# proceed if everything is correct

# ./b2 install -j10