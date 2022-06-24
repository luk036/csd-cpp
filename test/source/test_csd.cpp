#include <doctest/doctest.h>  // for ResultBuilder, CHECK_EQ, TEST_CASE

#include <iosfwd>  // for string
#include <string>  // for basic_string, operator==, operator<<

extern auto to_csd(double num, int places) -> std::string;
extern auto to_decimal(const std::string& csd_str) -> double;
extern auto to_csdfixed(double num, unsigned int nnz) -> std::string;

TEST_CASE("test to_csd") {
    CHECK_EQ(to_csd(28.5, 2), "+00-00.+0");
    CHECK_EQ(to_csd(-0.5, 2), "0.-0");
}

TEST_CASE("test to_decimal") {
    CHECK_EQ(to_decimal("+00-00.+"), 28.5);
    CHECK_EQ(to_decimal("0.-"), -0.5);
}

TEST_CASE("test to_csdfixed") {
    CHECK_EQ(to_csdfixed(28.5, 4), "+00-00.+");
    CHECK_EQ(to_csdfixed(-0.5, 4), "0.-");
}
