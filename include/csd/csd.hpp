/// @file csd.hpp
/// @brief Canonical Signed Digit (CSD) representation library
///
/// This library provides functions for converting between decimal numbers and
/// Canonical Signed Digit (CSD) representation. CSD is a signed-digit representation
/// where each digit can be -1, 0, or 1, with no consecutive non-zero digits.
///
/// CSD representation is particularly useful in digital signal processing and
/// hardware design as it minimizes the number of non-zero digits, leading to
/// more efficient implementations using only adders and subtractors.
///
/// @author Harnesser
/// @see https://sourceforge.net/projects/pycsd/
/// @par License
/// GPL2
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

/// @defgroup csd_functions CSD Conversion Functions
/// @brief Functions for converting between decimal and CSD representations
/// @{

/// @brief Maximum number of decimal places supported by CSD conversion
constexpr int MAX_DECIMAL_PLACES = 20;

/// @brief Minimum value for non-zero digit count in CSDNNZ functions
constexpr unsigned int MIN_NONZERO_DIGITS = 1;

/// @brief Exception thrown when invalid CSD characters are encountered
/// @details This exception is thrown when functions encounter characters
/// other than '0', '+', '-', or '.' in CSD strings
class invalid_csd_format : public std::invalid_argument {
public:
    /// @brief Construct invalid CSD format exception
    /// @param message Error message describing the invalid format
    explicit invalid_csd_format(const std::string& message) 
        : std::invalid_argument(message) {}
};

    /**
     * @brief Convert a floating-point number to CSD representation
     * @ingroup csd_functions
     *
     * Converts a double precision floating point number to a string
     * representation in Canonical Signed Digit (CSD) format with a
     * specified number of decimal places.
     *
     * The CSD format ensures that:
     * - Each digit is either '0', '+', or '-'
     * - No two consecutive digits are non-zero
     * - The representation has the minimal number of non-zero digits
     *
     * Example:
     * @code
     * to_csd(28.5, 2) returns "+00-00.+"
     * // Calculation: 2^5 - 2^2 + 2^0 + 2^(-1) = 32 - 4 + 1 + 0.5 = 28.5
     * @endcode
     *
     * @param[in] decimal_value The number to convert to CSD format. Can be positive,
     *                         negative, or zero.
     * @param[in] places The number of decimal places to include in the CSD representation.
     *                  Must be non-negative. Use 0 for integer values.
     *
     * @return String representation of the input number in CSD format.
     *         The string contains only '0', '+', '-', and '.' characters.
     *
     * @throws std::invalid_argument If places is negative or if the conversion
     *                               fails due to numerical limitations.
     *
     * @see to_csd_i() for integer-only conversion
     * @see to_decimal() for reverse conversion
     */
    extern auto to_csd(double decimal_value, int places) -> std::string;

    /**
     * @brief Convert an integer to CSD representation
     * @ingroup csd_functions
     *
     * Converts an integer to a string representation in Canonical Signed Digit (CSD) format.
     * This function is optimized for integer values and produces a CSD string without
     * a decimal point.
     *
     * Example:
     * @code
     * to_csd_i(28) returns "+00-00"
     * // Calculation: 2^5 - 2^2 = 32 - 4 = 28
     * 
     * to_csd_i(0) returns "0"
     * @endcode
     *
     * @param[in] decimal_value The integer to convert to CSD format. Can be positive,
     *                         negative, or zero.
     *
     * @return String representation of the input integer in CSD format.
     *         The string contains only '0', '+', and '-' characters.
     *
     * @throws std::invalid_argument If the conversion fails due to numerical limitations.
     *
     * @see to_csd() for floating-point conversion
     * @see to_decimal_i() for reverse conversion
     */
    extern auto to_csd_i(int decimal_value) -> std::string;

    /**
     * @brief Convert a floating-point number to CSD with limited non-zero digits
     * @ingroup csd_functions
     *
     * Converts a double precision floating point number to a CSD (Canonical Signed Digit)
     * string representation with a fixed number of non-zero digits. This is useful for
     * applications where hardware resources are limited and you want to control the
     * complexity of the resulting representation.
     *
     * The function will stop adding non-zero digits once the specified limit is reached,
     * potentially resulting in an approximation of the original value.
     *
     * Example:
     * @code
     * to_csdnnz(28.5, 3) returns "+00-00.+"
     * // Uses only 3 non-zero digits: + at position 5, - at position 2, + at position 0
     * @endcode
     *
     * @param[in] decimal_value The number to convert to CSD format. Can be positive,
     *                         negative, or zero.
     * @param[in] nnz The maximum number of non-zero digits allowed in the CSD representation.
     *               Must be at least 1. Larger values produce more accurate representations.
     *
     * @return String representation of the input number in CSD format with at most
     *         nnz non-zero digits.
     *
     * @throws std::invalid_argument If nnz is 0 or if the conversion fails.
     *
     * @see to_csdnnz_i() for integer version
     * @see to_csd() for unlimited non-zero digits
     */
    extern auto to_csdnnz(double decimal_value, unsigned int nnz) -> std::string;

    /**
     * @brief Convert an integer to CSD with limited non-zero digits
     * @ingroup csd_functions
     *
     * Converts an integer to a CSD (Canonical Signed Digit) string representation with
     * a fixed number of non-zero digits. This is particularly useful for hardware
     * design where you want to limit the number of adders/subtracters required.
     *
     * The function will stop adding non-zero digits once the specified limit is reached,
     * potentially resulting in an approximation of the original value.
     *
     * Example:
     * @code
     * to_csdnnz_i(28, 2) returns "+00-00"
     * // Uses only 2 non-zero digits: + at position 5, - at position 2
     * @endcode
     *
     * @param[in] decimal_value The integer to convert to CSD format. Can be positive,
     *                         negative, or zero.
     * @param[in] nnz The maximum number of non-zero digits allowed in the CSD representation.
     *               Must be at least 1.
     *
     * @return String representation of the input number in CSD format with at most
     *         nnz non-zero digits.
     *
     * @throws std::invalid_argument If nnz is 0 or if the conversion fails.
     *
     * @see to_csdnnz() for floating-point version
     * @see to_csd_i() for unlimited non-zero digits
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
     * @brief Convert CSD string to integer
     * @ingroup csd_functions
     *
     * The function `to_decimal_i` takes a CSD (Canonical Signed Digit) string as
     * input and converts it to an integer. This function only processes the
     * integral part of the CSD string and ignores any fractional part.
     *
     * This is essentially a wrapper around to_decimal_integral() for convenience
     * and API consistency.
     *
     * Example:
     * @code
     * to_decimal_i("+00-00") returns 28
     * to_decimal_i("+00-00.+") returns 28  // Fractional part ignored
     * to_decimal_i("0") returns 0
     * @endcode
     *
     * @param[in] csd Pointer to null-terminated CSD string containing
     *                only '0', '+', '-', and optional '.' characters.
     *
     * @return Integer value of the CSD string (fractional part ignored).
     *
     * @throws invalid_csd_format If the string contains invalid characters.
     *
     * @see to_csd_i() for reverse conversion
     * @see to_decimal() for floating-point conversion
     */
    CONSTEXPR14 auto to_decimal_i(const char *csd) -> int { return to_decimal_integral(csd); }
/// @}

}  // namespace csd
