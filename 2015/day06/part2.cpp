#include <algorithm>
#include <charconv>
#include <print>
#include <ranges>
#include <string>
#include <vector>
#include "aoc.h"

enum class InstructionType { on, off, toggle };

using point = std::pair<int, int>;

struct Instruction {
    InstructionType t;
    point start;
    point end;
};

auto parse_int = [](auto c) -> int {
    int d{};
    std::from_chars(std::ranges::data(c), std::ranges::data(c) + std::ranges::size(c), d);
    return d;
};

auto parse_pair(const std::string& s, char split = ',') -> point {
    const auto nums = s
        | std::views::split(split)
        | std::views::transform(parse_int)
        | std::ranges::to<std::vector<int>>();

    return {nums[0], nums[1]};
}

auto parse_line(const std::string& line) -> Instruction {
    const auto parts{line | std::views::split(' ') | std::ranges::to<std::vector<std::string>>()};

    size_t offset{0uz};
    if (parts[0] == "turn") offset++;

    const InstructionType t = [&] {
        if (parts[offset] == "on") return InstructionType::on;
        if (parts[offset] == "off") return InstructionType::off;
        return InstructionType::toggle;
    }();

    return {t, parse_pair(parts[1 + offset]), parse_pair(parts[3 + offset])};
}

auto parse_input() -> std::vector<Instruction> {
    auto f{AOC_INPUT()};
    std::vector<Instruction> instructions{};
    std::string line{};
    while (std::getline(f, line))
        instructions.emplace_back(parse_line(line));

    return instructions;
}

auto update(std::vector<std::vector<int>>& lights, const Instruction& instruction) {
    for (auto i{instruction.start.first}; i <= instruction.end.first; ++i)
        for (auto j{instruction.start.second}; j <= instruction.end.second; ++j)
            lights[i][j] += [&] {
                if (instruction.t == InstructionType::on) return 1;
                if (instruction.t == InstructionType::off) return lights[i][j] == 0 ? 0 : -1;
                return 2;
            }();
}

auto count_lights() -> int {
    std::vector lights{1000, std::vector(1000, 0)};
    const auto instructions{parse_input()};

    for (const auto& instruction : instructions)
        update(lights, instruction);

    return std::ranges::fold_left(lights, 0, [](const int acc, const std::vector<int>& row) {
       return acc + std::ranges::fold_left(row, 0, std::plus<int>{});
    });
}

auto main() -> int {
    std::println("{}", count_lights());
}
