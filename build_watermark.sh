#!/bin/bash

CC=g++
LIBS="-lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs"
INCLUDE_DIRS=/usr/local/include
LIBRARY_DIRS=/usr/local/lib

echo "Building watermark executable..."
${CC} -std=c++17 -L ${LIBRARY_DIRS} -I ${INCLUDE_DIRS} ${LIBS} -o watermark watermark.cpp
echo "Done!"