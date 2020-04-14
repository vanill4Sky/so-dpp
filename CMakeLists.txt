cmake_minimum_required(VERSION 3.2)

project(so-dpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

find_package(Curses REQUIRED)
find_package(Threads REQUIRED)

include_directories(${CURSES_INCLUDE_DIR})

add_executable(so-dpp src/main.cpp
               src/philosopher.hpp src/philosopher.cpp
               src/table.hpp src/table.cpp
               src/fork.hpp
               src/curses_wrapper.hpp src/curses_wrapper.cpp
               src/window.hpp src/window.cpp
               src/utils.hpp
               src/visualization.hpp src/visualization.cpp )

target_link_libraries(so-dpp ${CURSES_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
