cmake_minimum_required(VERSION 3.10)
project(FinMan)

project(tests)

include_directories(inc)

file(GLOB_RECURSE sources src/*.c src/*.cpp)
add_executable(FinMan ${sources})

# Link libraries
# find_package(SQLite3)
# include_directories(${SQLITE3_INCLUDE_DIRS})

include_directories(/usr/include)
link_directories(/usr/lib)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set(OPENSSL_USE_STATIC_LIBS TRUE)
find_package(OpenSSL REQUIRED)

set(LIBREADLINE /usr/lib/x86_64-linux-gnu/libreadline.so)

target_link_libraries(FinMan sqlite3 Threads::Threads OpenSSL::Crypto ${SQLITE3_LIBRARIES} ${LIBREADLINE} ${CMAKE_DL_LIBS})

#file(GLOB_RECURSE test_sources test/*.cpp)
#add_executable(tests ${sources} ${test_sources})
