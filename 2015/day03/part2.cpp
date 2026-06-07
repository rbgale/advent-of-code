#include <print>
#include <string>
#include <ranges>
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
    std::array<Point, 2> movers{};
    std::unordered_set<Point, PairHash> seen{{0, 0}};

    for (const auto& [i, delta] : std::views::zip(std::views::iota(0), parse_input())) {
        auto& mover{movers[i % 2]};
        mover.first += delta.first;
        mover.second += delta.second;
        seen.insert(mover);
    }

    return static_cast<int>(seen.size());
}

auto main() -> int {
    std::println("{}", sum_unique_locations());
}
