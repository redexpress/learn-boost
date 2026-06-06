#include <iostream>
#include <array>
#include <algorithm>
#include <random>
#include <chrono>

#include <boost/sort/sort.hpp>
#include <boost/sort/spreadsort/integer_sort.hpp>   // for integer_sort
#include "sort.h"

constexpr size_t N = 220000;

// generate random integer, range [1, max_value]
void generateData(std::array<int, N>& arr, int max_value) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, max_value);
    for (auto& x : arr) {
        x = dist(gen);
    }
}

// Returns elapsed microseconds for invoking `body`.
template <typename Body>
long long measure(Body&& body) {
    auto t = std::chrono::steady_clock::now();
    body();
    auto dur = std::chrono::steady_clock::now() - t;
    return std::chrono::duration_cast<std::chrono::microseconds>(dur).count();
}

// Run a sort on a fresh copy of `data` and report its time.
template <typename SortCall>
long long run_and_measure(const std::array<int, N>& data,
                        const char* label,
                        SortCall&& call) {
    std::array<int, N> copy = data;
    long long us = measure([&] { call(copy); });
    std::cout << label << ": " << us << " us\n";
    return us;
}

void use_sort() {
    std::array<int, N> data;
    generateData(data, 200000);

    std::cout << "Sorting benchmark on int (N = " << N << ")\n";
    std::cout << "--------------------------------------------\n";

    // --- std::sort (baseline) ---
    run_and_measure(data, "std::sort                       ", [](auto& c) {
        std::sort(c.begin(), c.end());
    });
    run_and_measure(data, "std::stable_sort                ", [](auto& c) {
        std::stable_sort(c.begin(), c.end());
    });

    // --- Boost sort algorithms (single-threaded) ---
    run_and_measure(data, "boost::sort::pdqsort            ", [](auto& c) {
        boost::sort::pdqsort(c.begin(), c.end());
    });
    run_and_measure(data, "boost::sort::spinsort           ", [](auto& c) {
        boost::sort::spinsort(c.begin(), c.end());
    });
    run_and_measure(data, "boost::sort::spreadsort::integer_sort", [](auto& c) {
        boost::sort::spreadsort::integer_sort(c.begin(), c.end());
    });

    // --- Parallel algorithms (need -pthread) ---
    run_and_measure(data, "boost::sort::block_indirect_sort", [](auto& c) {
        boost::sort::block_indirect_sort(c.begin(), c.end());
    });
    run_and_measure(data, "boost::sort::parallel_stable_sort", [](auto& c) {
        boost::sort::parallel_stable_sort(c.begin(), c.end());
    });
    run_and_measure(data, "boost::sort::sample_sort       ", [](auto& c) {
        boost::sort::sample_sort(c.begin(), c.end());
    });
}