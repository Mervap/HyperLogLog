cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 14)

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

include_directories(../hyper_log_log
        ${GTEST_INCLUDE_DIRS})

add_executable(test_hyper_log_log test_hyper_log_log.cpp)
target_link_libraries(test_hyper_log_log ${GTEST_LIBRARIES} Threads::Threads hyper_log_log)

enable_testing()
add_test(NAME TestHyperLogLOg COMMAND ./test_hyper_log_log)