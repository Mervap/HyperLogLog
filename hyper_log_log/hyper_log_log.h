//
// Created by dumpling on 28.05.19.
//

#ifndef COUNTER_H
#define COUNTER_H

/* The occupied memory is m = 2^14 bytes on buckets
 * + approximately 6Kib on bias data
 * + still on a trifle,
 * that is about 22-23 Kib
 */

struct Hyper_log_log {

    Hyper_log_log();

    ~Hyper_log_log();

    void add(int);

    uint32_t get_uniq_num();

    void clear();

private:

    long double get_estimate();

    static long double estimate_bias(long double);

    static const uint8_t p = 14;
    static const uint32_t m = (1u << p);
    constexpr static const long double alpha = 0.7213 / (1 + 1.079 / static_cast<long double>(m));

    uint8_t *buckets;
    uint32_t zeros_buckets;

    bool was_eval;
    uint32_t last_eval;
};

#endif //VK_COUNTER_H
