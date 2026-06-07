#include <print>
#include <string>
#include <ranges>
#include <openssl/md5.h>
#include "aoc.h"

auto parse_input() -> std::string {
    auto f{AOC_INPUT()};
    std::string key{};
    std::string line{};
    while (std::getline(f, line))
        key += line;

    return key;
}

auto get_MD5(const std::string& input) -> std::string {
    unsigned char hash[MD5_DIGEST_LENGTH];
    MD5(reinterpret_cast<const unsigned char*>(input.data()), input.size(), hash);

    constexpr std::string hash_lookup{"0123456789abcdef"};
    std::string result{};

    for (auto i{MD5_DIGEST_LENGTH - 1}; i >= 0; --i) {
        result += hash_lookup.at(static_cast<uint>(hash[i] & 0xf));
        result += hash_lookup.at(static_cast<uint>(hash[i] >> 4));
    }

    return {result.rbegin(), result.rend()};
}

auto least_valid_hash_key_completion() -> std::string {
    auto partial_key{parse_input()};

    for (auto i{0uz}; i < INT_MAX; ++i) {
        auto key_completion{std::to_string(i)};
        if (get_MD5(partial_key + key_completion).starts_with("00000"))
            return key_completion;
    }

    throw std::runtime_error{"No eligible hash found."};
}

auto main() -> int {
    std::println("{}", least_valid_hash_key_completion());
}
