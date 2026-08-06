/// @file BM_switch.cpp
/// @brief Benchmark for CSD to decimal conversion methods
///
/// This benchmark compares two implementations of CSD to decimal conversion:
/// - using_if_else: Uses if-else branching
/// - using_switch: Uses switch statement
///
/// @note This benchmark is used to measure performance of different parsing strategies

#define ANKERL_NANOBENCH_IMPLEMENT
#include <nanobench.h>

#include <csd/csd.hpp>

using namespace csd;

int main() {
    ankerl::nanobench::Bench bench;
    bench.title("CSD to decimal conversion methods")
        .unit("op")
        .warmup(1000)
        .epochs(100)
        .minEpochIterations(1000000);

    bench.run("using_if_else", [&] {
        std::string test("+00-00+00+00-00+00+0-0+0+.+00+00-0++");
        auto result = to_decimal(test.c_str());
        ankerl::nanobench::doNotOptimizeAway(result);
    });

    bench.run("using_switch", [&] {
        std::string test("+00-00+00+00-00+00+0-0+0+.+00+00-0++");
        auto result = to_decimal_using_switch(test.c_str());
        ankerl::nanobench::doNotOptimizeAway(result);
    });
}
