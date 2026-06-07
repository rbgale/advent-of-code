#include <print>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "aoc.h"

using Point = std::pair<int, int>;

const std::unordered_map<char, Point> DIRECTION_MAP{
    {'<', {-1, 0}},
    {'>', {1, 0}},
    {'^', {0, 1}},
    {'v', {0, -1}}
};

auto parse_input() -> std::vector<Point> {
    auto f{AOC_INPUT()};
    std::vector<Point> directions{};
    std::string line{};
    while (std::getline(f, line))
        for (const auto c : line)
            directions.emplace_back(DIRECTION_MAP.at(c));

    return directions;
}

auto sum_unique_locations() -> int {
    Point current{0, 0};
    std::unordered_set<Point, PairHash> seen{current};

    for (const auto& [x, y] : parse_input()) {
        current.first += x;
        current.second += y;
        seen.insert(current);
    }

    return static_cast<int>(seen.size());
}

auto main() -> int {
    std::println("{}", sum_unique_locations());
}
