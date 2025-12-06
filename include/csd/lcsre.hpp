/// @file lcsre.hpp
/// @brief Longest Common Substring with Repeated Elements
///
/// This module provides functionality for finding the longest repeated
/// non-overlapping substring in a given string. This is useful for
/// pattern recognition, sequence analysis, and string compression algorithms.
///
/// The algorithm uses dynamic programming with optimized space complexity
/// to efficiently find repeated patterns without allowing overlaps.
#pragma once

#include <iosfwd>  // for string
#include <string>  // for basic_string, operator==, operator<<

namespace csd {

/// @defgroup lcsre_functions Longest Repeated Substring Functions
/// @brief Functions for finding repeated substrings
/// @{

    /**
     * @brief Find the longest repeated non-overlapping substring
     * @ingroup lcsre_functions
     *
     * This function finds the longest substring that appears at least twice in the
     * input string, with the constraint that the occurrences must not overlap.
     * It uses a dynamic programming approach with optimized space complexity.
     *
     * The algorithm builds a 2D table where each cell [i][j] stores the length of
     * the longest common substring ending at positions i-1 and j-1, but only if
     * the substrings don't overlap (j-i > lcsre[i-1][j-1]).
     *
     * Example:
     * @code
     * longest_repeated_substring("banana", 6) returns "an"
     * // "an" appears at positions 1-2 and 3-4 (non-overlapping)
     *
     * longest_repeated_substring("abcabcabc", 9) returns "abc"
     * // "abc" appears at positions 0-2, 3-5, and 6-8
     *
     * longest_repeated_substring("abcdef", 6) returns ""
     * // No repeated substrings found
     * @endcode
     *
     * @param[in] sv Pointer to a null-terminated character array representing the
     *               input string. The string should contain valid ASCII or UTF-8 characters.
     * @param[in] n Length of the input string. Must be non-negative and should match
     *              the actual length of the string (excluding null terminator).
     *
     * @return The longest repeated non-overlapping substring found. If no repeated
     *         substring exists, returns an empty string.
     *
     * @throws std::invalid_argument If sv is nullptr or if n doesn't match the
     *                               actual string length.
     *
     * @note Time complexity: O(n²) where n is the string length
     * @note Space complexity: O(n) due to row-wise table optimization
     */
    extern auto longest_repeated_substring(const char *sv, size_t n) -> std::string;

/// @}

}  // namespace csd
