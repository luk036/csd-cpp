/// @file lcsre.cpp
#include <string>
#include <vector>

using std::string;
using std::vector;

namespace csd {
    /**
     * @brief Longest repeated non-overlapping substring
     *
     * The function `longest_repeated_substring` takes a string and its length as
     * input and returns the longest repeated substring in the string.
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
