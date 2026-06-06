#include <print>
#include <string>
#include <vector>
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
    for (const auto& parenthesis : parse_input())
        total += parenthesis == '(' ? 1 : -1;

    return total;
}

auto main() -> int {
    std::println("{}", sum_parentheses());
}
