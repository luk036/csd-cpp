#include <csd/csd_multiplier.hpp>
#include <fmt/format.h>

int main() {
    auto v = csd::generate_csd_multiplier("+0-", 8, 2);
    fmt::print("{}\n", v);
    return 0;
}
