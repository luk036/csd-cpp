#include <csd/csd_multiplier.hpp>
#include <cstdio>

int main() {
    auto v = csd::generate_csd_multiplier("+0-", 8, 2);
    std::printf("%s\n", v.c_str());
    return 0;
}
