/// @file csd_multiplier.cpp
/// @brief Generate Verilog code for CSD multiplier (experiment)
///
/// This experiment demonstrates csd::generate_csd_multiplier() with
/// LCSRe-based optimization. When the CSD string contains repeated patterns,
/// the generated Verilog shares hardware via a sub-expression wire.
///
/// @note This is an experimental file demonstrating CSD applications in hardware design

#include <csd/csd_multiplier.hpp>

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

auto main() -> int {
    try {
        // --- No repeated pattern (flat generation) ---
        std::cout << "=== +00-00+0 (no repeat) ===\n";
        std::cout << csd::generate_csd_multiplier("+00-00+0", 8, 7) << '\n';

        // --- Repeated pattern "+0-0" — optimized with _pat wire ---
        // Flat: x_shift7 - x_shift5 + x_shift3 - x_shift1  (4 adders)
        // Opt:  _pat + (_pat >>> 4)                         (2 adders + 1 shift)
        std::cout << "=== +0-0+0-0 (repeat: +0-0) ===\n";
        std::cout << csd::generate_csd_multiplier("+0-0+0-0", 8, 7) << '\n';

        // --- Triple repeat: +0-0+0-0+0-0 ---
        std::cout << "=== +0-0+0-0+0-0 (triple repeat) ===\n";
        std::cout << csd::generate_csd_multiplier("+0-0+0-0+0-0", 8, 11) << '\n';

        // --- Longer pattern: +00-00+00-00 ---
        std::cout << "=== +00-00+00-00 (repeat: +00-00) ===\n";
        std::cout << csd::generate_csd_multiplier("+00-00+00-00", 8, 11) << '\n';

        // --- Leading minus — also triggers LCSRe on "-0-" pattern ---
        // Flat: -x_shift2 - x_shift0    (2 adders, no repeat benefit)
        std::cout << "=== -0- (no repeat benefit) ===\n";
        std::cout << csd::generate_csd_multiplier("-0-", 8, 2) << '\n';

        // --- All zeros ---
        std::cout << "=== 000 (all zeros) ===\n";
        std::cout << csd::generate_csd_multiplier("000", 8, 2) << '\n';

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
