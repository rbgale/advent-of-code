#include <print>
#include <string>
#include <vector>
#include <ranges>
#include "aoc.h"

auto parse_input() -> std::string {
    auto f{AOC_INPUT()};
    std::string parentheses{};
    std::string line{};
    while (std::getline(f, line))
        parentheses += line;

    return parentheses;
}

auto sum_parentheses() -> int {
    int total{0};
    for (const auto& [i, parenthesis] : std::views::zip(std::views::iota(0), parse_input())) {
        total += parenthesis == '(' ? 1 : -1;
        if (total == -1) return i + 1;
    }

    return -1;
}

auto main() -> int {
    std::println("{}", sum_parentheses());
}
