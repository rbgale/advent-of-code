#pragma once

#include <charconv>
#include <filesystem>
#include <format>
#include <fstream>
#include <ranges>
#include <optional>
#include <source_location>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace aoc {

[[nodiscard]] inline auto open_source_input(const std::source_location source) -> std::ifstream {
    const auto path{std::filesystem::path(source.file_name()).parent_path() / "input.txt"};
    std::ifstream f{path};

    if (!f)
        throw std::runtime_error(std::format("Unable to open '{}'.", path.string()));

    return f;
}

template <std::integral T>
[[nodiscard]] auto modulo(const T dividend, const T modulus) -> T {
    const T result = dividend % modulus;
    return result < 0 ? result + modulus : result;
}

[[nodiscard]] inline auto read_lines(
    std::istream&& f = open_source_input(std::source_location::current())
) -> std::vector<std::string> {
    std::vector<std::string> lines{};
    std::string line{};

    while (std::getline(f, line))
        lines.push_back(line);

    return lines;
}

[[nodiscard]] inline auto read_multiline(
    std::istream&& f = open_source_input(std::source_location::current())
) -> std::string {
    std::string s{};
    std::string line{};

    while (std::getline(f, line))
        s += line;

    return s;
}

[[nodiscard]] inline auto read_sections(
    std::istream&& f = open_source_input(std::source_location::current())
) -> std::vector<std::vector<std::string>> {
    return read_lines(std::move(f))
        | std::views::split(std::string{})
        | std::ranges::to<std::vector<std::vector<std::string>>>();
}

template <typename F = std::identity>
[[nodiscard]] auto parse_lines(
    F parse={}, std::istream&& f = open_source_input(std::source_location::current())) {
    return read_lines(std::move(f))
        | std::views::transform(std::move(parse))
        | std::ranges::to<std::vector>();
}

[[nodiscard]] inline auto split(std::string_view s, std::string_view delimiter) -> std::vector<std::string> {
    return s | std::views::split(delimiter) | std::ranges::to<std::vector<std::string>>();
}

[[nodiscard]] inline auto split(std::string_view s, char delimiter) -> std::vector<std::string> {
    return s | std::views::split(delimiter) | std::ranges::to<std::vector<std::string>>();
}

template <typename T>
concept parsable_number = requires(const char* p, T& v) {
    std::from_chars(p, p, v);
};

template <parsable_number T = long long>
[[nodiscard]] auto try_extract_number(
    std::string_view s,
    std::size_t start = 0uz,
    std::size_t* end_pos = nullptr
) -> std::optional<T> {
    start = s.find_first_of("0123456789", start);

    if (start == std::string_view::npos)
        return std::nullopt;

    if (std::is_signed_v<T> && start > 0 && s[start - 1] == '-' && (start == 1 || !isdigit(s[start - 2])))
        start -= 1;

    T value;
    const auto [end_ptr, ec] = std::from_chars(s.data() + start, s.data() + s.size(), value);

    if (ec == std::errc::result_out_of_range)
        throw std::out_of_range(std::format("Number in string '{}' is out of range.", s));

    if (end_pos)
        *end_pos = static_cast<std::size_t>(end_ptr - s.data());

    return value;
}

template <parsable_number T = long long>
[[nodiscard]] auto extract_number(
    std::string_view s,
    std::size_t start = 0uz,
    std::size_t* end_pos = nullptr
) -> T {
    if (auto number = try_extract_number<T>(s, start, end_pos))
        return *number;

    throw std::invalid_argument(std::format("No numbers found in string '{}'.", s));
}

template <parsable_number T = long long>
[[nodiscard]] auto extract_numbers(
    std::string_view s,
    std::size_t start = 0uz,
    std::size_t* end_pos = nullptr
) -> std::vector<T> {
    std::vector<T> numbers{};

    while (auto maybe_number = try_extract_number<T>(s, start, &start))
        numbers.push_back(*maybe_number);

    if (end_pos)
        *end_pos = start;

    return numbers;
}

// hashing method is from boost::hash_combine
template <typename T>
auto hash_combine(std::size_t& seed, const T& val) -> void {
    seed ^= std::hash<T>{}(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

struct PairHash {
    template <typename T, typename U = T>
    std::size_t operator()(const std::pair<T, U>& p) const {
        std::size_t seed = 0;
        hash_combine(seed, p.first);
        hash_combine(seed, p.second);

        return seed;
    }
};

}
