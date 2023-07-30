#include <doctest/doctest.h> // for ResultBuilder, CHECK_EQ, TEST_CASE

#include <csd/lcsre.hpp> // for to_csd, to_decimal, to_csdfixed, to_decimal_using_switch

using namespace csd;

TEST_CASE("test lcsre") { // Driver Code
    CHECK_EQ(longest_repeated_substring("+-00+-00+-00+-0", 15U), "+-00+-0");
}
