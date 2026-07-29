#define ANKERL_NANOBENCH_IMPLEMENT
#include <nanobench.h>

#include <csd/csd.hpp>

int main() {
    // Fast operations (<50ns) - need many iterations per epoch to beat Windows
    // scheduler tick (~15ms). Each epoch needs >= 100ms to average out noise.
    {
        ankerl::nanobench::Bench bench;
        bench.title("CSD Benchmarks (fast)")
            .unit("op")
            .warmup(10000)
            .epochs(30)
            .minEpochIterations(10000000);

        bench.run("to_decimal('+00-00.+0')", [&] {
            auto result = csd::to_decimal("+00-00.+0");
            ankerl::nanobench::doNotOptimizeAway(result);
        });

        bench.run("to_decimal_i('+00-00')", [&] {
            auto result = csd::to_decimal_i("+00-00");
            ankerl::nanobench::doNotOptimizeAway(result);
        });
    }

    // Medium operations (50-200ns)
    {
        ankerl::nanobench::Bench bench;
        bench.title("CSD Benchmarks (medium)")
            .unit("op")
            .warmup(5000)
            .epochs(50)
            .minEpochIterations(2000000);

        bench.run("to_csd_i(28)", [&] {
            auto result = csd::to_csd_i(28);
            ankerl::nanobench::doNotOptimizeAway(result);
        });

        bench.run("to_csdnnz(28.5, 4)", [&] {
            auto result = csd::to_csdnnz(28.5, 4);
            ankerl::nanobench::doNotOptimizeAway(result);
        });

        bench.run("to_csd(0.0, 10) [zero]", [&] {
            auto result = csd::to_csd(0.0, 10);
            ankerl::nanobench::doNotOptimizeAway(result);
        });

        bench.run("to_csd(0.5, 10) [small]", [&] {
            auto result = csd::to_csd(0.5, 10);
            ankerl::nanobench::doNotOptimizeAway(result);
        });
    }

    // Slow operations (>200ns) - fewer iterations suffice
    {
        ankerl::nanobench::Bench bench;
        bench.title("CSD Benchmarks (slow)")
            .unit("op")
            .warmup(2000)
            .epochs(80)
            .minEpochIterations(500000);

        bench.run("to_csd(28.5, 10)", [&] {
            auto result = csd::to_csd(28.5, 10);
            ankerl::nanobench::doNotOptimizeAway(result);
        });

        bench.run("to_csd(-28.5, 10) [neg]", [&] {
            auto result = csd::to_csd(-28.5, 10);
            ankerl::nanobench::doNotOptimizeAway(result);
        });

        bench.run("to_csd(1024.75, 10) [large]", [&] {
            auto result = csd::to_csd(1024.75, 10);
            ankerl::nanobench::doNotOptimizeAway(result);
        });
    }
}
