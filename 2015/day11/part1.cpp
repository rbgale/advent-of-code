#include <algorithm>
#include <array>
#include <cstddef>
#include <optional>
#include <print>
#include <ranges>
#include <string>

#include "aoc.h"

namespace {

constexpr std::array FORBIDDEN = {'i', 'o', 'l'};

auto increment_password(std::string& password) -> void {
    for (char& it : std::views::reverse(password)) {
        if (it != 'z') {
            it += 1;
            break;
        }

        it = 'a';
    }
}

auto is_valid_password(const std::string& password) -> bool {
    bool has_three_straight = false;
    std::optional<size_t> latest_pair = std::nullopt;
    int num_distinct_pairs = 0;

    for (size_t i{0}; i < password.length(); ++i) {
        if (std::ranges::contains(FORBIDDEN, password[i]))
            return false;

        if (i > 0 && password[i] == password[i - 1] && latest_pair != i - 1) {
            ++num_distinct_pairs;
            latest_pair = static_cast<int>(i);
        }

        if (password.length() > i + 2 && password[i + 2] == password[i + 1] + 1 && password[i + 1] == password[i] + 1)
            has_three_straight = true;
    }

    return has_three_straight && num_distinct_pairs >= 2;
}

auto get_next_valid_password(std::string&& password) -> std::string {
    do {
        increment_password(password);
    } while (!is_valid_password(password));

    return password;
}

} // namespace

auto main() -> int {
    std::println("{}", get_next_valid_password(aoc::read_multiline()));
}
