#include <csd/version.h>

#include <iostream>

auto main() -> int {
    const auto ok = (CSD_VERSION_MAJOR >= 1);
    std::cout << "csd installed test: version " << CSD_VERSION << "\n";
    return ok ? 0 : 1;
}
