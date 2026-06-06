#include <algorithm>
#include <array>
#include <charconv>
#include <print>
#include <ranges>
#include <string>
#include <vector>
#include "aoc.h"

using Box = std::array<int, 3>;

auto parse_input() -> std::vector<Box> {
    auto f{AOC_INPUT()};
    std::vector<Box> dimensions{};
    std::string line{};

    auto parse = [](auto c) {
        int d{};
        std::from_chars(std::ranges::data(c), std::ranges::data(c) + std::ranges::size(c), d);
        return d;
    };

    while (std::getline(f, line)) {
        std::ranges::copy(line | std::views::split('x') | std::views::transform(parse),
            dimensions.emplace_back().begin());
    }

    return dimensions;
}

auto box_paper(Box box) -> int {
    std::ranges::sort(box);
    auto [l, w, h]{box};
    return 2 * (l*w + w*h + l*h) + l*w;
}

auto sum_wrapping_paper() -> int {
    return std::ranges::fold_left(parse_input() | std::views::transform(box_paper), 0, std::plus{});
}

auto main() -> int {
    std::println("{}", sum_wrapping_paper());
}
