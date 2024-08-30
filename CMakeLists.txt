cmake_minimum_required(VERSION 3.10)

project(game_of_life)

include_directories( SYSTEM "include/" )

file(GLOB sources src/*.cpp)
add_executable(gol ${sources})

target_link_libraries(gol glfw)
