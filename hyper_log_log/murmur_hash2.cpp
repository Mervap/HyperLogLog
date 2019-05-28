//
// Created by dumpling on 29.05.19.
//

#include <string>
#include <cstdint>
#include "murmur_hash2.h"

static const uint32_t m = 0x5bd1e995;
static const uint32_t r = 24;
static const uint32_t seed = 42;

uint32_t murmur_hash2(int x) {

    uint32_t len = std::to_string(x).length();
    uint32_t h = seed ^len;

    auto k = static_cast<uint32_t>(x);

    k *= m;
    k ^= k >> r;
    k *= m * m;
    h ^= k;

    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;

    return h;
}

