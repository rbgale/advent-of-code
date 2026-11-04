#include <iostream>
#include <print>
#include <vector>
#include "aoc.h"

constexpr int NUM_ROUNDS = 50;

auto look_and_say(const std::string& numbers, const int rounds) -> std::string {
    if (rounds == 0)
        return numbers;

    std::string result{};

    size_t i{0uz};
    while (i < numbers.length()) {
        const char current{numbers[i]};
        int streak{1uz};

        while (++i < numbers.length() && current == numbers[i])
            ++streak;

        result += std::to_string(streak) + current;
    }

    return look_and_say(result, rounds - 1);
}

auto main() -> int {
    std::println("{}", look_and_say(aoc::read_multiline(), NUM_ROUNDS).length());
}
