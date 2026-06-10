#include <algorithm>
#include <print>
#include <ranges>
#include <string>
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

auto literal_length_difference(const std::string& s) -> int {
    int difference{2}; // add 2 for quotes
    size_t i{1uz}; // trim quotes

    while (i < s.length() - 1) { // trim quotes
        i = s.find('\\', i);

        if (i == std::string::npos) break;

        switch (s[i + 1]) {
            case '\\':
            case '\"':
                difference++;
                i += 1;
                break;
            case 'x':
                difference += 3;
                i += 3;
                break;
            default:
                throw std::invalid_argument("invalid character");
        }
        i++;
    }

    return difference;
}

auto main() -> int {
    std::println("{}", std::ranges::fold_left(parse_input()
        | std::views::transform(literal_length_difference), 0, std::plus<int>{}));
}
