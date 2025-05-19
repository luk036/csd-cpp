#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS

#include <doctest/doctest.h>  // for ResultBuilder, CHECK_EQ, TEST_CASE

#include <csd/csd.hpp>  // for to_csd, to_decimal, to_csdnnz, to_decimal_using_switch
#include <exception>

using namespace csd;

TEST_CASE("test to_csd") {
    CHECK_EQ(to_csd(28.5, 2), "+00-00.+0");
    CHECK_EQ(to_csd(-0.5, 2), "0.-0");
    CHECK_EQ(to_csd(0.0, 2), "0.00");
    CHECK_EQ(to_csd(0.0, 0), "0.");
}

TEST_CASE("test to_csd_i") {
    CHECK_EQ(to_csd_i(28), "+00-00");
    CHECK_EQ(to_csd_i(-0), "0");
    CHECK_EQ(to_csd_i(-0), "0");
}

TEST_CASE("test to_decimal") {
    CHECK_EQ(to_decimal("+00-00.+"), 28.5);
    CHECK_EQ(to_decimal("0.-"), -0.5);
    CHECK_EQ(to_decimal("0"), 0.0);
    CHECK_EQ(to_decimal("0.0"), 0.0);
    CHECK_EQ(to_decimal("0.+"), 0.5);
    CHECK_EQ(to_decimal("0.-"), -0.5);
    CHECK_THROWS(to_decimal("+00XX-00.+"));
    CHECK_THROWS(to_decimal("+00-00.+XXX"));
}

TEST_CASE("test to_decimal_using_switch") {
    CHECK_EQ(to_decimal_using_switch("+00-00.+"), 28.5);
    CHECK_EQ(to_decimal_using_switch("0.-"), -0.5);
    CHECK_EQ(to_decimal_using_switch("0"), 0.0);
    CHECK_EQ(to_decimal_using_switch("0.0"), 0.0);
    CHECK_EQ(to_decimal_using_switch("0.+"), 0.5);
    CHECK_EQ(to_decimal_using_switch("0.-"), -0.5);
    CHECK_THROWS(to_decimal_using_switch("+00XX-00.+"));
    CHECK_THROWS(to_decimal_using_switch("+00-00.+XXX"));
}

TEST_CASE("test to_csdnnz") {
    CHECK_EQ(to_csdnnz(28.5, 4), "+00-00.+");
    CHECK_EQ(to_csdnnz(-0.5, 4), "0.-");
    CHECK_EQ(to_csdnnz(0.0, 4), "0");
    CHECK_EQ(to_csdnnz(0.5, 4), "0.+");
    CHECK_EQ(to_csdnnz(-0.5, 4), "0.-");
    CHECK_EQ(to_csdnnz(28.5, 2), "+00-00");
    CHECK_EQ(to_csdnnz(28.5, 1), "+00000");
}

TEST_CASE("test to_csdnnz_i") {
    CHECK_EQ(to_csdnnz_i(28, 4), "+00-00");
    CHECK_EQ(to_csdnnz_i(-0, 4), "0");
    CHECK_EQ(to_csdnnz_i(0, 4), "0");
    CHECK_EQ(to_csdnnz_i(28, 2), "+00-00");
    CHECK_EQ(to_csdnnz_i(28, 1), "+00000");
    CHECK_EQ(to_csdnnz_i(158, 2), "+0+00000");
}


TEST_CASE("test to_decimal_i") {
    CHECK_EQ(to_decimal_i("+00-00"), 28);
    CHECK_EQ(to_decimal_i("0"), 0);
    CHECK_EQ(to_decimal_i("+00-00.00+"), 28);
    // CHECK_THROWS(to_decimal_i("+00-00.00+"));
}
