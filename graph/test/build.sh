#!/bin/bash

clang++ -std=c++14 -O0  -I../ base_test.cpp -lpthread -lgtest -lgtest_main
