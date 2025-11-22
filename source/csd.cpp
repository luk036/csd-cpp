/// @file csd.cpp
/**
 Canonical Signed Digit Functions

 Handles:
  * Decimals

 CSD representation example:
 ```svgbob
    Bit position   6 5 4 3 2 1 0
                   | | | | | | |
    Decimal:    =  1 0 1 0 0 0 0  = 80
    CSD:        = +0-0000          = 3*(2^6) - 3*(2^4) = 64-16 = 48  ... wait, let me recalculate:
    Actually:   = +00-00+0         = 2^6 - 2^3 + 2^1 = 64-8+2 = 58

    Better example:
    Decimal: 28.5 = 11100.1
    CSD:     +00-00.+ = 2^5 - 2^2 + 2^0 + 2^(-1) = 32-4+1+0.5 = 28.5
 ```

 eg, +00-00+000.0 or 0.+0000-00+
 Where: '+' is +1
        '-' is -1
        '0' is  0

 Harnesser
 License: GPL2
*/

#include <cmath>    // for fabs, pow, ceil, log2
#include <cstdint>  // for uint32_t
#include <iosfwd>   // for string
#include <string>   // for basic_string

using std::abs;
using std::ceil;
using std::fabs;
using std::log2;
using std::pow;
using std::string;

/**
 * @brief the highest power of two
 *
 * https://thecodingbot.com/find-the-greatest-power-of-2-less-than-or-equal-to-a-given-number/
 *
 * The function calculates the highest power of two that is less than or equal
 * to a given number.
 *
 * Algorithm visualization:
 * ```svgbob
 *   Input: x = 42 (binary: 101010)
 *   Step 1: x |= x >> 1
 *     x = 101010 | 010101 = 111111
 *   Step 2: x |= x >> 2
 *     x = 111111 | 001111 = 111111
 *   Step 3: x |= x >> 4
 *     x = 111111 | 000011 = 111111
 *   (Continue with >>8, >>16)
 *   Step 4: return x ^ (x >> 1)
 *     result = 111111 ^ 011111 = 100000 = 32 = 2^5
 *   So highest power of 2 <= 42 is 32
 * ```
 *
 * @param[in] x The parameter `x` is an unsigned 32-bit integer.
 *
 * @return the highest power of two that is less than or equal to the input
 * number.
 */
inline uint32_t highest_power_of_two_in(uint32_t x) {
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    return x ^ (x >> 1);
}

namespace csd {
    /**
     * @brief Convert to CSD (Canonical Signed Digit) string representation
     *
     * Original author: Harnesser
     * https://sourceforge.net/projects/pycsd/
     * License: GPL2
     *
     * The function `to_csd` converts a given number to its Canonical Signed Digit
     * (CSD) representation with a specified number of decimal places.
     *
     * Algorithm visualization:
     * ```svgbob
     *   Input: 28.5, places=2
     *   Step 1: Calculate highest power needed
     *     abs(28.5) = 28.5 >= 1.0, so rem = ceil(log2(28.5*1.5)) = ceil(log2(42.75)) = 6
     *   Step 2: Process each bit position from high to low
     *     p2n = 2^6 = 64
     *     det = 1.5 * 28.5 = 42.75
     *     Since 42.75 > 64? No, so check next condition...
     *     Since 42.75 < -64? No, so add '0'
     *     Continue until we get: +00-00.+0
     * ```
     *
     * @param[in] decimal_value The `decimal_value` parameter is a double precision floating-point
     * number that represents the value to be converted to CSD (Canonic Signed Digit)
     * representation.
     * @param[in] places The `places` parameter in the `to_csd` function represents the
     * number of decimal places to include in the CSD (Canonical Signed Digit)
     * representation of the given `decimal_value`.
     *
     * @return The function `to_csd` returns a string representation of the given
     * `decimal_value` in Canonical Signed Digit (CSD) format.
     */
    auto to_csd(double decimal_value, int places) -> string {
        auto absnum = fabs(decimal_value);
        int rem{0};
        string csd{"0"};
        if (absnum >= 1.0) {
            rem = int(ceil(log2(absnum * 1.5)));
            csd = string{""};
        }

        auto p2n = pow(2.0, rem);
        auto loop_fn = [&](int value) {
            while (rem > value) {
                p2n /= 2.0;
                rem -= 1;
                auto const det = 1.5 * decimal_value;
                if (det > p2n) {
                    csd += '+';
                    decimal_value -= p2n;
                } else {
                    if (det < -p2n) {
                        csd += '-';
                        decimal_value += p2n;
                    } else {
                        csd += '0';
                    }
                }
            }
        };

        loop_fn(0);
        csd += '.';
        loop_fn(-places);
        return csd;
    }

