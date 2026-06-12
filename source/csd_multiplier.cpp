/// @file csd_multiplier.cpp
/// @brief Implementation of Verilog CSD multiplier generation
#include <csd/csd_multiplier.hpp>
#include <csd/lcsre.hpp>
#include <map>        // for map
#include <set>        // for set, greater
#include <stdexcept>  // for invalid_argument
#include <string>     // for string, to_string
#include <utility>    // for pair
#include <vector>     // for vector

namespace csd {
    namespace {
        /// @brief Operation type for a CSD term
        enum class TermOp { Add, Sub };

        /**
         * @brief Parse CSD string into (power, operation) pairs
         *
         * Iterates through the CSD string and extracts each non-zero digit as a
         * term with its bit position. Fixes the first-term sign bug: a leading '-'
         * is properly reflected in the first term's operation rather than being
         * silently dropped.
         *
         * @param[in] csd_str CSD string containing '+', '-', '0' characters
         * @param[in] max_power Highest power of two in the CSD representation
         * @return Vector of (power, operation) pairs for non-zero digits
         * @throws std::invalid_argument If invalid characters are encountered
         */
        auto parse_terms(const std::string& csd_str, int max_power)
            -> std::vector<std::pair<int, TermOp>> {
            std::vector<std::pair<int, TermOp>> terms;
            auto const len = static_cast<int>(csd_str.size());
            for (int i = 0; i < len; ++i) {
                auto const power = max_power - i;
                switch (csd_str[i]) {
                    case '+':
                        terms.emplace_back(power, TermOp::Add);
                        break;
                    case '-':
                        terms.emplace_back(power, TermOp::Sub);
                        break;
                    case '0':
                        break;
                    default:
                        throw std::invalid_argument("CSD string can only contain '+', '-', or '0'");
                }
            }
            return terms;
        }

        /**
         * @brief Build a flat Verilog expression for a range of CSD positions
         *
         * Creates a Verilog expression using absolute x_shift references for all
         * non-zero digits in the specified range of the CSD string.
         *
         * @param[in] csd_str The full CSD string
         * @param[in] start Starting position in the CSD string
         * @param[in] length Number of positions to include
         * @param[in] max_power Highest power of two in the CSD representation
         * @return Verilog expression string, or empty if the range has no non-zero digits
         */
        auto build_range_expr(const std::string& csd_str, size_t start, size_t length,
                              int max_power) -> std::string {
            std::string expr;
            bool first = true;
            for (size_t i = start; i < start + length && i < csd_str.size(); ++i) {
                auto const power = max_power - static_cast<int>(i);
                switch (csd_str[i]) {
                    case '+':
                        if (first) {
                            expr += "x_shift" + std::to_string(power);
                            first = false;
                        } else {
                            expr += " + x_shift" + std::to_string(power);
                        }
                        break;
                    case '-':
                        if (first) {
                            expr += "-x_shift" + std::to_string(power);
                            first = false;
                        } else {
                            expr += " - x_shift" + std::to_string(power);
                        }
                        break;
                    default:
                        break;
                }
            }
            return expr;
        }

        /**
         * @brief Find all non-overlapping occurrences of a pattern in a CSD string
         *
         * The pattern is expected to come from longest_repeated_substring, which
         * guarantees non-overlapping with itself.
         *
         * @param[in] csd_str The CSD string to search in
         * @param[in] pattern The substring pattern to find
         * @return Sorted vector of starting positions for each occurrence
         */
        auto find_pattern_occurrences(const std::string& csd_str, const std::string& pattern)
            -> std::vector<size_t> {
            std::vector<size_t> positions;
            size_t pos = 0;
            while ((pos = csd_str.find(pattern, pos)) != std::string::npos) {
                positions.push_back(pos);
                pos += pattern.size();
            }
            return positions;
        }

