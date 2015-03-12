#!/bin/bash

INPUT=pdfpreview
CFLAGS=`pkg-config --cflags poppler`
LIBS=`pkg-config --libs poppler`


g++ -std=c++11 -Wall -pedantic $CFLAGS -o $INPUT $INPUT.cpp $LIBS && strip $INPUT
