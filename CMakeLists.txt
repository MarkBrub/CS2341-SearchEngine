cmake_minimum_required(VERSION 3.15)
project(Final_Project)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

add_executable(Final_Project main.cpp AVL.hpp word.hpp indexHandler.cpp indexHandler.hpp porter2_stemmer.cpp porter2_stemmer.h util/hash.h DocumentParser.h DocumentParser.cpp DShashtable.h Document.h QueryProcessor.cpp QueryProcessor.h UserInterface.cpp UserInterface.h)