        /**
         * @brief Build a coefficient expression using CSE wire and flat gap terms
         *
         * Constructs a Verilog expression that references the shared sub-expression
         * (CSE) wire for pattern occurrences and builds flat expressions for any
         * gap regions between them.
         *
         * @param[in] csd The full CSD string for this coefficient
         * @param[in] max_power Highest power of two in the CSD representation
         * @param[in] pattern The shared sub-expression pattern (empty if no CSE)
         * @param[in] base_pos Base position for the CSE wire
         * @param[in] cse_name Name of the CSE wire signal
         * @return Verilog expression string
         */
        auto build_coeff_expr(const std::string& csd, int max_power, const std::string& pattern,
                              int base_pos, const std::string& cse_name) -> std::string {
            if (pattern.empty()) {
                return build_range_expr(csd, 0, csd.size(), max_power);
            }

            auto const positions = find_pattern_occurrences(csd, pattern);
            std::vector<std::string> parts;
            size_t cur = 0;

            for (auto const pos : positions) {
                // gap before this occurrence
                if (pos > cur) {
                    auto gap = build_range_expr(csd, cur, pos - cur, max_power);
                    if (!gap.empty()) {
                        parts.push_back(gap);
                    }
                }
                // CSE reference
                auto const shift = static_cast<int>(pos) - base_pos;
                if (shift == 0) {
                    parts.push_back(cse_name);
                } else {
                    parts.push_back("(" + cse_name + " >>> " + std::to_string(shift) + ")");
                }
                cur = pos + pattern.size();
            }
            // suffix
            if (cur < csd.size()) {
                auto gap = build_range_expr(csd, cur, csd.size() - cur, max_power);
                if (!gap.empty()) {
                    parts.push_back(gap);
                }
            }

            if (parts.empty()) return {};
            std::string result = parts[0];
            for (size_t i = 1; i < parts.size(); ++i) {
                result += " + " + parts[i];
            }
            return result;
        }
    }  // anonymous namespace

    auto generate_csd_multiplier(const std::string& csd_str, int input_width, int max_power)
        -> std::string {
        // --- validation ---
        auto const len = static_cast<int>(csd_str.size());
        if (len != max_power + 1) {
            throw std::invalid_argument("CSD length " + std::to_string(len)
                                        + " doesn't match max_power=" + std::to_string(max_power)
                                        + " (should be max_power+1)");
        }

        // Validate characters (allows early error)
        for (auto const c : csd_str) {
            if (c != '+' && c != '-' && c != '0') {
                throw std::invalid_argument("CSD string can only contain '+', '-', or '0'");
            }
        }

        auto const terms = parse_terms(csd_str, max_power);
        auto const output_width = input_width + max_power;

        // --- module header ---
        std::string verilog;
        verilog += "\nmodule csd_multiplier (\n";
        verilog += "    input signed [" + std::to_string(input_width - 1)
                   + ":0] x,      // Input value\n";
        verilog += "    output signed [" + std::to_string(output_width - 1)
                   + ":0] result // Result of multiplication\n";
        verilog += ");";

        // --- wire declarations (deduplicated powers) ---
        if (!terms.empty()) {
            verilog += "\n\n    // Create shifted versions of input";
            std::set<int, std::greater<int>> powers_needed;
            for (auto const& term : terms) {
                powers_needed.insert(term.first);
            }
            for (auto const p : powers_needed) {
                verilog += "\n    wire signed [" + std::to_string(output_width - 1) + ":0] x_shift"
                           + std::to_string(p) + " = x <<< " + std::to_string(p) + ";";
            }
        }

        // --- detect LCSRe optimization opportunity ---
        auto const repeated = longest_repeated_substring(csd_str.c_str(), csd_str.size());

        bool use_opt = false;
        std::vector<size_t> pat_positions;
        if (!repeated.empty() && repeated.size() > 1) {
            // Count non-zero digits in the pattern — only optimize if it
            // actually saves hardware (≥2 non-zero terms per occurrence)
            int pat_nnz = 0;
            for (auto const c : repeated) {
                if (c == '+' || c == '-') ++pat_nnz;
            }
            if (pat_nnz >= 2) {
                pat_positions = find_pattern_occurrences(csd_str, repeated);
                use_opt = (pat_positions.size() >= 2);
            }
        }

        // --- combinational logic ---
        if (terms.empty()) {
            verilog += "\n\n    // CSD implementation";
            verilog += "\n    assign result = 0;";
        } else if (use_opt) {
            // --- LCSRe-optimized path: share repeated sub-expression ---
            auto const base_pos = pat_positions[0];

            // Generate the sub-expression wire for the pattern at its first occurrence
            auto const pat_expr = build_range_expr(csd_str, base_pos, repeated.size(), max_power);
            verilog += "\n\n    // LCSRe: repeated pattern \"" + repeated + "\"";
            verilog += "\n    wire signed [" + std::to_string(output_width - 1)
                       + ":0] _pat = " + pat_expr + ";";

            // Build the full expression from segments
            std::string expr;
            size_t cur = 0;
            for (size_t i = 0; i < pat_positions.size(); ++i) {
                auto const pos = pat_positions[i];

                // prefix/gap before this occurrence
                if (pos > cur) {
                    auto const gap_expr = build_range_expr(csd_str, cur, pos - cur, max_power);
                    if (!gap_expr.empty()) {
                        if (expr.empty()) {
                            expr = gap_expr;
                        } else {
                            expr += " + " + gap_expr;
                        }
                    }
                }

                // the pattern occurrence (shifted if not the first)
                auto const shift = static_cast<int>(pos - base_pos);
                auto const pat_ref = (shift == 0) ? std::string("_pat")
                                                  : "(_pat >>> " + std::to_string(shift) + ")";

                if (expr.empty()) {
                    expr = pat_ref;
                } else {
                    expr += " + " + pat_ref;
                }

                cur = pos + repeated.size();
            }

            // suffix after the last occurrence
            if (cur < csd_str.size()) {
                auto const suffix_expr
                    = build_range_expr(csd_str, cur, csd_str.size() - cur, max_power);
                if (!suffix_expr.empty()) {
                    expr += " + " + suffix_expr;
                }
            }

            verilog += "\n\n    // CSD implementation (LCSRe optimized)";
            verilog += "\n    assign result = " + expr + ";";
        } else {
            // --- flat path (no repeated pattern) ---
            verilog += "\n\n    // CSD implementation";
            std::string expr;
            for (size_t i = 0; i < terms.size(); ++i) {
                auto const power = terms[i].first;
                auto const op = terms[i].second;
                if (i == 0) {
                    if (op == TermOp::Sub) {
                        expr += "-";
                    }
                    expr += "x_shift" + std::to_string(power);
                } else {
                    expr += (op == TermOp::Add) ? " + x_shift" + std::to_string(power)
                                                : " - x_shift" + std::to_string(power);
                }
            }
            verilog += "\n    assign result = " + expr + ";";
        }

        verilog += "\nendmodule\n";
        return verilog;
    }
    // ------------------------------------------------------------------
    // Cross-CSE helpers
    // ------------------------------------------------------------------
    namespace {

