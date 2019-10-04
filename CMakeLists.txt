cmake_minimum_required(VERSION 3.14)
project(FinMan)

project(tests)

include_directories(inc)

file(GLOB_RECURSE sources src/*.c src/*.cpp)
add_executable(FinMan ${sources})

# Link libraries
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set(OPENSSL_USE_STATIC_LIBS TRUE)
find_package(OpenSSL REQUIRED)

set(LIBREADLINE /usr/lib/x86_64-linux-gnu/libreadline.so)

link_directories(lib/sqlite3)
target_link_libraries(FinMan sqlite3 Threads::Threads OpenSSL::Crypto ${LIBREADLINE} ${CMAKE_DL_LIBS})

#file(GLOB_RECURSE test_sources test/*.cpp)
#add_executable(tests ${sources} ${test_sources})