cmake_minimum_required(VERSION 3.14)
project(FinMan)

file(GLOB_RECURSE sources src/*.c src/*.cpp)
add_executable(FinMan ${sources})
include_directories(inc)

link_directories(SQLite)
target_link_libraries(FinMan sqlite3)
