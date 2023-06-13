#include <benchmark/benchmark.h>

#include <csd/csd.hpp>

using namespace csd;

extern auto to_decimal(const std::string &csd_str) -> double;
extern auto to_decimal_using_switch(const std::string &csd_str) -> double;

static void using_if_else(benchmark::State &state) {
  // Code inside this loop is measured repeatedly
  for (auto _ : state) {
    std::string test("+00-00+00+0-0+0+.+00+00-0++");

    auto result = to_decimal(test.c_str());
    // Make sure the variable is not optimized away by compiler
    benchmark::DoNotOptimize(result);
  }
}
// Register the function as a benchmark
BENCHMARK(using_if_else);

static void using_switch(benchmark::State &state) {
  // Code inside this loop is measured repeatedly
  for (auto _ : state) {
    std::string test("+00-00+00+0-0+0+.+00+00-0++");

    auto result = to_decimal_using_switch(test.c_str());
    // Make sure the variable is not optimized away by compiler
    benchmark::DoNotOptimize(result);
  }
}
BENCHMARK(using_switch);

BENCHMARK_MAIN();
