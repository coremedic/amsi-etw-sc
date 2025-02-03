cmake_minimum_required(VERSION 3.28)
project(DbgLdr C)

set(CMAKE_C_STANDARD 23)

include_directories(include)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG)
endif()

add_executable(DbgLdr main.c)
