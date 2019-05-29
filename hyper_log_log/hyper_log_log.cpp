//
// Created by dumpling on 28.05.19.
//

#include <cstdio>
#include <unordered_set>
#include <cstring>
#include <cmath>
#include "hyper_log_log.h"
#include "murmur_hash2.h"
#include "data.h"

static const uint8_t BIT_DEPTH = 64;

uint32_t get_id(uint64_t x, uint8_t p) {
    return static_cast<uint32_t>(x >> (BIT_DEPTH - p));
}

uint8_t get_zeros_cnt(uint64_t x, uint8_t st_pos) {
    uint8_t cnt = 0;
    for (uint64_t i = 1ull << st_pos; i > 0; i >>= 1u, ++cnt) {
        if ((x & i) > 0) {
            break;
        }
    }

    return cnt;
}

uint32_t round_ans(long double x) {
    return static_cast<uint32_t>(std::round(x));
}

Hyper_log_log::Hyper_log_log() : zeros_buckets(0), was_eval(false), last_eval(0) {
    this->buckets = new uint8_t[m];
    clear();
}

Hyper_log_log::~Hyper_log_log() {
    delete[] buckets;
}

void Hyper_log_log::add(int x) {
    was_eval = false;
    uint64_t hash = murmur_hash2_64(x);
    uint32_t id = get_id(hash, p);
    uint8_t zeros = get_zeros_cnt(hash, BIT_DEPTH - p - 1) + 1;
    if (buckets[id] == 0) {
        --zeros_buckets;
    }
    buckets[id] = std::max(buckets[id], zeros);
}


long double Hyper_log_log::get_estimate() {
    long double sum = 0;

    for (int i = 0; i < m; ++i) {
        sum += 1.0 / static_cast<long double>(1u << buckets[i]);
    }


    return alpha * m * m / sum;
}

uint32_t Hyper_log_log::get_uniq_num() {
    if (was_eval) {
        return last_eval;
    }

    was_eval = true;
    long double estimate = get_estimate();

    if (estimate <= 5.0 * m) {
        estimate -= estimate_bias(estimate);
    }

    if (zeros_buckets != 0) {
        auto linear = m * std::log(static_cast<long double>(m) / zeros_buckets);
        if (linear <= threshold) {
            estimate = linear;
        }
    }

    last_eval = round_ans(estimate);
    return last_eval;
}

void Hyper_log_log::clear() {
    memset(buckets, 0, m);
    this->zeros_buckets = m;
}

long double Hyper_log_log::estimate_bias(long double estimate) {
    if (estimate <= raw_estimate_data[0]) {
        return bias_data[0];
    }

    size_t len = sizeof(raw_estimate_data) / sizeof(raw_estimate_data[0]);
    if (raw_estimate_data[len - 1] < estimate) {
        return bias_data[len - 1];
    }

    int i = 1;
    while (i < len && raw_estimate_data[i] < estimate) {
        ++i;
    }

    return bias_data[i - 1] + (bias_data[i] - bias_data[i - 1]) * (estimate - raw_estimate_data[i - 1]) /
                              (raw_estimate_data[i] - raw_estimate_data[i - 1]);
}

