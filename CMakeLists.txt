cmake_minimum_required(VERSION 3.25)
project(IfaradaPageReplacement C)

set(CMAKE_C_STANDARD 99)

add_library(IfaradaPageReplacement Simulator.c PageTable.h DataLoader.h data-1.txt DataLoader.c PageTable.c)