        /**
         * @brief Count non-zero digits ('+' or '-') in a CSD substring
         * @param[in] s The CSD substring to analyze
         * @return Number of non-zero digit characters found
         */
        auto count_nnz(const std::string& s) -> int {
            int n = 0;
            for (auto c : s) {
                if (c == '+' || c == '-') ++n;
            }
            return n;
        }

        /**
         * @brief Find substrings that appear across multiple CSD strings
         *
         * Searches for substrings with at least 2 non-zero digits that appear in
         * 2 or more different CSD strings. These substrings are candidates for
         * cross-coefficient common sub-expression elimination.
         *
         * @param[in] csd_list List of CSD strings to search across
         * @return Map from pattern string to list of (coefficient_index, position) pairs
         */
        auto find_cross_patterns(const std::vector<std::string>& csd_list)
            -> std::map<std::string, std::vector<std::pair<int, int>>> {
            std::map<std::string, std::vector<std::pair<int, int>>> patterns;
            for (int ci = 0; ci < static_cast<int>(csd_list.size()); ++ci) {
                auto const& csd = csd_list[ci];
                auto const n = static_cast<int>(csd.size());
                for (int i = 0; i < n; ++i) {
                    for (int j = i + 2; j <= n; ++j) {
                        auto sub = csd.substr(i, j - i);
                        if (count_nnz(sub) >= 2) {
                            patterns[sub].emplace_back(ci, i);
                        }
                    }
                }
            }
            // Keep only patterns crossing >= 2 different CSD strings
            auto it = patterns.begin();
            while (it != patterns.end()) {
                std::set<int> unique_idx;
                for (auto const& occ_pair : it->second) {
                    unique_idx.insert(occ_pair.first);
                }
                if (unique_idx.size() < 2) {
                    it = patterns.erase(it);
                } else {
                    ++it;
                }
            }
            return patterns;
        }
    }  // anonymous namespace

