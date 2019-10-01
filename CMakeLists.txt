cmake_minimum_required(VERSION 3.14)
project(FinMan)

add_executable(FinMan src/main.cpp)
include_directories(inc)

link_directories(SQLite)
target_link_libraries(FinMan sqlite3)
