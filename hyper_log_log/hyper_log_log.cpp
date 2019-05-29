//
// Created by dumpling on 28.05.19.
//

#include <cstdio>
#include <unordered_set>
#include <cstring>
#include <math.h>
#include "hyper_log_log.h"
#include "murmur_hash2.h"

static const uint8_t BIT_DEPTH = 32;
static const long double TWO32 = static_cast<long double>(1ull << 32);

uint32_t get_id(uint32_t x, uint8_t p) {
    return x >> (BIT_DEPTH - p);
}

uint8_t get_zeros_cnt(uint32_t x, uint8_t st_pos) {
    uint8_t cnt = 0;
    for (uint32_t i = 1u << st_pos; i > 0; i >>= 1u, ++cnt) {
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
    uint32_t hash = murmur_hash2(x);
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
    auto estimate = get_estimate();
    if (estimate <= 5.0 / 2.0 * m) {
        if (zeros_buckets != 0) {
            estimate = m * std::log(static_cast<long double>(m) / zeros_buckets);
        } else {
            estimate = estimate;
        }
    } else if (estimate > TWO32 / 30) {
        estimate = -TWO32 * std::log(1 - estimate / TWO32);
    }

    last_eval = round_ans(estimate);
    return last_eval;
}

void Hyper_log_log::clear() {
    memset(buckets, 0, m);
    this->zeros_buckets = m;
}

