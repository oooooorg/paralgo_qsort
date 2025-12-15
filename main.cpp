//
// Created by Tsvetkov Alexey on 12/14/25.
//


#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <random>
#include <parlay/parallel.h>
#include <parlay/sequence.h>
#include "quick_sort.h"

constexpr size_t ELEMENTS = 100000000;
constexpr int RUNS_COUNT = 5;

std::vector<int> generate_data(size_t n) {
    std::vector<int> data(n);
    parlay::parallel_for(0, n, [&](size_t i) {
        thread_local std::mt19937 generator(std::random_device{}() + i);
        std::uniform_int_distribution<> dist(1, INT32_MAX);
        data[i] = dist(generator);
    });
    return data;
}

template<typename Func>
double measure_time(Func func) {
    const auto start = std::chrono::high_resolution_clock::now();
    func();
    const auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double>(end - start).count();
}

int main() {
    parlay::sequence<int> test_seq = {
        10, 5, 2, 3, 7, 10, 1, 9, 4, 8, 623523, 234, 234, 14124, 4353, 234, 4, 342, 124, 235, 12, 41, 25, 23, 5, 2, 35,
        23, 5, 1, 25, 436, 23,
    };
    par_qsort(test_seq);

    if (!std::is_sorted(test_seq.begin(), test_seq.end())) {
        std::cerr << "Seq sort not correct test #1" << std::endl;
        return 1;
    }

    test_seq = {
        1, 2, 3, 4, 5, 9, 10, 8, 7, 6, 5, 4, 3, 2, 1, 7, 8, 9, 10, 4, 5, 6, 7, 8, 9, 10, 1231, 123, 124, 125, 126, 127,
        2349, 12, 89, 123, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69,
        69, 69, 69, 69, 69, 69, 69
    };

    par_qsort(test_seq);

    if (!std::is_sorted(test_seq.begin(), test_seq.end())) {
        std::cerr << "Par sort not correct test #2" << std::endl;
        return 1;
    }

    std::vector<int> test_vec = {
        10, 5, 2, 3, 7, 10, 1, 9, 4, 8, 623523, 234, 234, 14124, 4353, 234, 4, 342, 124, 235, 12, 41, 25, 23, 5, 2, 35,
        23, 5, 1, 25, 436, 23,
    };
    seq_qsort(test_vec);
    if (!std::is_sorted(test_vec.begin(), test_vec.end())) {
        std::cerr << "Seq sort not correct test #1" << std::endl;
        return 1;
    }

    test_vec = {
        1, 2, 3, 4, 5, 9, 10, 8, 7, 6, 5, 4, 3, 2, 1, 7, 8, 9, 10, 4, 5, 6, 7, 8, 9, 10, 1231, 123, 124, 125, 126, 127,
        2349, 12, 89, 123, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69,
        69, 69, 69, 69, 69, 69, 69
    };

    seq_qsort(test_vec);

    if (!std::is_sorted(test_seq.begin(), test_seq.end())) {
        std::cerr << "Seq sort not correct test #2" << std::endl;
        return 1;
    }

    std::cout << "Par and seq sorts are correct\n" << std::endl;

    std::vector<int> master_data = generate_data(ELEMENTS);

    std::cout << std::fixed << std::setprecision(4);

    double seq_total = 0;
    std::cout << "Seq sort test 100kk bench." << std::endl;
    for (int i = 0; i < 5; ++i) {
        std::vector<int> seq_buffer = master_data;
        double t = measure_time([&]() {
            seq_qsort(seq_buffer);
        });
        std::cout << "Run " << i + 1 << ": " << t << "s" << std::endl;
        seq_total += t;
    }
    double seq_avg = seq_total / RUNS_COUNT;
    std::cout << "AVG: " << seq_avg << "s\n" << std::endl;

    double par_total = 0;
    std::cout << "Par sort test 100kk bench (4 threads)." << std::endl;

    parlay::sequence<int> par_result;

    for (int i = 0; i < RUNS_COUNT; ++i) {
        parlay::sequence<int> par_input(master_data.begin(), master_data.end());

        double t = measure_time([&]() {
            par_qsort(par_input);
        });

        std::cout << "Run " << i + 1 << ": " << t << "s" << std::endl;
        par_total += t;

        par_result.clear();
    }
    double par_avg = par_total / RUNS_COUNT;
    std::cout << "AVG: " << par_avg << "s\n" << std::endl;

    std::cout << "Speedup: " << seq_avg / par_avg << "x" << std::endl;

    return 0;
}
