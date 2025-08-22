/// @file csd.hpp
#pragma once

#include <iosfwd>     // for string
#include <stdexcept>  // for invalid_argument
#include <string>     // for basic_string, operator==, operator<<

#if __cpp_constexpr >= 201304
#    define CONSTEXPR14 constexpr
#else
#    define CONSTEXPR14 inline
#endif

namespace csd {

    /**
     * Converts a double precision floating point number to a string
     * representation in Canonical Signed Digit (CSD) format with a
     * specified number of decimal places.
     *
     * Original author: Harnesser
     * https://sourceforge.net/projects/pycsd/
     * License: GPL2
     *
     * @param[in] decimal_value - The number to convert to CSD format.
     * @param[in] places - The number of decimal places to include in the CSD representation.
     * @return String representation of the input number in CSD format.
     */
    extern auto to_csd(double decimal_value, int places) -> std::string;

    /**
     * Converts an integer to a string representation in Canonical Signed Digit (CSD) format.
     *
     * Original author: Harnesser
     * https://sourceforge.net/projects/pycsd/
     * License: GPL2
     *
     * @param[in] decimal_value - The integer to convert to CSD format.
     * @return String representation of the input integer in CSD format.
     */
    extern auto to_csd_i(int decimal_value) -> std::string;

    /**
     * Converts a double precision floating point number to a CSD (Canonical Signed Digit)
     * string representation with a fixed number of non-zero digits.
     *
     * This is an exported API function.
     *
     * @param[in] decimal_value - The number to convert to CSD format.
     * @param[in] nnz - The maximum number of non-zero digits allowed in the CSD representation.
     * @return String representation of the input number in CSD format with nnz non-zero digits.
     */
    extern auto to_csdnnz(double decimal_value, unsigned int nnz) -> std::string;

    /**
     * Converts a double precision floating point number to a CSD (Canonical Signed Digit)
     * string representation with a fixed number of non-zero digits.
     *
     * This is an exported API function.
     *
     * @param[in] decimal_value - The integer to convert to CSD format.
     * @param[in] nnz - The maximum number of non-zero digits allowed in the CSD representation.
     * @return String representation of the input number in CSD format with nnz non-zero digits.
     */
    extern auto to_csdnnz_i(int decimal_value, unsigned int nnz) -> std::string;

    /**
     * Converts a CSD string to a double precision decimal number
     * using a switch statement.
     *
     * This is an internal implementation detail, not part of the public API.
     *
     * @param[in] csd The parameter `csd` is a pointer to a character array, which
     * represents the input string. It is assumed that the string is
     * null-terminated.
     * @return double decimal value of the CSD format
     */
    CONSTEXPR14 auto to_decimal_using_switch(const char *csd) -> double {
        auto decimal_value = 0.0;
        // Handle integral part
        for (; *csd != '.' && *csd != '\0'; ++csd) {
            switch (*csd) {
                case '0':
                    decimal_value *= 2.0;
                    break;
                case '+':
                    decimal_value = 2.0 * decimal_value + 1.0;
                    break;
                case '-':
                    decimal_value = 2.0 * decimal_value - 1.0;
                    break;
                // case '.':
                //     break;
                // case '\0':
                //     break;
                default:
                    throw std::invalid_argument("Work with 0, +, -, and . only");
            }
        }
        if (*csd == '\0') {
            return decimal_value;
        }
        // Handle fractional part
        auto scale = 0.5;
        for (++csd; *csd != '\0'; ++csd) {
            switch (*csd) {
                case '0':
                    break;
                case '+':
                    decimal_value += scale;
                    break;
                case '-':
                    decimal_value -= scale;
                    break;
                // case '\0':
                //     break;
                default:
                    throw std::invalid_argument("Fractional part work with 0, +, and - only");
            }
            scale /= 2;
        }
        return decimal_value;
    }

