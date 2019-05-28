cmake_minimum_required(VERSION 3.1)
project(Set)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_GLIBCXX_DEBUG -fsanitize=address,undefined")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

include_directories(hyper_log_log)

add_executable(set main.cpp)
add_subdirectory(hyper_log_log hyper_log_log)
target_link_libraries(set hyper_log_log)

enable_testing()
add_subdirectory(test test)

