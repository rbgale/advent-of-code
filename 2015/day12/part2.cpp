#include <algorithm>
#include <cstddef>
#include <functional>
#include <print>
#include <string>
#include <string_view>

#include "aoc.h"

namespace {

constexpr std::string UNWANTED = ":\"red\"";

auto remove_all_red(std::string text) -> std::string {
    auto find_balance = [&](size_t i, int balance, const int incr) -> size_t {
        for (i += incr; i < text.length(); i += incr) {
            if (text[i] == '{')
                ++balance;
            else if (text[i] == '}')
                --balance;
            if (balance == 0)
                break;
        }
        return i;
    };

    for (size_t left = text.find(UNWANTED); left != std::string_view::npos; left = text.find(UNWANTED, left)) {
        left = find_balance(left, -1, -1);
        const size_t right = find_balance(left, 1, 1);
        text.erase(left, 1 + right - left);
    }

    return text;
}

auto add_valid_numbers(const std::string& text) -> long long {
    return std::ranges::fold_left(aoc::extract_numbers(remove_all_red(text)), 0, std::plus{});
}

} // namespace

auto main() -> int {
    std::println("{}", add_valid_numbers(aoc::read_multiline()));
}
