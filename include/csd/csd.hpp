/// @file csd.hpp
#pragma once

#include <cmath>     // for fabs, pow, ceil, log2
#include <iosfwd>    // for string
#include <stdexcept> // for invalid_argument
#include <string>    // for basic_string, operator==, operator<<

#if __cpp_constexpr >= 201304
#define CONSTEXPR14 constexpr
#else
#define CONSTEXPR14 inline
#endif

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
extern auto to_csd(double num, int places) -> std::string;

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
extern auto to_csd_i(int num) -> std::string;

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
extern auto to_csdfixed(double num, unsigned int nnz) -> std::string;

/**
 * @brief Longest repeated non-overlapping substring
 *
 * The function `longest_repeated_substring` takes a string and its length as
 * input and returns the longest repeated substring in the string.
 *
 * @param sv The parameter `sv` is a pointer to a character array, which
 * represents the input string. It is assumed that the string is
 * null-terminated.
 * @param n The parameter `n` represents the length of the input string `sv`.
 *
 * @return The function `longest_repeated_substring` returns a string, which is
 * the longest repeated substring in the given input string `sv`.
 */
extern auto longest_repeated_substring(const char *sv, size_t n) -> std::string;

/**
 * @brief Convert the CSD string to a decimal
 *
 * The function `to_decimal_using_switch` takes a CSD (Canonical Signed Digit)
 * string as input and converts it to a decimal number using a switch statement.
 *
 * @param csd
 * @return double
 */
CONSTEXPR14 auto to_decimal_using_switch(const char *csd) -> double {
  auto num = 0.0;
  // Handle integral part
  for (; *csd != '.' && *csd != '\0'; ++csd) {
    switch (*csd) {
    case '0':
      num *= 2.0;
      break;
    case '+':
      num = 2.0 * num + 1.0;
      break;
    case '-':
      num = 2.0 * num - 1.0;
      break;
    case '.':
      break;
    case '\0':
      break;
    default:
      throw std::invalid_argument("Work with 0, +, -, . only");
    }
  }
  if (*csd == '\0') {
    return num;
  }
  // Handle fractional part
  auto scale = 0.5;
  for (++csd; *csd != '\0'; ++csd) {
    switch (*csd) {
    case '0':
      break;
    case '+':
      num += scale;
      break;
    case '-':
      num -= scale;
      break;
    case '\0':
      break;
    default:
      throw std::invalid_argument("Fractional part work with 0, +, - only");
    }
    scale /= 2;
  }
  return num;
}

/**
 * @brief Convert the CSD string to a decimal
 *
 * The function `to_decimal` takes a CSD (Canonical Signed Digit) string as
 * input and converts it to a decimal number. It iterates through the characters
 * of the string and performs the corresponding operations based on the
 * character.
 *
 * @param csd
 * @return double
 */
CONSTEXPR14 auto to_decimal(const char *csd) -> double {
  auto num = 0.0;
  // Handle integral part
  for (;; ++csd) {
    auto digit = *csd;
    if (digit == '0') {
      num *= 2.0;
    } else if (digit == '+') {
      num = 2.0 * num + 1.0;
    } else if (digit == '-') {
      num = 2.0 * num - 1.0;
    } else if (digit == '.' || digit == '\0') {
      break;
    } else {
      throw std::invalid_argument("Work with 0, +, -, . only");
    }
  }
  if (*csd == '\0') {
    return num;
  }
  // Handle fractional part
  auto scale = 0.5;
  for (++csd;; ++csd) {
    auto digit = *csd;
    if (digit == '0') {
    } else if (digit == '+') {
      num += scale;
    } else if (digit == '-') {
      num -= scale;
    } else if (digit == '\0') {
      break;
    } else {
      throw std::invalid_argument("Fractional part work with 0, +, - only");
    }
    scale /= 2;
  }
  return num;
}

/**
 * @brief Convert the CSD string to a decimal
 *
 * The function `to_decimal_i` takes a CSD (Canonical Signed Digit) string as
 * input and converts it to an integer. It iterates through the characters of
 * the string and performs the corresponding operations based on the character.
 *
 * @param csd
 * @return int
 */
CONSTEXPR14 auto to_decimal_i(char const *csd) -> int {
  auto num = 0;
  for (;; ++csd) {
    auto digit = *csd;
    if (digit == '0') {
      num = 2 * num;
    } else if (digit == '+') {
      num = 2 * num + 1;
    } else if (digit == '-') {
      num = 2 * num - 1;
    } else if (digit == '\0') {
      break;
    } else {
      throw std::invalid_argument("Integer conversion work with 0, +, - only");
    }
  }
  return num;
}
} // namespace csd
