#include <csd/csd.hpp>  // for to_decimal, to_csd, and to_csdfixed
#include <cxxopts.hpp>
#include <iostream>
#include <string>

// #include <unordered_map>

auto main(int argc, char **argv) -> int {
    cxxopts::Options options(*argv, "Canonical Signed Digit (CSD) Conversion");

    double decimal;
    double decimal2;
    std::string csdstr;
    int nnz;
    int places;
    const double INFTY = -1.0e100;

    // clang-format off
  options.add_options()
    ("h,help", "Show help")
    ("v,version", "Print the current version number")
    ("d,to_decimal", "Convert to decimal", cxxopts::value(csdstr)->default_value(""))
    ("c,to_csd", "Convert to CSD with places", cxxopts::value(decimal)->default_value("-1.0e100"))
    ("f,to_csdfixed", "Convert to CSD with number of non-zeros", cxxopts::value(decimal2)->default_value("-1.0e100"))
    ("p,place", "Number of places", cxxopts::value(places)->default_value("4"))
    ("z,nnz", "Number of non-zeros", cxxopts::value(nnz)->default_value("3"))
  ;
    // clang-format on

    auto result = options.parse(argc, argv);

    if (result["help"].as<bool>()) {
        std::cout << options.help() << std::endl;
        return 0;
    }

    if (result["version"].as<bool>()) {
        std::cout << "Csd, version "
                  << "1.0.0" << std::endl;
        return 0;
    }

    if (decimal != INFTY) {
        std::cout << csd::to_csd(decimal, places) << std::endl;
    }

    if (decimal2 != INFTY) {
        std::cout << csd::to_csdfixed(decimal2, (unsigned int)(nnz)) << std::endl;
    }

    if (!csdstr.empty()) {
        std::cout << csd::to_decimal(csdstr.c_str()) << std::endl;
    }

    return 0;
}
