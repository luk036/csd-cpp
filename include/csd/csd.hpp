#pragma once

#include <cmath>  // for fabs, pow, ceil, log2
#include <iosfwd> // for string
#include <string> // for basic_string, operator==, operator<<
// #include <string_view> // for string_view

namespace csd {

extern auto to_csd(double num, int places) -> std::string;
extern auto to_csdfixed(double num, unsigned int nnz) -> std::string;
extern auto longest_repeated_substring(const char *sv, size_t n) -> std::string;
// extern auto to_decimal(const char *csd) -> double;
// extern auto to_decimal_using_switch(const char *csd) -> double;
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
  const char *loc_ptr = 0;
  auto num = 0.0;
  for (; *csd != '\0'; ++csd) {
    switch (*csd) {
    case '0':
      num = 2.0 * num;
      break;
    case '+':
      num = 2.0 * num + 1.0;
      break;
    case '-':
      num = 2.0 * num - 1.0;
      break;
    case '.':
      loc_ptr = csd + 1;
      break;
    default:
      exit(1); // unknown character
    }
  }
  if (loc_ptr != (const char *)0) {
    num /= std::pow(2.0, csd - loc_ptr);
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
  const char *loc_ptr = 0;
  auto num = 0.0;
  for (;; ++csd) {
    auto digit = *csd;
    if (digit == '0') {
      num = 2.0 * num;
    } else if (digit == '+') {
      num = 2.0 * num + 1.0;
    } else if (digit == '-') {
      num = 2.0 * num - 1.0;
    } else if (digit == '.') {
      loc_ptr = csd + 1;
    } else if (digit == '\0') {
      break;
    } else {
      exit(1); // unknown character
    }
  }
  if (loc_ptr != (const char *)0) {
    num /= std::pow(2.0, csd - loc_ptr);
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
