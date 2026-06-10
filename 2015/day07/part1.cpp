#include <functional>
#include <print>
#include <ranges>
#include <string>
#include <variant>
#include <vector>
#include "aoc.h"

using Wire = std::string;
using Unary = std::function<uint16_t(uint16_t)>;
using Binary = std::function<uint16_t(uint16_t, uint16_t)>;

enum class InstructionType { source, lshift, rshift, land, lor, lnot };

struct Instruction {
    std::variant<Wire, uint16_t> left;
    std::optional<std::variant<Wire, uint16_t>> right;
    std::variant<Unary, Binary> f;

    Instruction(
        const InstructionType t,
        const std::variant<Wire, uint16_t>& left,
        const std::variant<Wire, uint16_t>& right)
            : left{left}, right{right}, f{get_function(t)} {}

    Instruction(
        const InstructionType t,
        const std::variant<Wire, uint16_t>& left)
            : left{left}, f{get_function(t)} {}

    static auto get_function(const InstructionType t) -> std::variant<Unary, Binary> {
        switch (t) {
            case InstructionType::lshift:
                return [](const uint16_t value, const uint16_t shift) { return value << shift; };
            case InstructionType::rshift:
                return [](const uint16_t value, const uint16_t shift) { return value >> shift; };
            case InstructionType::land:
                return [](const uint16_t left, const uint16_t right) { return left & right; };
            case InstructionType::lor:
                return [](const uint16_t left, const uint16_t right) { return left | right; };
            case InstructionType::lnot:
                return [](const uint16_t value) { return ~value; };
            default:
                return [](const uint16_t value) { return value; };
        }
    }
};

auto evaluate_wire(const Wire& wire, std::unordered_map<Wire, Instruction>& instructions) -> uint16_t {
    const auto instruction{instructions.at(wire)};

    const uint16_t left = [&] {
        if (std::holds_alternative<uint16_t>(instruction.left))
            return std::get<uint16_t>(instruction.left);
        return evaluate_wire(std::get<Wire>(instruction.left), instructions);
    }();

    const uint16_t output = [&] {
        if (std::holds_alternative<Binary>(instruction.f)) {
            const uint16_t right = [&] {
                if (std::holds_alternative<uint16_t>(*instruction.right))
                    return std::get<uint16_t>(*instruction.right);
                return evaluate_wire(std::get<Wire>(*instruction.right), instructions);
            }();

            return std::get<Binary>(instruction.f)(left, right);
        }

        return std::get<Unary>(instruction.f)(left);
    }();

    instructions.at(wire) = {InstructionType::source, output};
    return output;
}

auto parse_line(const std::string& line) -> std::pair<Wire, Instruction> {
    const auto parts{line | std::views::split(' ') | std::ranges::to<std::vector<std::string>>()};

    const Wire& destination{parts.back()};

    const InstructionType t = [&] {
        switch (parts[1][0]) {
            case 'A': return InstructionType::land;
            case 'O': return InstructionType::lor;
            case 'L': return InstructionType::lshift;
            case 'R': return InstructionType::rshift;
            case '-': return InstructionType::source;
            default: return InstructionType::lnot;
        }
    }();

    const auto [left_index, right_index] = [&] -> std::pair<size_t, size_t> {
        if (parts[0] == "NOT") return {1, 1};
        if (parts[1] == "->") return {0, 0};
        return {0, 2};
    }();

    auto get_wire_or_source = [](const std::string& input) -> std::variant<Wire, uint16_t> {
        if (isdigit(input[0])) return static_cast<uint16_t>(stoul(input));
        return input;
    };

    const auto left{get_wire_or_source(parts[left_index])};

    if (left_index == right_index)
        return {destination, {t, left}};

    return {destination, {t, left, get_wire_or_source(parts[right_index])}};
}

auto parse_input() -> std::unordered_map<Wire, Instruction> {
    auto f{AOC_INPUT()};
    std::unordered_map<Wire, Instruction> instructions{};
    std::string line{};
    while (std::getline(f, line)) {
        const auto [destination, instruction] = parse_line(line);
        instructions.emplace(destination, instruction);
    }

    return instructions;
}

auto main() -> int {
    auto instructions{parse_input()};
    std::println("{}", evaluate_wire("a", instructions));
}
