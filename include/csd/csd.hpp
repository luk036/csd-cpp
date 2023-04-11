#pragma once

#include <cmath>  // for fabs, pow, ceil, log2
#include <iosfwd> // for string
#include <string> // for basic_string, operator==, operator<<

namespace csd {

extern auto to_csd(double num, int places) -> std::string;
extern auto to_csdfixed(double num, unsigned int nnz) -> std::string;
extern auto longest_repeated_substring(const char *sv, size_t n) -> std::string;
extern auto to_csd_i(int num) -> std::string;

#if __cpp_constexpr >= 201304
#define CONSTEXPR14 constexpr
#else
#define CONSTEXPR14 inline
#endif

/**
 * @brief Convert the CSD string to a decimal
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
      exit(1); // unknown character
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
      exit(1); // unknown character
    }
    scale /= 2;
  }
  return num;
}

/**
 * @brief Convert the CSD string to a decimal
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
      exit(1); // unknown character
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
      exit(1); // unknown character
    }
    scale /= 2;
  }
  return num;
}

/**
 * @brief Convert the CSD string to a decimal
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
      exit(1); // unknown character
    }
  }
  return num;
}
} // namespace csd
