#!/bin/bash
export CC=clang
export CXX=clang++

export BOOST_LIBS=/home/vagrant/boost/lib
export BOOST_INCLUDES=/home/vagrant/boost/include

./waf configure --boost-libs=$BOOST_LIBS --boost-includes=$BOOST_INCLUDES
