/// @file lcsre.cpp
#include <string>
#include <vector>

using std::string;
using std::vector;

namespace csd {
    /**
     * Finds the longest repeated substring in the given string.
     *
     * This implements a dynamic programming algorithm to find the longest substring
     * that appears at least twice in the input string. It builds a 2D table lcsre
     * where lcsre[i][j] stores the length of longest common substring ending at
     * indices i and j.
     *
     * It returns the longest repeated substring found. If no repeat is found,
     * it returns an empty string.
     *
     * Time complexity is O(n^2) where n is length of input string.
     *
     * @param[in] sv The parameter `sv` is a pointer to a character array, which
     * represents the input string. It is assumed that the string is
     * null-terminated.
     * @param[in] n The parameter `n` represents the length of the input string `sv`.
     *
     * @return The function `longest_repeated_substring` returns a string, which is
     * the longest repeated substring in the given input string `sv`.
     */
    auto longest_repeated_substring(const char *sv, size_t n) -> string {
        auto lcsre = vector<vector<unsigned int>>(n + 1, vector<unsigned int>(n + 1, 0U));

        auto res_length = 0U;  // To store length of result

        // building table in bottom-up manner
        auto index = 0U;
        for (auto i = 1U; i != n + 1; ++i) {
            for (auto j = i + 1; j != n + 1; ++j) {
                // (j-i) > lcsre[i-1][j-1] to remove
                // overlapping
                if (sv[i - 1] == sv[j - 1] && lcsre[i - 1][j - 1] < (j - i)) {
                    lcsre[i][j] = lcsre[i - 1][j - 1] + 1;

                    // updating maximum length of the
                    // substring and updating the finishing
                    // index of the suffix
                    if (lcsre[i][j] > res_length) {
                        res_length = lcsre[i][j];
                        if (index < i) {
                            index = i;
                        }
                    }
                } else {
                    lcsre[i][j] = 0U;
                }
            }
        }

        // If we have non-empty result, then insert
        // all characters from first character to
        // last character of string

        auto res = string("");  // To store result
        if (res_length > 0) {
            for (auto i = index - res_length + 1; i != index + 1; ++i) {
                res += sv[i - 1];
            }
        }

        return res;
    }

    // This code is contributed by ita_c
}  // namespace csd
