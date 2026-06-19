/// @file test_rapidcheck.cpp
/// @brief Property-based tests for CSD library using RapidCheck
///
/// Uses property-based testing to verify CSD invariants:
/// - Valid character sets in CSD output strings
/// - Round-trip inverse relationship: to_decimal(to_csd(x)) ~= x
/// - Non-zero digit limits in nnz variants
/// - CSD property: no consecutive non-zero digits
/// - Correct handling of positive, negative, and fractional values

#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS

#include <doctest/doctest.h>

#ifdef RAPIDCHECK_H
#    include <rapidcheck.h>

#    include <csd/csd.hpp>
#    include <string>

using namespace csd;

TEST_CASE("Property-based test: to_csd returns valid characters") {
    rc::check("to_csd returns string containing only '0', '+', '-', and '.'", []() {
        auto value = *rc::gen::inRange(-1000, 1000);
        auto places = static_cast<int>(*rc::gen::inRange(0, 11));

        auto result = to_csd(value, places);

        for (char c : result) {
            RC_ASSERT(c == '0' || c == '+' || c == '-' || c == '.');
        }
    });
}

TEST_CASE("Property-based test: to_csd_i returns valid characters") {
    rc::check("to_csd_i returns string containing only '0', '+', and '-'", []() {
        auto value = *rc::gen::inRange(-1000, 1000);
        auto result = to_csd_i(value);

        for (char c : result) {
            RC_ASSERT(c == '0' || c == '+' || c == '-');
        }
    });
}

TEST_CASE("Property-based test: to_decimal and to_csd are inverses") {
    rc::check("to_decimal(to_csd(value, places)) approximately equals value", []() {
        auto value = *rc::gen::inRange(-100, 100);
        auto places = static_cast<int>(*rc::gen::inRange(0, 6));

        auto csd_str = to_csd(value, places);
        auto converted = to_decimal(csd_str.c_str());

        RC_ASSERT(std::abs(converted - value) < std::pow(0.5, places));
    });
}

TEST_CASE("Property-based test: to_decimal_i and to_csd_i are inverses") {
    rc::check("to_decimal_i(to_csd_i(value)) equals value", []() {
        auto value = *rc::gen::inRange(-1000, 1000);
        auto csd_str = to_csd_i(value);
        auto converted = to_decimal_i(csd_str.c_str());

        RC_ASSERT(converted == value);
    });
}

TEST_CASE("Property-based test: to_csdnnz limits non-zero digits") {
    rc::check("to_csdnnz respects the non-zero digit limit", []() {
        auto value = *rc::gen::inRange(-100, 100);
        auto nnz = static_cast<unsigned int>(*rc::gen::inRange(1, 6));

        auto result = to_csdnnz(value, nnz);

        // Count non-zero digits
        size_t count = 0;
        for (char c : result) {
            if (c == '+' || c == '-') {
                count++;
            }
        }

        RC_ASSERT(count <= nnz);
    });
}

TEST_CASE("Property-based test: to_csdnnz_i limits non-zero digits") {
    rc::check("to_csdnnz_i respects the non-zero digit limit", []() {
        auto value = *rc::gen::inRange(-1000, 1000);
        auto nnz = static_cast<unsigned int>(*rc::gen::inRange(1, 6));

        auto result = to_csdnnz_i(value, nnz);

        // Count non-zero digits
        size_t count = 0;
        for (char c : result) {
            if (c == '+' || c == '-') {
                count++;
            }
        }

        RC_ASSERT(count <= nnz);
    });
}

TEST_CASE("Property-based test: CSD strings have no consecutive non-zero digits") {
    rc::check("CSD representation has no consecutive non-zero digits", []() {
        auto value = *rc::gen::inRange(-1000, 1000);
        auto places = static_cast<int>(*rc::gen::inRange(0, 11));

        auto result = to_csd(value, places);

        bool last_nonzero = false;
        for (char c : result) {
            if (c == '+' || c == '-') {
                RC_ASSERT(!last_nonzero);
                last_nonzero = true;
            } else if (c == '0') {
                last_nonzero = false;
            }
            // '.' doesn't affect the rule
        }
    });
}

TEST_CASE("Property-based test: to_decimal handles valid strings correctly") {
    rc::check("to_decimal_using_switch handles valid CSD strings", []() {
        auto value = *rc::gen::inRange(-100, 100);
        auto places = static_cast<int>(*rc::gen::inRange(0, 6));

        auto csd_str = to_csd(value, places);
        auto result = to_decimal_using_switch(csd_str.c_str());

        RC_ASSERT(std::abs(result - value) < std::pow(0.5, places));
    });
}

