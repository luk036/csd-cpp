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

#include <cmath>  // for fabs, pow, ceil, log2
#include <iosfwd> // for string
#include <string> // for basic_string

using std::abs;
using std::ceil;
using std::fabs;
using std::log2;
using std::pow;
using std::string;

namespace csd {
/**
 * @brief Convert to CSD (Canonical Signed Digit) string representation
 *
 * Original author: Harnesser
 * https://sourceforge.net/projects/pycsd/
 * License: GPL2
 *
 * @param num
 * @param places
 * @return string
 */
[[nodiscard]] auto to_csd(double num, const int places) -> string {
  if (num == 0.0) {
    return "0";
  }
  auto absnum = fabs(num);
  double rem{0.0};
  string csd{"0"};
  if (absnum >= 1.0) {
    rem = ceil(log2(absnum * 1.5));
    csd = string{""};
  }

  auto p2n = pow(2.0, rem);
  auto const eps = pow(2.0, -places);
  while (p2n > eps) {
    if (p2n == 1.0) {
      csd += '.';
    }
    p2n /= 2.0;
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
 * @param num
 * @param places
 * @return string
 */
[[nodiscard]] auto to_csd_i(int num) -> string {
  if (num == 0) {
    return "0";
  }
  auto p2n = int(pow(2.0, ceil(log2(abs(num) * 1.5))));
  string csd("");
  while (p2n > 1) {
    auto const p2n_half = p2n / 2;
    auto const det = 3 * num;
    if (det > p2n) {
      csd += '+';
      num -= p2n_half;
    } else if (det < -p2n) {
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
 * @brief Convert the CSD string to a decimal
 *
 * @param csd
 * @return double
 */
[[nodiscard]] auto to_decimal_using_switch(const char *csd) -> double {
  static const auto null_char = '\0'; // avoid nullptr
  const auto *loc_ptr = &null_char;
  auto num = 0.0;
  for (; *csd != null_char; ++csd) {
    switch (*csd) {
    case '0':
      num = num * 2.0;
      break;
    case '+':
      num = num * 2.0 + 1.0;
      break;
    case '-':
      num = num * 2.0 - 1.0;
      break;
    case '.':
      loc_ptr = csd + 1;
      break;
    default:
      // ignore unknown characters such as '
      break;
    }
  }
  if (loc_ptr != &null_char) {
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
[[nodiscard]] auto to_decimal(const char *csd) -> double {
  static const auto null_char = '\0'; // avoid nullptr
  const auto *loc_ptr = &null_char;
  auto num = 0.0;
  for (;; ++csd) {
    auto digit = *csd;
    if (digit == '0') {
      num *= 2.0;
    } else if (digit == '+') {
      num = num * 2.0 + 1.0;
    } else if (digit == '-') {
      num = num * 2.0 - 1.0;
    } else if (digit == '.') {
      loc_ptr = csd + 1;
    } else if (digit == null_char) {
      break;
    } else {
      // ignore unknown characters such as '
    }
  }
  if (loc_ptr != &null_char) {
    num /= std::pow(2.0, csd - loc_ptr);
  }
  return num;
}

/**
 * @brief Convert to CSD (Canonical Signed Digit) string representation
 *
 * @param[in] num
 * @param[in] nnz number of non-zero
 * @return string
 */
[[nodiscard]] auto to_csdfixed(double num, unsigned int nnz) -> string {
  if (num == 0.0) {
    return "0";
  }
  auto const absnum = fabs(num);
  double rem{0.0};
  string csd{"0"};
  if (absnum >= 1.0) {
    rem = ceil(log2(absnum * 1.5));
    csd = string{""};
  }

  auto p2n = pow(2.0, rem);
  while (p2n > 1.0 || (nnz > 0 && fabs(num) > 1e-100)) {
    if (p2n == 1.0) {
      csd += '.';
    }
    p2n /= 2.0;
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
