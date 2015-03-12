#!/bin/bash

INPUT=pdfpreview
CFLAGS=`pkg-config --cflags poppler`
LIBS=`pkg-config --libs poppler`

# note:
#  when compiling on OSX g++ will not probably find some includes, fixing this 
#  by adding poppler's includedir to header search path
CFLAGS="$CFLAGS -I`pkg-config --variable=includedir poppler`";

g++ -std=c++11 -Wall -pedantic $CFLAGS -o $INPUT $INPUT.cpp $LIBS && strip $INPUT
