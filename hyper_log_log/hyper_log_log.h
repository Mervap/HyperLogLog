//
// Created by dumpling on 25.05.19.
//

#ifndef COUNTER_H
#define COUNTER_H

struct Hyper_log_log {

    Hyper_log_log();

    ~Hyper_log_log();

    void add(int);

    uint32_t get_uniq_num();

    void clear();

private:

    long double get_estimate();

    static const uint8_t p = 16;
    static const uint32_t m = (1u << p);
    constexpr static const long double alpha = 0.7213 / (1 + 1.079 / static_cast<long double>(m));

    uint8_t *bucket;
    uint32_t zeros_buckets;

    bool was_eval;
    uint32_t last_eval;
};

#endif //VK_COUNTER_H
