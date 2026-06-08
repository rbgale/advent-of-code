#include <algorithm>
#include <map>
#include <print>
#include <ranges>
#include <string>
#include <vector>
#include "aoc.h"

std::vector<std::string> FORBIDDEN_STRINGS{"ab", "cd", "pq", "xy"};

auto parse_input() -> std::vector<std::string> {
    auto f{AOC_INPUT()};
    std::vector<std::string> strings{};
    std::string line{};
    while (std::getline(f, line))
        strings.push_back(line);

    return strings;
}

auto has_disjoint_pairs(const std::string& s) -> bool {
    std::map<char, int> vowel_count{{'a', 0}, {'e', 0}, {'i', 0}, {'o', 0}, {'u', 0}};

    for (const auto& c : s)
        if (const auto it{vowel_count.find(c)}; it != vowel_count.end())
            it->second += 1;

    return 3 <= std::ranges::fold_left(vowel_count, 0, [](int acc, const auto& pair) {
        return acc + pair.second;
    });
}

auto has_spaced_repeated_letter(const std::string& s) -> bool {
    for (auto i{1uz}; i < s.size(); ++i)
        if (s[i - 1] == s[i]) return true;

    return false;
}

auto has_no_forbidden_strings(const std::string& s) -> bool {
    for (auto i{1uz}; i < s.size(); ++i)
        for (const auto& f : FORBIDDEN_STRINGS)
            if (s.substr(i - 1, 2) == f) return false;

    return true;
}

auto is_nice(const std::string& s) -> bool {
    return has_disjoint_pairs(s) && has_spaced_repeated_letter(s) && has_no_forbidden_strings(s);
}

auto count_nice_strings() -> long {
    const auto strings{parse_input()};
    return std::ranges::count_if(strings.begin(), strings.end(), is_nice);
}

auto main() -> int {
    std::println("{}", count_nice_strings());
}
