import std;
import aoc;

namespace {

auto build_happiness_map(const std::vector<std::tuple<std::string, std::string, int>>& rows)
        -> std::map<std::string, std::map<std::string, int>> {
    std::map<std::string, std::map<std::string, int>> happinesses{};

    for (const auto& [person, neighbor, happiness] : rows)
        happinesses[person][neighbor] = happiness;

    return happinesses;
}

// Treats mirrored seatings as distinct for simplicity since performance difference is negligible
auto generate_seating_permutations(auto people) -> std::vector<std::vector<std::string>> {
    std::vector<std::vector<std::string>> permutations{};

    do {
        permutations.push_back(people);
    } while (std::ranges::next_permutation(people | std::views::drop(1)).found);

    return permutations;
}

auto compute_happiness_change(const std::map<std::string, std::map<std::string, int>>& happinesses) -> long long {
    long long max_happiness = std::numeric_limits<long long>::min();

    auto compute_permutation_happiness = [&](const std::vector<std::string>& permutation) -> long long {
        long long happiness = 0;

        for (const auto& [i, entry] : std::views::enumerate(permutation)) {
            const std::size_t left = aoc::modulo(i - 1, static_cast<long>(permutation.size()));
            const std::size_t right = aoc::modulo(i + 1, static_cast<long>(permutation.size()));

            happiness += happinesses.at(entry).at(permutation[left]) + happinesses.at(entry).at(permutation[right]);
        }

        return happiness;
    };

    for (auto& permutation :
         generate_seating_permutations(happinesses | std::views::keys | std::ranges::to<std::vector>())) {
        max_happiness = std::max(max_happiness, compute_permutation_happiness(permutation));
    }

    return max_happiness;
}

} // namespace

auto main() -> int {
    const std::regex pattern{R"(^(\w+) would (?:(gain)|lose) (\d+) happiness units by sitting next to (\w+)\.$)"};
    auto parser = [&](std::string_view line) -> std::tuple<std::string, std::string, int> {
        std::match_results<std::string_view::const_iterator> m;
        std::regex_search(line.begin(), line.end(), m, pattern);
        return {m[1], m[4], (m[2].matched ? 1 : -1) * aoc::extract_number(m[3].str())};
    };
    std::println("{}", compute_happiness_change(build_happiness_map(aoc::parse_lines(parser))));
}
