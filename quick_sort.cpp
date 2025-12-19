//
// Created by Tsvetkov Alexey on 12/14/25.
//


#include "quick_sort.h"
#include <parlay/parallel.h>
#include <algorithm>
#include <vector>

constexpr size_t BLOCK = 512;

void par_qsort_impl(parlay::slice<int*, int*> arr) {
    if (arr.size() < BLOCK) {
        std::sort(arr.begin(), arr.end());
        return;
    }

    thread_local std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<size_t> dist(0, arr.size() - 1);

    int pivot = arr[dist(gen)];

    auto left = std::partition(arr.begin(), arr.end(),
        [pivot](const int x) { return x < pivot; });

    auto right = std::partition(left, arr.end(),
        [pivot](const int x) { return x == pivot; });

    parlay::par_do(
        [&]() { par_qsort_impl(parlay::make_slice(arr.begin(), left)); },
        [&]() { par_qsort_impl(parlay::make_slice(right, arr.end())); }
    );
}

void par_qsort(std::vector<int>& arr) {
    if (arr.empty()) return;
    auto data_ptr = arr.data();
    par_qsort_impl(parlay::make_slice(data_ptr, data_ptr + arr.size()));
}

int partition(std::vector<int>& arr, int low, int high) {
    int pivot = arr[high];

    int i = low - 1;

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }

    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

void seq_qsort_impl(std::vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        seq_qsort_impl(arr, low, pi - 1);
        seq_qsort_impl(arr, pi + 1, high);
    }
}

void seq_qsort(std::vector<int>& arr) {
    if (arr.empty()) return;
    seq_qsort_impl(arr, 0, arr.size() - 1);
}
