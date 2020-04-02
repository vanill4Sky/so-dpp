cmake_minimum_required(VERSION 3.2)

project(so-dpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(Curses REQUIRED)
find_package(Threads REQUIRED)

include_directories(${CURSES_INCLUDE_DIR})

add_executable(so-dpp src/main.cpp)

target_link_libraries(so-dpp ${CURSES_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
