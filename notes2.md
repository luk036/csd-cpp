#include <iostream>
#include <string>
#include <cmath>

double to_csd(double decimal, int places) {
// implementation of to_csd function
}

double to_csdfixed(double decimal, int nnz) {
// implementation of to_csdfixed function
}

double to_decimal(std::string csdstr) {
// implementation of to_decimal function
}

struct Args {
double decimal;
double decimal2;
std::string csdstr;
int places;
int nnz;
};

Args parse_args(int argc, char\* argv[]) {
Args args;
args.decimal = INFINITY;
args.decimal2 = INFINITY;
args.csdstr = "";
args.places = 4;
args.nnz = 4;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-c" || arg == "--to_csd") {
            args.decimal = std::stod(argv[++i]);
        } else if (arg == "-f" || arg == "--to_csdfixed") {
            args.decimal2 = std::stod(argv[++i]);
        } else if (arg == "-d" || arg == "--to_decimal") {
            args.csdstr = argv[++i];
        } else if (arg == "-p" || arg == "--places") {
            args.places = std::stoi(argv[++i]);
        } else if (arg == "-z" || arg == "--nnz") {
            args.nnz = std::stoi(argv[++i]);
        }
    }

    return args;

}

void main(int argc, char\* argv[]) {
Args args = parse_args(argc, argv);
if (args.decimal != INFINITY) {
double ans = to_csd(args.decimal, args.places);
std::cout << ans << std::endl;
}
if (args.decimal2 != INFINITY) {
double ans = to_csdfixed(args.decimal2, args.nnz);
std::cout << ans << std::endl;
}
if (!args.csdstr.empty()) {
double ans = to_decimal(args.csdstr);
std::cout << ans << std::endl;
}
}

int main() {
int argc = 0;
char\* argv[] = {};
main(argc, argv);
return 0;
}
