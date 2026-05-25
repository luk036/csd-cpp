/// @file csd_multiplier.hpp
/// @brief Generate Verilog code for CSD multiplier
///
/// Translates a Canonical Signed Digit (CSD) string into a Verilog hardware
/// module that implements multiplication using only shift-and-add/subtract
/// operations — no actual multipliers needed. Each non-zero CSD digit becomes
/// a shifted term, and the terms are summed.
///
/// Example:
///   csd = "+00-00+0" → result = x_shift7 - x_shift4 + x_shift1
///
/// @see https://github.com/luk036/csd-cpp
/// @par License
/// GPL2
#pragma once

#include <string>       // for string
#include <vector>       // for vector

namespace csd {

    /**
     * @brief Specification for a single CSD multiplier coefficient
     */
    struct MultiplierSpec {
        std::string name;        ///< Output port name
        std::string csd;         ///< CSD string ('+', '-', '0')
        int input_width;         ///< Bit width of input x
        int max_power;           ///< Highest power (len(csd)-1)
    };

    /**
     * @brief Generate Verilog code for a single CSD multiplier module
     *
     * Converts a Canonical Signed Digit (CSD) string into a synthesizable
     * Verilog module that performs constant multiplication using shifts and
     * additions/subtractions.
     *
     * @param[in] csd_str CSD string using '+', '-', '0' (e.g. "+00-00+0+")
     * @param[in] input_width Bit width of the input signal x
     * @param[in] max_power Highest power of two in the CSD (must be len(csd)-1)
     *
     * @return Verilog module code as a string
     *
     * @throws std::invalid_argument if csd_str length doesn't match max_power+1
     *         or if the string contains characters other than '+', '-', '0'
     */
    auto generate_csd_multiplier(const std::string& csd_str, int input_width,
                                 int max_power) -> std::string;

    /**
     * @brief Generate Verilog for multiple CSD multipliers with cross-CSE
     *
     * When the same CSD substring appears in multiple coefficients, a shared
     * sub-expression wire is created — reducing total adder count across the
     * entire filter.
     *
     * All coefficients **must** share the same ``input_width`` and ``max_power``
     * so that the same bit position encodes the same power of two.
     *
     * @param[in] coeffs List of coefficient specifications
     * @param[in] module_name Name for the generated Verilog module
     *
     * @return Verilog module code
     *
     * @throws std::invalid_argument on validation failure or width mismatch
     */
    auto generate_csd_multipliers(const std::vector<MultiplierSpec>& coeffs,
                                  const std::string& module_name = "csd_filter")
        -> std::string;

}  // namespace csd
