cmake_minimum_required(VERSION 3.10)
project(PiggyBank)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${FinMan_SOURCE_DIR})

include_directories(inc)

file(GLOB_RECURSE sources src/*.cpp src/*.cxx)
add_executable(PiggyBank ${sources})

#file(GLOB_RECURSE test_sources test/*.cpp test/catch.hpp)
#add_executable(TEST ${sources} ${test_sources})

# Link libraries
# find_package(SQLite3)
# include_directories(${SQLITE3_INCLUDE_DIRS})

#include_directories(/usr/include)
#link_directories(/usr/lib)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set(OPENSSL_USE_STATIC_LIBS TRUE)
find_package(OpenSSL REQUIRED)

set(LIBREADLINE /usr/lib/x86_64-linux-gnu/libreadline.so)



target_link_libraries(PiggyBank
        sqlite3
        Threads::Threads
        OpenSSL::Crypto

        ${SQLITE3_LIBRARIES}
        ${LIBREADLINE}
        ${CMAKE_DL_LIBS}
        )
#target_link_libraries(TEST sqlite3 Threads::Threads OpenSSL::Crypto ${SQLITE3_LIBRARIES} ${LIBREADLINE} ${CMAKE_DL_LIBS})
