#!/bin/bash
export CC=clang
export CXX=clang++

export BOOST_LIBS=/usr/local/lib
export BOOST_INCLUDES=/usr/local/include

./waf configure --with-tests --boost-libs=$BOOST_LIBS --boost-includes=$BOOST_INCLUDES --with-tests
