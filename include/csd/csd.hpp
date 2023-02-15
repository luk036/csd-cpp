#pragma once

#include <iosfwd> // for string
#include <string> // for basic_string, operator==, operator<<
// #include <string_view> // for string_view

namespace csd {

extern auto to_csd(double num, int places) -> std::string;
extern auto to_csdfixed(double num, unsigned int nnz) -> std::string;
extern auto longest_repeated_substring(const char *sv, size_t n) -> std::string;
extern auto to_decimal(const char *csd) -> double;
extern auto to_decimal_using_switch(const char *csd) -> double;
extern auto to_csd_i(int num) -> std::string;

/**
 * @brief Convert the CSD string to a decimal
 *
 * @param csd
 * @return int
 */
inline auto to_decimal_i(char const *csd) -> int {
  auto num = 0;
  for (;; ++csd) {
    auto digit = *csd;
    if (digit == '0') {
      num *= 2;
    } else if (digit == '+') {
      num = num * 2 + 1;
    } else if (digit == '-') {
      num = num * 2 - 1;
    } else if (digit == '\0') {
      break;
    } else {
      // ignore unknown characters such as '
    }
  }
  return num;
}
} // namespace csd
