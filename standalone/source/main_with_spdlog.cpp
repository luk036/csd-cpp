#include <csd/csd.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <iostream>
#include <string>

int main() {
    // Create a file logger
    auto logger = spdlog::basic_logger_mt("file_logger", "csd_example.txt");
    spdlog::set_default_logger(logger);
    spdlog::set_level(spdlog::level::info);

    // Log some information
    spdlog::info("Starting CSD library example with spdlog");

    // Test some CSD conversions and log the results
    double test_value = 28.5;
    auto csd_result = csd::to_csd(test_value, 2);
    spdlog::info("CSD conversion: {} -> {}", test_value, csd_result);

    int test_int = 28;
    auto csd_int_result = csd::to_csd_i(test_int);
    spdlog::info("CSD integer conversion: {} -> {}", test_int, csd_int_result);

    auto decimal_result = csd::to_decimal(csd_result.c_str());
    spdlog::info("Decimal conversion back: {} -> {}", csd_result, decimal_result);

    // Test with different values
    for (int i = 1; i <= 10; ++i) {
        auto csd_val = csd::to_csd_i(i);
        auto dec_val = csd::to_decimal_i(csd_val.c_str());
        spdlog::debug("Value {}: CSD='{}', Decimal='{}'", i, csd_val, dec_val);
    }

    spdlog::info("Example completed successfully!");

    return 0;
}