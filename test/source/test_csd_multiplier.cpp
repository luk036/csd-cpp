#include <doctest/doctest.h>

#include <csd/csd_multiplier.hpp>
#include <stdexcept>
#include <string>

using namespace csd;

// ---------------------------------------------------------------------------
// Basic structural tests
// ---------------------------------------------------------------------------

TEST_CASE("csd_multiplier basic valid") {
    auto v = generate_csd_multiplier("+0-", 8, 2);
    CHECK(v.find("module csd_multiplier") != std::string::npos);
    CHECK(v.find("endmodule") != std::string::npos);
    CHECK(v.find("input signed [7:0] x") != std::string::npos);
    CHECK(v.find("output signed [9:0] result") != std::string::npos);
    CHECK(v.find("assign result = x_shift2 - x_shift0") != std::string::npos);
}

TEST_CASE("csd_multiplier positive only") {
    auto v = generate_csd_multiplier("+0+", 4, 2);
    CHECK(v.find("assign result = x_shift2 + x_shift0") != std::string::npos);
}

TEST_CASE("csd_multiplier negative only") {
    // Leading minus must be preserved
    auto v = generate_csd_multiplier("-0-", 8, 2);
    CHECK(v.find("assign result = -x_shift2 - x_shift0") != std::string::npos);
}

TEST_CASE("csd_multiplier all zeros") {
    auto v = generate_csd_multiplier("000", 8, 2);
    CHECK(v.find("assign result = 0;") != std::string::npos);
    CHECK(v.find("x_shift") == std::string::npos);
}

TEST_CASE("csd_multiplier single non-zero") {
    auto v = generate_csd_multiplier("+00", 8, 2);
    CHECK(v.find("assign result") != std::string::npos);
    CHECK(v.find("x_shift2") != std::string::npos);
}

TEST_CASE("csd_multiplier invalid chars") {
    CHECK_THROWS_AS(generate_csd_multiplier("123", 8, 2), std::invalid_argument);
}

TEST_CASE("csd_multiplier invalid length") {
    CHECK_THROWS_AS(generate_csd_multiplier("+0-", 8, 3), std::invalid_argument);
}

// ---------------------------------------------------------------------------
// LCSRe optimization tests
// ---------------------------------------------------------------------------

TEST_CASE("csd_multiplier flat when pattern nnz is 1") {
    // "+0" has only 1 non-zero digit — should NOT be optimized
    auto v = generate_csd_multiplier("+00-00+0", 8, 7);
    CHECK(v.find("_pat") == std::string::npos);
    CHECK(v.find("x_shift7 - x_shift4 + x_shift1") != std::string::npos);
}

TEST_CASE("csd_multiplier double repeat optimization") {
    // +0-0+0-0: repeated "+0-0" (2 nnz) at positions 0 and 4
    auto v = generate_csd_multiplier("+0-0+0-0", 8, 7);
    CHECK(v.find("_pat") != std::string::npos);
    CHECK(v.find("_pat = x_shift7 - x_shift5") != std::string::npos);
    CHECK(v.find("(_pat >>> 4)") != std::string::npos);
    CHECK(v.find("LCSRe") != std::string::npos);
}

TEST_CASE("csd_multiplier triple repeat optimization") {
    // +0-0+0-0+0-0: repeated "+0-0" at positions 0, 4, 8
    auto v = generate_csd_multiplier("+0-0+0-0+0-0", 8, 11);
    CHECK(v.find("_pat") != std::string::npos);
    CHECK(v.find("(_pat >>> 4)") != std::string::npos);
    CHECK(v.find("(_pat >>> 8)") != std::string::npos);
}

TEST_CASE("csd_multiplier longer pattern repeat") {
    // +00-00+00-00: repeated "+00-00" (2 nnz, 5 chars) at positions 0 and 6
    auto v = generate_csd_multiplier("+00-00+00-00", 8, 11);
    CHECK(v.find("_pat") != std::string::npos);
    CHECK(v.find("_pat = x_shift11 - x_shift8") != std::string::npos);
    CHECK(v.find("(_pat >>> 6)") != std::string::npos);
}

TEST_CASE("csd_multiplier leading minus no optimization") {
    // CSD starting with '-' and no repeated pattern
    auto v = generate_csd_multiplier("-0-", 8, 2);
    CHECK(v.find("_pat") == std::string::npos);
    CHECK(v.find("-x_shift2 - x_shift0") != std::string::npos);
}

TEST_CASE("csd_multiplier pattern with leading minus") {
    // Repeated pattern starting with '-': -0+0-0+0
    auto v = generate_csd_multiplier("-0+0-0+0", 8, 7);
    CHECK(v.find("_pat") != std::string::npos);
    CHECK(v.find("_pat = -x_shift7 + x_shift5") != std::string::npos);
    CHECK(v.find("(_pat >>> 4)") != std::string::npos);
}

TEST_CASE("csd_multiplier no optimization for single occurrence") {
    // CSD with unique pattern throughout — no repeat = flat
    auto v = generate_csd_multiplier("+0-+00-0", 8, 7);
    CHECK(v.find("_pat") == std::string::npos);
}

TEST_CASE("csd_multiplier pat wire width matches output") {
    // output_width = 8 + 7 = 15, so wire signed [14:0]
    auto v = generate_csd_multiplier("+0-0+0-0", 8, 7);
    CHECK(v.find("[14:0] _pat") != std::string::npos);
}

TEST_CASE("csd_multiplier repeat with trailing gap") {
    // Repeated pattern followed by non-repeating suffix
    auto v = generate_csd_multiplier("+0-0+0-0+0", 8, 9);
    CHECK(v.find("_pat") != std::string::npos);
    CHECK(v.find("(_pat >>> 4)") != std::string::npos);
}

// ---------------------------------------------------------------------------
// Edge cases
// ---------------------------------------------------------------------------

TEST_CASE("csd_multiplier very short CSD") {
    // Length-1 CSD
    auto v = generate_csd_multiplier("+", 8, 0);
    CHECK(v.find("assign result = x_shift0") != std::string::npos);
}

TEST_CASE("csd_multiplier all minus signs") {
    auto v = generate_csd_multiplier("---", 8, 2);
    CHECK(v.find("_pat") == std::string::npos);  // pattern has 3 nnz but
                                                 // no repeat of same substring
}

TEST_CASE("csd_multiplier always has proper module boundaries") {
    auto v = generate_csd_multiplier("+0-0+0-0", 8, 7);
    // Must start with module and end with endmodule
    CHECK(v.find("\nmodule csd_multiplier") != std::string::npos);
    CHECK(v.find("endmodule\n") != std::string::npos);
}

TEST_CASE("csd_multiplier lcsre comment present when optimized") {
    auto v = generate_csd_multiplier("+0-0+0-0", 8, 7);
    CHECK(v.find("LCSRe") != std::string::npos);
}

TEST_CASE("csd_multiplier no lcsre comment when flat") {
    auto v = generate_csd_multiplier("+00-00+0", 8, 7);
    CHECK(v.find("LCSRe") == std::string::npos);
}
