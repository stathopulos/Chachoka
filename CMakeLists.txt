# Minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Project name and languages used
project(chachoka CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Source files
set(SOURCES
    Chachoka.cpp
    CLI.cpp
    Game.cpp
    Player.cpp
    PlayerList.cpp
)

# Header files
set(HEADERS
    CLI.h
    Game.h
    Player.h
    PlayerList.h
)

# Add the executable target
add_executable(chachoka ${SOURCES} ${HEADERS})
