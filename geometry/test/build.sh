#!/bin/bash

clang++ -std=c++14 -O0 -g -I../../ `ls | grep cpp` ../base.cpp ../delaunay_graph.cpp -lpthread -lgtest -lgtest_main