    /**
     * @brief Convert to CSD (Canonical Signed Digit) string representation
     *
     * Original author: Harnesser
     * https://sourceforge.net/projects/pycsd/
     * License: GPL2
     *
     * The function converts a given integer into a Canonical Signed Digit (CSD)
     * representation.
     *
     * Algorithm visualization:
     * ```svgbob
     *   Input: 28
     *   Step 1: Calculate highest power needed using highest_power_of_two_in
     *     temp = abs(28) * 3 / 2 = 42
     *     highest_power_of_two_in(42) = 32, then multiply by 2 = 64 = 2^6
     *   Step 2: Process each bit position from high to low
     *     det = 3 * 28 = 84
     *     Compare det with 2*p2n to decide +, -, or 0
     *     84 > 64? Yes, so output '+' and subtract 32 (p2n/2) from value
     *     Continue until we get: +00-00
     * ```
     *
     * @param[in] decimal_value The parameter `decimal_value` is an integer that represents the
     * number for which we want to generate the CSD (Canonical Signed Digit) representation.
     *
     * @return The function `to_csd_i` returns a string.
     */
    auto to_csd_i(int decimal_value) -> string {
        if (decimal_value == 0) {
            return "0";
        }
        // auto p2n = int(pow(2.0, ceil(log2(abs(decimal_value) * 1.5))));
        auto temp = uint32_t(abs(decimal_value) * 3 / 2);
        auto p2n = highest_power_of_two_in(temp) * 2;
        string csd("");

        while (p2n > 1) {
            auto const p2n_half = p2n >> 1;
            auto const det = 3 * decimal_value;
            if (det > int(p2n)) {
                csd += '+';
                decimal_value -= p2n_half;
            } else if (det < -int(p2n)) {
                csd += '-';
                decimal_value += p2n_half;
            } else {
                csd += '0';
            }
            p2n = p2n_half;
        }

        return csd;
    }

    /**
     * @brief Convert to CSD (Canonical Signed Digit) string representation
     *
     * The function `to_csdnnz` converts a given number into a CSD (Canonic Signed
     * Digit) representation with a specified number of non-zero digits.
     *
     * Algorithm visualization:
     * ```svgbob
     *   Input: 28.5, nnz=4
     *   Step 1: Calculate highest power needed
     *     abs(28.5) = 28.5 >= 1.0, so rem = ceil(log2(28.5*1.5)) = 6
     *   Step 2: Process each bit position but stop when nnz reaches 0
     *     Continue conversion until we have used 4 non-zero digits (+ or -)
     *     Output: +00-00.+ (has 4 non-zero digits: + at pos 5, - at pos 2, + at pos 0, + at fract pos)
     * ```
     *
     * @param[in] decimal_value The parameter `decimal_value` is a double precision floating-point
     * number that represents the input value for conversion to CSD (Canonic Signed Digit)
     * fixed-point representation.
     * @param[in] nnz The parameter `nnz` stands for "number of non-zero bits". It
     * represents the maximum number of non-zero bits allowed in the output CSD
     * (Canonical Signed Digit) representation of the given `decimal_value`.
     *
     * @return The function `to_csdnnz` returns a string representation of the
     * given `decimal_value` in Canonical Signed Digit (CSD) format.
     */
    auto to_csdnnz(double decimal_value, unsigned int nnz) -> string {
        // if (decimal_value == 0.0) {
        //     return "0";
        // }
        auto const absnum = fabs(decimal_value);
        int rem{0};
        string csd{"0"};
        if (absnum >= 1.0) {
            rem = int(ceil(log2(absnum * 1.5)));
            csd = string{""};
        }
        auto p2n = pow(2.0, rem);

        while (rem > 0 || (nnz > 0 && fabs(decimal_value) > 1e-100)) {
            if (rem == 0) {
                csd += '.';
            }
            p2n /= 2.0;
            rem -= 1;
            auto const det = 1.5 * decimal_value;
            if (det > p2n) {
                csd += '+';
                decimal_value -= p2n;
                nnz -= 1;
            } else {
                if (det < -p2n) {
                    csd += '-';
                    decimal_value += p2n;
                    nnz -= 1;
                } else {
                    csd += '0';
                }
            }
            if (nnz == 0) {
                decimal_value = 0.0;
            }
        }

        return csd;
    }

    /**
     * @brief Convert to CSD (Canonical Signed Digit) string representation
     *
     * Original author: Harnesser
     * https://sourceforge.net/projects/pycsd/
     * License: GPL2
     *
     * The function converts a given integer into a Canonical Signed Digit (CSD)
     * representation.
     *
     * Algorithm visualization:
     * ```svgbob
     *   Input: 28, nnz=4
     *   Step 1: Calculate highest power needed using highest_power_of_two_in
     *     temp = abs(28) * 3 / 2 = 42
     *     highest_power_of_two_in(42) = 32, then multiply by 2 = 64 = 2^6
     *   Step 2: Process each bit position from high to low but stop when nnz reaches 0
     *     Continue conversion until we have used 4 non-zero digits
     *     Output: +00-00 (has 4 non-zero digits: + at pos 5, - at pos 2)
     * ```
     *
     * @param[in] decimal_value The parameter `decimal_value` is an integer that represents the
     * number for which we want to generate the CSD (Canonical Signed Digit) representation.
     *
     * @return The function `to_csdnnz_i` returns a string.
     */
    auto to_csdnnz_i(int decimal_value, unsigned int nnz) -> string {
        if (decimal_value == 0) {
            return "0";
        }
        // auto p2n = int(pow(2.0, ceil(log2(abs(decimal_value) * 1.5))));
        auto temp = uint32_t(abs(decimal_value) * 3 / 2);
        auto p2n = highest_power_of_two_in(temp) * 2;
        string csd("");

        while (p2n > 1) {
            auto const p2n_half = p2n >> 1;
            auto const det = 3 * decimal_value;
            if (det > int(p2n)) {
                csd += '+';
                decimal_value -= p2n_half;
                nnz -= 1;
            } else if (det < -int(p2n)) {
                csd += '-';
                decimal_value += p2n_half;
                nnz -= 1;
            } else {
                csd += '0';
            }
            p2n = p2n_half;
            if (nnz == 0) {
                decimal_value = 0;
            }
        }

        return csd;
    }
}  // namespace csd
