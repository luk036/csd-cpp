#include <csd/greeter.h>
// #include <csd/version.h>
#include <doctest/doctest.h>

#include <string>

TEST_CASE("Csd") {
    using namespace csd;

    Csd csd("Tests");

    CHECK(csd.greet(LanguageCode::EN) == "Hello, Tests!");
    CHECK(csd.greet(LanguageCode::DE) == "Hallo Tests!");
    CHECK(csd.greet(LanguageCode::ES) == "¡Hola Tests!");
    CHECK(csd.greet(LanguageCode::FR) == "Bonjour Tests!");
}

// TEST_CASE("Csd version") {
//     static_assert(std::string_view(CSD_VERSION) == std::string_view("1.0.6"));
//     CHECK(std::string(CSD_VERSION) == std::string("1.0.6"));
// }
