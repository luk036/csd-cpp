#include <doctest/doctest.h>

#include <csd/csd.hpp>
#include <csd/lcsre.hpp>
#include <limits>
#include <random>
#include <string>
#include <vector>

using namespace csd;

TEST_CASE("CSD Stress Tests - Large Numbers") {
    double large_pos = 123456789.123456789;
    double large_neg = -987654321.987654321;

    auto csd_large_pos = to_csd(large_pos, 10);
    CHECK(to_decimal(csd_large_pos.c_str()) == doctest::Approx(large_pos));

    auto csd_large_neg = to_csd(large_neg, 10);
    CHECK(to_decimal(csd_large_neg.c_str()) == doctest::Approx(large_neg));
}

TEST_CASE("CSD Stress Tests - Precision") {
    double near_zero_pos = 0.00000000012345;
    double near_zero_neg = -0.00000000098765;

    auto csd_near_zero_pos = to_csd(near_zero_pos, 20);
    CHECK(to_decimal(csd_near_zero_pos.c_str()) == doctest::Approx(near_zero_pos));

    auto csd_near_zero_neg = to_csd(near_zero_neg, 20);
    CHECK(to_decimal(csd_near_zero_neg.c_str()) == doctest::Approx(near_zero_neg));
}

TEST_CASE("CSD Stress Tests - Round Trip Random") {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-1000000.0, 1000000.0);

    for (int i = 0; i < 100; ++i) {
        double random_val = dis(gen);
        auto csd_repr = to_csd(random_val, 15);
        CHECK(to_decimal(csd_repr.c_str()) == doctest::Approx(random_val));
    }
}

TEST_CASE("LCSRE Stress Tests - Long Strings") {
    std::string long_str_with_repeat;
    long_str_with_repeat.reserve(2000);
    for (int i = 0; i < 99; ++i) {
        long_str_with_repeat += "abcdefghij";
    }
    CHECK_EQ(
        longest_repeated_substring(long_str_with_repeat.c_str(), long_str_with_repeat.length()),
        "abcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghij"
        "abcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghij"
        "abcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghij"
        "abcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghij"
        "abcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghij"
        "abcdefghijabcdefghijabcdefghijabcdefghij");
}

TEST_CASE("LCSRE Stress Tests - Max Length String") {
    std::string s(1000, 'a');
    CHECK_EQ(longest_repeated_substring(s.c_str(), s.length()), std::string(500, 'a'));
}