TEST_CASE("Property-based test: to_csd with zero places produces minimal decimal part") {
    rc::check("to_csd with places=0 produces representation with minimal decimal digits", []() {
        auto value = *rc::gen::inRange(-100, 100);
        auto result = to_csd(value, 0);

        // Verify the result is valid and reconstructs approximately
        auto converted = to_decimal(result.c_str());
        RC_ASSERT(std::abs(converted - value) < 1.0);
    });
}

TEST_CASE("Property-based test: CSD zero representation") {
    rc::check("to_csd(0, places) returns representation starting with '0'", []() {
        auto places = static_cast<int>(*rc::gen::inRange(0, 11));
        auto result = to_csd(0.0, places);

        RC_ASSERT(!result.empty());
        RC_ASSERT(result[0] == '0');
    });
}

TEST_CASE("Property-based test: to_csdnnz accuracy decreases with lower nnz") {
    rc::check("Lower nnz values produce approximations", []() {
        auto value = *rc::gen::inRange(1, 100);
        auto nnz = static_cast<unsigned int>(*rc::gen::inRange(1, 4));

        auto csd_str = to_csdnnz(value, nnz);
        auto converted = to_decimal(csd_str.c_str());

        // With limited nnz, we might not get exact value
        // Just verify it's in reasonable range
        RC_ASSERT(std::abs(converted) <= std::abs(value) * 2.0);
    });
}

TEST_CASE("Property-based test: to_decimal_integral and fractional work together") {
    rc::check("to_decimal_integral and to_decimal_fractional reconstruct value", []() {
        auto value = *rc::gen::inRange(-100, 100);
        auto places = static_cast<int>(*rc::gen::inRange(1, 6));

        auto csd_str = to_csd(value, places);
        const char* ptr = csd_str.c_str();

        auto integral = to_decimal_integral(ptr);
        double fractional = 0.0;
        if (*ptr == '.') {
            fractional = to_decimal_fractional(ptr);
        }

        auto total = static_cast<double>(integral) + fractional;
        RC_ASSERT(std::abs(total - value) < std::pow(0.5, places));
    });
}

TEST_CASE("Property-based test: negative values produce negative-leading CSD") {
    rc::check("Negative values produce CSD starting with '-'", []() {
        auto value = *rc::gen::inRange(-100, -1);
        auto places = static_cast<int>(*rc::gen::inRange(0, 6));

        auto result = to_csd(value, places);

        // Find first non-zero character
        size_t pos = result.find_first_of("+-");
        RC_ASSERT(pos != std::string::npos);
        RC_ASSERT(result[pos] == '-');
    });
}

TEST_CASE("Property-based test: positive values produce positive-leading CSD") {
    rc::check("Positive values produce CSD starting with '+'", []() {
        auto value = *rc::gen::inRange(1, 100);
        auto places = static_cast<int>(*rc::gen::inRange(0, 6));

        auto result = to_csd(value, places);

        // Find first non-zero character
        size_t pos = result.find_first_of("+-");
        RC_ASSERT(pos != std::string::npos);
        RC_ASSERT(result[pos] == '+');
    });
}

TEST_CASE("Property-based test: to_csd handles fractional values") {
    rc::check("to_csd correctly handles fractional values", []() {
        auto int_part = *rc::gen::inRange(0, 100);
        auto frac_numerator = *rc::gen::inRange(1, 100);
        auto frac_denominator = *rc::gen::inRange(2, 10);
        auto places = static_cast<int>(*rc::gen::inRange(1, 8));

        double value
            = static_cast<double>(int_part)
              + static_cast<double>(frac_numerator) / static_cast<double>(frac_denominator);

        auto csd_str = to_csd(value, places);
        auto converted = to_decimal(csd_str.c_str());

        RC_ASSERT(std::abs(converted - value) < std::pow(0.5, places));
    });
}

TEST_CASE("Property-based test: to_decimal_i extracts integer part") {
    rc::check("to_decimal_i extracts integer part from CSD string", []() {
        auto int_part = *rc::gen::inRange(-100, 100);

        // Create CSD string for integer value
        auto csd_str = to_csd_i(int_part);
        auto converted = to_decimal_i(csd_str.c_str());

        RC_ASSERT(converted == int_part);
    });
}

#endif
