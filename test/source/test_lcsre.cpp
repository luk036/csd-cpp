#include <doctest/doctest.h>  // for ResultBuilder, CHECK_EQ, TEST_CASE

#include <csd/lcsre.hpp>  // for longest_repeated_substring

using namespace csd;

TEST_CASE("test lcsre") {  // Driver Code
    CHECK_EQ(longest_repeated_substring("+-00+-00+-00+-0", 15U), "+-00+-0");
    CHECK_EQ(longest_repeated_substring("abcdefghijklmno", 15U), "");
    CHECK_EQ(longest_repeated_substring("banana", 6U), "an");
}

TEST_CASE("test lcsre empty string") {
    CHECK_EQ(longest_repeated_substring("", 0U), "");
}

TEST_CASE("test lcsre single char") {
    CHECK_EQ(longest_repeated_substring("a", 1U), "");
    CHECK_EQ(longest_repeated_substring("+", 1U), "");
}

TEST_CASE("test lcsre two same chars") {
    CHECK_EQ(longest_repeated_substring("aa", 2U), "a");
}

TEST_CASE("test lcsre no repeat short") {
    CHECK_EQ(longest_repeated_substring("ab", 2U), "");
    CHECK_EQ(longest_repeated_substring("abc", 3U), "");
}

TEST_CASE("test lcsre exact repeat at end") {
    CHECK_EQ(longest_repeated_substring("abab", 4U), "ab");
}

TEST_CASE("test lcsre leading minus pattern") {
    CHECK_EQ(longest_repeated_substring("-0+0-0+0", 8U), "-0+0");
}
