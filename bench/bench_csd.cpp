#include <chrono>
#include <cstdio>
#include <csd/csd.hpp>
#include <string>

template <typename F>
void bench(const char* name, F&& f, int iterations = 100000) {
    // warmup
    for (int i = 0; i < 1000; ++i) f();
    auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < iterations; ++i) f();
    auto end = std::chrono::steady_clock::now();
    auto ns = std::chrono::duration<double, std::nano>(end - start).count() / iterations;
    std::printf("  %-30s %8.1f ns/op  (%d iters)\n", name, ns, iterations);
}

int main() {
    std::printf("=== CSD Benchmarks (C++) ===\n");
    bench("to_csd(28.5, 10)", []() { csd::to_csd(28.5, 10); });
    bench("to_csd_i(28)", []() { csd::to_csd_i(28); });
    bench("to_csdnnz(28.5, 4)", []() { csd::to_csdnnz(28.5, 4); });
    bench("to_decimal('+00-00.+0')", []() { csd::to_decimal("+00-00.+0"); });
    bench("to_decimal_i('+00-00')", []() { csd::to_decimal_i("+00-00"); });
    bench("to_csd(0.0, 10) [zero]", []() { csd::to_csd(0.0, 10); });
    bench("to_csd(-28.5, 10) [neg]", []() { csd::to_csd(-28.5, 10); });
    bench("to_csd(0.5, 10) [small]", []() { csd::to_csd(0.5, 10); });
    bench("to_csd(1024.75, 10) [large]", []() { csd::to_csd(1024.75, 10); });
}
