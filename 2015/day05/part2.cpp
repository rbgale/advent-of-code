#include <algorithm>
#include <map>
#include <print>
#include <ranges>
#include <string>
#include <unordered_map>
#include <vector>
#include "aoc.h"

auto parse_input() -> std::vector<std::string> {
    auto f{AOC_INPUT()};
    std::vector<std::string> strings{};
    std::string line{};
    while (std::getline(f, line))
        strings.push_back(line);

    return strings;
}

auto has_disjoint_pairs(const std::string& s) -> bool {
    std::unordered_map<std::string, std::vector<size_t>> pairs_to_locations;

    for (auto i{1uz}; i < s.size(); ++i)
        pairs_to_locations[s.substr(i - 1, 2)].push_back(i - 1);

    return std::ranges::any_of(pairs_to_locations | std::views::values,
        [](const auto& locations) {
            return *std::ranges::max_element(locations) - *std::ranges::min_element(locations) > 1;
        }
    );
}

auto has_spaced_repeated_letter(const std::string& s) -> bool {
    for (auto i{2uz}; i < s.size(); ++i)
        if (s[i - 2] == s[i]) return true;

    return false;
}

auto is_nice(const std::string& s) -> bool {
    return has_disjoint_pairs(s) && has_spaced_repeated_letter(s);
}

auto count_nice_strings() -> long {
    const auto strings{parse_input()};
    return std::ranges::count_if(strings.begin(), strings.end(), is_nice);
}

auto main() -> int {
    std::println("{}", count_nice_strings());
}
