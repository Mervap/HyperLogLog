project(Hyper_log_log)

add_library(hyper_log_log STATIC
        hyper_log_log.h
        hyper_log_log.cpp
        murmur_hash2.h
        murmur_hash2.cpp)