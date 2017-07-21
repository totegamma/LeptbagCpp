#!/bin/sh
g++-7 --std=c++17 -c main.cpp
g++-7 -rdynamic --std=c++17 -L /usr/local/lib/ -ldl -o main main.o 