    auto generate_csd_multipliers(const std::vector<MultiplierSpec>& coeffs,
                                  const std::string& module_name) -> std::string {
        if (coeffs.empty()) {
            throw std::invalid_argument("At least one coefficient is required");
        }

        // Validation and uniform-width enforcement
        auto const input_width = coeffs[0].input_width;
        auto const max_power = coeffs[0].max_power;

        for (auto const& spec : coeffs) {
            if (spec.input_width != input_width || spec.max_power != max_power) {
                throw std::invalid_argument(
                    "All coefficients must share the same input_width and max_power "
                    "for cross-CSE. Pad narrower CSDs with leading '0'.");
            }
            auto const len = static_cast<int>(spec.csd.size());
            if (len != max_power + 1) {
                throw std::invalid_argument("CSD length mismatch for '" + spec.name + "'");
            }
            for (auto c : spec.csd) {
                if (c != '+' && c != '-' && c != '0') {
                    throw std::invalid_argument("CSD string can only contain '+', '-', or '0'");
                }
            }
        }

        auto const output_width = input_width + max_power;

        // Collect all x_shift powers
        std::set<int, std::greater<int>> all_powers;
        for (auto const& spec : coeffs) {
            for (int i = 0; i < static_cast<int>(spec.csd.size()); ++i) {
                if (spec.csd[i] != '0') {
                    all_powers.insert(max_power - i);
                }
            }
        }

        // Find best cross-CSD pattern
        std::vector<std::string> csd_strings;
        csd_strings.reserve(coeffs.size());
        for (auto const& spec : coeffs) {
            csd_strings.push_back(spec.csd);
        }
        auto const cross = find_cross_patterns(csd_strings);

        std::string best_pattern;
        std::vector<std::pair<int, int>> best_occurrences;
        int best_score = 0;

        for (auto const& kv : cross) {
            auto const& pat = kv.first;
            auto const& occ = kv.second;
            auto const nnz = count_nnz(pat);
            auto const score_val = (nnz - 1) * (static_cast<int>(occ.size()) - 1);
            if (score_val > best_score) {
                best_score = score_val;
                best_pattern = pat;
                best_occurrences = occ;
            }
        }

        // Base position for the CSE wire
        int cse_base_pos = 0;
        if (!best_pattern.empty()) {
            cse_base_pos = best_occurrences[0].second;
            for (auto const& occ_pair : best_occurrences) {
                auto const pos = occ_pair.second;
                if (pos < cse_base_pos) {
                    cse_base_pos = pos;
                }
            }
        }

        // Build the Verilog module
        std::string verilog;
        verilog += "\nmodule " + module_name + " (";
        verilog += "\n    input signed [" + std::to_string(input_width - 1)
                   + ":0] x,      // Input value";

        for (auto const& spec : coeffs) {
            auto ow = spec.input_width + spec.max_power;
            verilog += "\n    output signed [" + std::to_string(ow - 1) + ":0] " + spec.name;
        }
        verilog += "\n);";

        // x_shift wires
        if (!all_powers.empty()) {
            verilog += "\n\n    // Create shifted versions of input";
            for (auto p : all_powers) {
                verilog += "\n    wire signed [" + std::to_string(output_width - 1) + ":0] x_shift"
                           + std::to_string(p) + " = x <<< " + std::to_string(p) + ";";
            }
        }

        // Shared CSE wire
        std::string cse_name = "_cse_0";
        if (!best_pattern.empty()) {
            auto const cse_expr
                = build_range_expr(best_pattern, 0, best_pattern.size(), max_power - cse_base_pos);
            verilog += "\n\n    // Cross-CSE: shared pattern \"" + best_pattern + "\"";
            verilog += "\n    wire signed [" + std::to_string(output_width - 1) + ":0] " + cse_name
                       + " = " + cse_expr + ";";
        }

        // Set of coeff indices that have the pattern
        std::set<int> cse_coeffs;
        for (auto const& occ_pair : best_occurrences) {
            cse_coeffs.insert(occ_pair.first);
        }

        // Per-coefficient assignments
        for (int idx = 0; idx < static_cast<int>(coeffs.size()); ++idx) {
            auto const& spec = coeffs[idx];
            auto ow = spec.input_width + spec.max_power;
            verilog += "\n\n    // " + spec.name + ": " + spec.csd;

            bool has_cse = !best_pattern.empty() && (cse_coeffs.count(idx) != 0U);
            auto const expr = has_cse ? build_coeff_expr(spec.csd, max_power, best_pattern,
                                                         cse_base_pos, cse_name)
                                      : build_coeff_expr(spec.csd, max_power, {}, 0, {});

            if (expr.empty()) {
                verilog += "\n    wire signed [" + std::to_string(ow - 1) + ":0] " + spec.name
                           + " = 0;";
            } else {
                verilog += "\n    wire signed [" + std::to_string(ow - 1) + ":0] " + spec.name
                           + " = " + expr + ";";
            }
        }

        verilog += "\nendmodule\n";
        return verilog;
    }
}  // namespace csd
