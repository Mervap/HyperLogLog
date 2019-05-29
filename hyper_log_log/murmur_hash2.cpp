//
// Created by dumpling on 29.05.19.
//

#include <string>
#include <cstdint>
#include "murmur_hash2.h"

static const uint64_t m = 0xc6a4a7935bd1e995ull;
static const uint64_t r = 47;
static const uint64_t seed = 42;

uint64_t murmur_hash2_64(int x) {

    uint64_t len = std::to_string(x).length();

    uint64_t h = seed ^(len * m);

    auto k = static_cast<uint64_t>(x);

    k *= m;
    k ^= k >> r;
    k *= m;

    h ^= k;
    h *= m * m;

    h ^= h >> r;
    h *= m;
    h ^= h >> r;

    return h;
}
