#!/bin/bash

INPUT=pdfpreview
CXXFLAGS="$CXXFLAGS `pkg-config --cflags poppler`"
LIBS=`pkg-config --libs poppler`

# note:
#  when compiling on OSX g++ will not probably find some includes, fixing this 
#  by adding poppler's includedir to header search path
CXXFLAGS="$CXXFLAGS -I`pkg-config --variable=includedir poppler`";

g++ -std=c++11 -Wall -pedantic $CXXFLAGS -o $INPUT $INPUT.cpp $LIBS && strip $INPUT
