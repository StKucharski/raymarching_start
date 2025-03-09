#!/bin/bash
g++ main.cpp -o app $(pkg-config --cflags --libs glfw3 glew) -lGL -std=c++11
./app
