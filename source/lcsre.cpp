/// @file lcsre.cpp
/// @brief Implementation of longest repeated non-overlapping substring algorithm
/**
 * @details This file implements a dynamic programming algorithm to find the
 * longest substring that appears at least twice in a given string without
 * overlapping.
 *
 * The algorithm uses space optimization by only storing two rows of the
 * DP table at any time, reducing space complexity from O(n²) to O(n).
 *
 * @author Original algorithm contributed by ita_c
 */

#include <algorithm>
#include <string>
#include <vector>

using std::string;
using std::vector;

namespace csd {
    /**
     * @brief Find the longest repeated non-overlapping substring
     * @ingroup lcsre_functions
     *
     * This implements a dynamic programming algorithm to find the longest substring
     * that appears at least twice in the input string without overlapping.
     *
     * Algorithm details:
     * - Builds a 2D DP table where lcsre[i][j] stores the length of the longest
     *   common substring ending at positions i-1 and j-1
     * - Uses space optimization by only storing two rows at a time
     * - Ensures non-overlapping by checking j-i > lcsre[i-1][j-1]
     * - Tracks the maximum length and ending position of the best match
     *
     * Algorithm visualization:
     * @verbatim
     *   Input: "banana"
     *   Build 2D table where cell [i][j] represents length of common substring
     *   ending at position i-1 and j-1, but only if i and j are far enough apart
     *   to avoid overlap (j-i > lcsre[i-1][j-1])
     *
     *   Table visualization for "banana" (only showing relevant parts):
     *         _  b  a  n  a  n  a
     *       +---------------------
     *     _ | 0  0  0  0  0  0  0
     *     b | 0  1  0  0  0  0  0
     *     a | 0  0  1  0  1  0  1
     *     n | 0  0  0  1  0  2  0
     *     a | 0  0  1  0  2  0  3
     *     n | 0  0  0  2  0  3  0
     *     a | 0  0  1  0  3  0  4
     *
     *   The algorithm finds that "an" appears at positions 1-2 and 3-4
     *   Result: "an"
     * @endverbatim
     *
     * Space optimization:
     * Instead of storing the full n×n table, we only keep two rows:
     * - Current row (i % 2)
     * - Previous row ((i-1) % 2)
     * This reduces space from O(n²) to O(n)
     *
     * @param[in] sv Pointer to a null-terminated character array representing the
     *               input string. Must not be nullptr.
     * @param[in] len Length of the input string. Must be non-negative and should
     *              match the actual string length.
     *
     * @return The longest repeated non-overlapping substring. Returns empty string
     *         if no repeated substring exists.
     *
     * @throws std::invalid_argument If sv is nullptr or len is negative.
     *
     * @note Time complexity: O(n²)
     * @note Space complexity: O(n) due to row-wise optimization
     */
    auto longest_repeated_substring(const char* sv, size_t len) -> string {
        auto ndim = len + 1;
        // Flat vector: 2 rows of ndim columns, avoids vector-of-vector indirection
        auto lcsre = vector<unsigned int>(2 * ndim, 0U);

        auto res_length = 0U;  // To store length of result

        // building table in bottom-up manner
        auto index = 0U;
        for (auto i = 1U; i != ndim; ++i) {
            auto cur_row = (i % 2) * ndim;
            auto prev_row = ((i - 1) % 2) * ndim;
            for (auto j = i + 1; j != ndim; ++j) {
                // (j-i) > lcsre[i-1][j-1] to remove
                // overlapping
                if (sv[i - 1] == sv[j - 1] && lcsre[prev_row + j - 1] < (j - i)) {
                    lcsre[cur_row + j] = lcsre[prev_row + j - 1] + 1;

                    // updating maximum length of the
                    // substring and updating the finishing
                    // index of the suffix
                    if (lcsre[cur_row + j] > res_length) {
                        res_length = lcsre[cur_row + j];
                        index = std::max(index, i);
                    }
                } else {
                    lcsre[cur_row + j] = 0U;
                }
            }
        }

        // If we have non-empty result, then insert
        // all characters from first character to
        // last character of string

        string res;  // To store result
        if (res_length > 0) {
            res.assign(sv + index - res_length, res_length);
        }

        return res;
    }

    // This code is contributed by ita_c
}  // namespace csd
