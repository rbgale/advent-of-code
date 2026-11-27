#include <algorithm>
#include <print>

#include "aoc.h"

auto main() -> int {
    std::println("{}", std::ranges::fold_left(aoc::extract_numbers(aoc::read_multiline()), 0, std::plus{}));
}
