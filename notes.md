#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>

#include <argparse/argparse.hpp>
#include <spdlog/spdlog.h>

#include "csdigit.hpp"
#include "csdigit/csd.hpp"

namespace spd = spdlog;

void parse_args(int argc, char* argv[]) {
    argparse::ArgumentParser parser("Converts a decimal to a CSD format");
    parser.add_argument("--version")
        .action([](const std::string& value) { std::cout << "csdigit " << value << std::endl; })
        .default_value(csdigit::version())
        .help("show program's version number and exit");
    parser.add_argument("-c", "--to_csd")
        .dest("decimal")
        .help("a decimal number")
        .type<float>()
        .default_value(std::numeric_limits<float>::infinity())
        .metavar("FLOAT");
    parser.add_argument("-v", "--verbose")
        .dest("loglevel")
        .help("set loglevel to INFO")
        .action([](const std::string&) { spd::set_level(spd::level::info); })
        .default_value(spd::level::off);
    parser.parse_args(argc, argv);
}

void setup_logging(spd::level::level_enum loglevel) {
    std::string logformat = "[%Y-%m-%d %H:%M:%S] [%l] [%n] %v";
    spd::set_pattern(logformat);
    spd::set_level(loglevel);
}

void main(int argc, char* argv[]) {
    parse_args(argc, argv);
    setup_logging(spd::level::debug);
    spd::debug("Starting crazy calculations...");

    argparse::ArgumentParser parser("Converts a decimal to a CSD format");
    float decimal = parser.get<float>("decimal");
    if (decimal != std::numeric_limits<float>::infinity()) {
        std::vector<int> ans = csdigit::to_csd(decimal);
        for (int digit : ans) {
            std::cout << digit;
        }
        std::cout << std::endl;
    }

    float decimal2 = parser.get<float>("decimal2");
    if (decimal2 != std::numeric_limits<float>::infinity()) {
        int nnz = parser.get<int>("nnz");
        std::vector<int> ans = csdigit::to_csdfixed(decimal2, nnz);
        for (int digit : ans) {
            std::cout << digit;
        }
        std::cout << std::endl;
    }

    std::string csdstr = parser.get<std::string>("csdstr");
    if (!csdstr.empty()) {
        float ans = csdigit::to_decimal(csdstr);
        std::cout << ans << std::endl;
    }

    spd::info("Script ends here");
}

int main(int argc, char* argv[]) {
    main(argc, argv);
    return 0;
}


