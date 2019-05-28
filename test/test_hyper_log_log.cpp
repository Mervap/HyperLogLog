//
// Created by dumpling on 25.05.19.
//

#include <gtest/gtest.h>
#include <vector>
#include <random>
#include <climits>
#include <algorithm>
#include <chrono>
#include <unordered_set>
#include "hyper_log_log.h"

double relative_error(int expected, int got) {
    return abs(got - expected) / (double) expected;
}

double run_vk_test(int min, int max) {

    static std::random_device rd;
    static std::mt19937 gen(rd());

    const int N = (int) 1e6;
    std::uniform_int_distribution<> dis(min, max);
    std::unordered_set<int> all;
    Hyper_log_log counter;
    for (int i = 0; i < N; i++) {
        int value = dis(gen);
        all.insert(value);
        counter.add(value);
    }

    int expected = (int) all.size();
    int counter_result = counter.get_uniq_num();
    double error = relative_error(expected, counter_result);

    printf("%d numbers in range [%d .. %d], %d uniq, %d result, %.5f relative error\n",
           N, min, max, expected, counter_result, error);

    return error;
}

std::chrono::nanoseconds
get_time(const std::chrono::steady_clock::time_point &st, const std::chrono::steady_clock::time_point &end) {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - st);
}

TEST(Errors, empty) {
    Hyper_log_log counter;
    ASSERT_EQ(counter.get_uniq_num(), 0);
}

TEST(Errors, same_elements) {
    Hyper_log_log counter;

    for (int i = 0; i < (int) 1e6; ++i) {
        counter.add(1);
    }

    ASSERT_EQ(counter.get_uniq_num(), 1);
}

TEST(Errors, queries_without_adding) {
    int N = (int) 1e6;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(INT_MIN, INT_MAX);

    Hyper_log_log counter;

    for (int i = 0; i < N; ++i) {
        counter.add(dis(gen));
    }

    auto ans = counter.get_uniq_num();
    for (int i = 0; i < N; ++i) {
        auto ans1 = counter.get_uniq_num();
        ASSERT_EQ(ans, ans1);
    }
}

TEST(Vk_test, test1) {
    const int N = (int) 1e6;
    for (int k : {1, 10, 1000, 10000, N / 10, N, N * 10}) {
        double error = run_vk_test(1, k);
        ASSERT_LE(error, 0.1);
    }
}

TEST(Vk_test, test2) {
    for (int k = 1; k < 31; ++k) {
        double error = run_vk_test(-(1u << k), (1u << k));
        ASSERT_LE(error, 0.1);
    }

    double error = run_vk_test(INT_MIN, INT_MAX);
    ASSERT_LE(error, 0.1);
}


TEST(Performance, queries_without_adding) {
    int N = (int) 1e6;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(INT_MIN, INT_MAX);

    Hyper_log_log counter;

    for (int i = 0; i < N; ++i) {
        counter.add(dis(gen));
    }

    auto ans = counter.get_uniq_num();
    std::chrono::nanoseconds cnt = std::chrono::nanoseconds::zero();
    for (int i = 0; i < N; ++i) {
        auto st = std::chrono::steady_clock::now();
        auto ans1 = counter.get_uniq_num();
        auto end = std::chrono::steady_clock::now();
        cnt += get_time(st, end);
        ASSERT_EQ(ans, ans1);
    }

    ASSERT_LE(std::chrono::duration_cast<std::chrono::milliseconds>(cnt).count(), 50);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}