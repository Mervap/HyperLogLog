//
// Created by dumpling on 25.05.19.
//

#include <gtest/gtest.h>
#include <vector>
#include <random>
#include <climits>
#include <algorithm>
#include <chrono>
#include "hyper_log_log.h"

double relative_error(int expected, int got) {
    return abs(got - expected) / (double) expected;
}

TEST(Vk_test, vk_test) {
    std::random_device rd;
    std::mt19937 gen(rd());

    const int N = (int) 2e6;
    for (int k : {2, 20, 2000, 20000, N / 10, N, N * 10}) {
        std::uniform_int_distribution<> dis(-k, k);
        std::set<int> all;
        Hyper_log_log counter;
        for (int i = 0; i < N; i++) {
            int value = dis(gen);
            all.insert(value);
            counter.add(value);
        }

        int expected = (int) all.size();
        int counter_result = counter.get_uniq_num();
        double error = relative_error(expected, counter_result);
        printf("%d numbers in range [1 .. %d], %d uniq, %d result, %.5f relative error\n", N, k, expected,
               counter_result, error);
        ASSERT_LE(error, 0.1);
    }
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}