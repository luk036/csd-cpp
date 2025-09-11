#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <algorithm>
#include <stdexcept>

using namespace std;

/**
 * Generate Verilog code for a CSD multiplier module with proper signed handling.
 *
 * @param csd CSD string (e.g., "+00-00+0+")
 * @param N Input bit width
 * @param M Highest power in CSD (must be len(csd)-1)
 * @return Verilog module code as a string
 * @throws invalid_argument if CSD length doesn't match M+1 or contains invalid characters
 */
string generate_csd_multiplier(const string& csd, int N, int M) {
    // Validate inputs
    if (csd.length() != M + 1) {
        throw invalid_argument("CSD length " + to_string(csd.length()) + 
                              " doesn't match M=" + to_string(M) + " (should be M+1)");
    }

    for (char c : csd) {
        if (c != '+' && c != '-' && c != '0') {
            throw invalid_argument("CSD string can only contain '+', '-', or '0'");
        }
    }

    // Parse CSD and collect non-zero terms
    vector<pair<int, string>> terms;
    for (size_t i = 0; i < csd.length(); i++) {
        int power = M - i; // Most significant digit is highest power
        char c = csd[i];
        if (c == '+') {
            terms.emplace_back(power, "add");
        } else if (c == '-') {
            terms.emplace_back(power, "sub");
        }
    }

    // Generate module header
    string verilog_code = 
        "\nmodule csd_multiplier (\n"
        "    input signed [" + to_string(N-1) + ":0] x,      // Input value\n"
        "    output signed [" + to_string(N+M-1) + ":0] result // Result of multiplication\n"
        ");";

    // Generate shifted versions
    if (!terms.empty()) {
        verilog_code += "\n\n    // Create shifted versions of input";
        set<int, greater<int>> powers_needed;
        for (const auto& term : terms) {
            powers_needed.insert(term.first);
        }

        for (int p : powers_needed) {
            verilog_code += "\n    wire signed [" + to_string(N+M-1) + ":0] x_shift" + 
                           to_string(p) + " = x <<< " + to_string(p) + ";";
        }
    }

    // Generate the computation
    verilog_code += "\n\n    // CSD implementation";
    if (terms.empty()) {
        verilog_code += "\n    assign result = 0;";
    } else {
        string expr = "x_shift" + to_string(terms[0].first);

        for (size_t i = 1; i < terms.size(); i++) {
            const auto& [power, op] = terms[i];
            if (op == "add") {
                expr += " + x_shift" + to_string(power);
            } else {
                expr += " - x_shift" + to_string(power);
            }
        }

        verilog_code += "\n    assign result = " + expr + ";";
    }

    verilog_code += "\nendmodule\n";
    return verilog_code;
}

int main() {
    try {
        string csd = "+00-00+0";  // Represents 114
        int N = 8;  // Input bit width
        int M = 7;  // Highest power (2^7 for this CSD)

        string verilog_code = generate_csd_multiplier(csd, N, M);
        cout << verilog_code << endl;
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}