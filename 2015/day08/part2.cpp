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
    const auto special_characters{std::ranges::count(s, '\\') + std::ranges::count(s, '\"')};

    return 2 + static_cast<int>(special_characters); // add 2 for quotes
}

auto main() -> int {
    std::println("{}", std::ranges::fold_left(parse_input()
        | std::views::transform(literal_length_difference), 0, std::plus<int>{}));
}
