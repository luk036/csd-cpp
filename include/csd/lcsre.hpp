/// @file lcsre.hpp
#pragma once

#include <iosfwd> // for string
#include <string> // for basic_string, operator==, operator<<

namespace csd {

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

} // namespace csd
