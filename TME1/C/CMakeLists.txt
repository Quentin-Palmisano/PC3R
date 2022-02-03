cmake_minimum_required(VERSION 3.0)

project(tme1 LANGUAGES C)

add_executable(tme1 main.c)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(tme1 PRIVATE Threads::Threads)

install(TARGETS tme1 RUNTIME DESTINATION bin)