    /**
     * @brief Convert the integral part of a CSD string to a decimal
     *
     * This function takes a pointer to a null-terminated CSD string, which represents a
     * number in canonical signed digit format. It iterates through the string, processing
     * each character and accumulating a decimal value for just the integral part of the
     * number.
     *
     * For each '0' digit, it multiplies the current value by 2.
     * For each '+' digit, it multiplies the current value by 2 and adds 1.
     * For each '-' digit, it multiplies the current value by 2 and subtracts 1.
     *
     * It stops when it reaches either a '.' or '\0' character, as those mark the end of
     * the integral part.
     *
     * It throws an exception if any invalid character is encountered.
     *
     * @param[in] csd - Pointer to the null-terminated CSD string
     * @return The decimal value of the integral part
     */
    CONSTEXPR14 auto to_decimal_integral(const char *&csd) -> int {
        auto decimal_value = 0;

        for (;; ++csd) {
            auto digit = *csd;
            if (digit == '0') {
                decimal_value <<= 1;
            } else if (digit == '+') {
                decimal_value = (decimal_value << 1) + 1;
            } else if (digit == '-') {
                decimal_value = (decimal_value << 1) - 1;
            } else if (digit == '.' || digit == '\0') {
                break;
            } else {
                throw std::invalid_argument("Work with 0, +, -, and . only");
            }
        }

        return decimal_value;
    }

    /**
     * @brief Convert the fractional part of a CSD string to a decimal
     *
     * This function takes a pointer to a CSD string that contains a fractional part,
     * starting after the '.' character. It iterates through the fractional digits,
     * keeping track of a scaling factor. For each '+' it adds, and for each '-' it
     * subtracts, a fraction of the current scale. This builds up the fractional part
     * of the final decimal number.
     */
    CONSTEXPR14 auto to_decimal_fractional(const char *csd) -> double {
        auto decimal_value = 0.0;
        auto scale = 0.5;

        for (++csd;; ++csd) {
            auto digit = *csd;
            if (digit == '0') {
            } else if (digit == '+') {
                decimal_value += scale;
            } else if (digit == '-') {
                decimal_value -= scale;
            } else if (digit == '\0') {
                break;
            } else {
                throw std::invalid_argument("Fractional part work with 0, +, and - only");
            }
            scale /= 2.0;
        }

        return decimal_value;
    }

    /**
     * @brief Convert the CSD string to a decimal
     *
     * This function takes a CSD (Canonical Signed Digit) string as input and converts
     * it to a decimal number. It iterates through the characters of the string and
     * performs operations based on each character to build up the decimal value.
     *
     * The integral part is processed first, multiplying the value by 2 for each '0' and
     * adding/subtracting 1 for '+'/'-'.
     *
     * Then the fractional part is processed, starting with a scale of 0.5 and adding/
     * subtracting fractions of that scale based on the '+/-' digits.
     *
     * The function throws an exception if any invalid characters are encountered.
     *
     * @param[in] csd - Pointer to the null-terminated CSD string to convert
     * @return The decimal value of the CSD string
     */
    CONSTEXPR14 auto to_decimal(const char *csd) -> double {
        auto integral = to_decimal_integral(csd);

        if (*csd == '\0') {
            return double(integral);
        }

        auto fractional = to_decimal_fractional(csd);
        return double(integral) + fractional;
    }

    /**
     * @brief Convert the CSD string to a decimal
     *
     * The function `to_decimal_i` takes a CSD (Canonical Signed Digit) string as
     * input and converts it to an integer. It iterates through the characters of
     * the string and performs the corresponding operations based on the character.
     *
     * @param[in] csd The parameter `csd` is a pointer to a character array, which
     * represents the input string. It is assumed that the string is
     * null-terminated.
     * @return int decimal value of the CSD format
     */
    CONSTEXPR14 auto to_decimal_i(const char *csd) -> int { return to_decimal_integral(csd); }
}  // namespace csd
