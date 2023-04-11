cmake_minimum_required(VERSION 3.22)
project(GoGame)

set(CMAKE_CXX_STANDARD 14)

add_executable(GoGame main.cpp conio2.cpp conio2.h Board.cpp Board.h Cell.cpp Cell.h StringVector.cpp StringVector.h SingleLinkedList.cpp SingleLinkedList.h Node.cpp Node.h)
