/// @file csd.cpp
/**
 Canonical Signed Digit Functions

 Handles:
  * Decimals
  *
  *

 eg, +00-00+000.0 or 0.+0000-00+
 Where: '+' is +1
        '-' is -1

 Harnesser
 License: GPL2
*/

#include <cmath>   // for fabs, pow, ceil, log2
#include <cstdint> // for uint32_t
#include <iosfwd>  // for string
#include <string>  // for basic_string

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
 * @param x The parameter `x` is an unsigned 32-bit integer.
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
 * @param num The `num` parameter is a double precision floating-point number
 * that represents the value to be converted to CSD (Canonic Signed Digit)
 * representation.
 * @param places The `places` parameter in the `to_csd` function represents the
 * number of decimal places to include in the CSD (Canonical Signed Digit)
 * representation of the given `num`.
 *
 * @return The function `to_csd` returns a string representation of the given
 * `num` in Canonical Signed Digit (CSD) format.
 */
auto to_csd(double num, const int places) -> string {
    if (num == 0.0) {
        return "0";
    }
    auto absnum = fabs(num);
    int rem{0};
    string csd{"0"};
    if (absnum >= 1.0) {
        rem = int(ceil(log2(absnum * 1.5)));
        csd = string{""};
    }

    auto p2n = pow(2.0, rem);
    // auto const eps = pow(2.0, -places);
    while (rem > -places) {
        if (rem == 0) {
            csd += '.';
        }
        p2n /= 2.0;
        rem -= 1;
        auto const det = 1.5 * num;
        if (det > p2n) {
            csd += '+';
            num -= p2n;
        } else {
            if (det < -p2n) {
                csd += '-';
                num += p2n;
            } else {
                csd += '0';
            }
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
 * @param num The parameter `num` is an integer that represents the number for
 * which we want to generate the CSD (Canonical Signed Digit) representation.
 *
 * @return The function `to_csd_i` returns a string.
 */
auto to_csd_i(int num) -> string {
    if (num == 0) {
        return "0";
    }
    // auto p2n = int(pow(2.0, ceil(log2(abs(num) * 1.5))));
    auto temp = uint32_t(abs(num)) * 3 / 2;
    auto p2n = highest_power_of_two_in(temp) * 2;
    string csd("");
    while (p2n > 1) {
        auto const p2n_half = p2n / 2;
        auto const det = 3 * num;
        if (det > int(p2n)) {
            csd += '+';
            num -= p2n_half;
        } else if (det < -int(p2n)) {
            csd += '-';
            num += p2n_half;
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
 * The function `to_csdfixed` converts a given number into a CSD (Canonic Signed
 * Digit) representation with a specified number of non-zero digits.
 *
 * @param num The parameter `num` is a double precision floating-point number
 * that represents the input value for conversion to CSD (Canonic Signed Digit)
 * fixed-point representation.
 * @param nnz The parameter `nnz` stands for "number of non-zero bits". It
 * represents the maximum number of non-zero bits allowed in the output CSD
 * (Canonical Signed Digit) representation of the given `num`.
 *
 * @return The function `to_csdfixed` returns a string representation of the
 * given `num` in Canonical Signed Digit (CSD) format.
 */
auto to_csdfixed(double num, unsigned int nnz) -> string {
    if (num == 0.0) {
        return "0";
    }
    auto const absnum = fabs(num);
    int rem{0};
    string csd{"0"};
    if (absnum >= 1.0) {
        rem = int(ceil(log2(absnum * 1.5)));
        csd = string{""};
    }

    auto p2n = pow(2.0, rem);
    while (rem > 0 || (nnz > 0 && fabs(num) > 1e-100)) {
        if (rem == 0) {
            csd += '.';
        }
        p2n /= 2.0;
        rem -= 1;
        auto const det = 1.5 * num;
        if (det > p2n) {
            csd += '+';
            num -= p2n;
            nnz -= 1;
        } else {
            if (det < -p2n) {
                csd += '-';
                num += p2n;
                nnz -= 1;
            } else {
                csd += '0';
            }
        }
        if (nnz == 0) {
            num = 0.0;
        }
    }
    return csd;
}
} // namespace csd
