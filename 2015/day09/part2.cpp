#include <algorithm>
#include <print>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "aoc.h"

using Graph = std::unordered_map<std::string, std::unordered_map<std::string, int>>;

auto parse_graph() -> Graph {
    Graph graph{};

    const auto connections{aoc::parse_lines([](const auto& line) {
        // line structure: "[source] to [destination] = [distance]"
        auto words{aoc::split(line, ' ')};
        return std::make_tuple(words[0], words[2], std::stoi(words[4]));
    })};

    for (const auto& [source, destination, distance] : connections) {
        graph[source][destination] = distance;
        graph[destination][source] = distance;
    }

    return graph;
}

auto longest_hamiltonian_path(
    const Graph& graph,
    std::unordered_set<std::string>& remaining_nodes,
    const std::string& start
) -> int {
    if (remaining_nodes.empty())
        return 0;

    int max_remaining_path{std::numeric_limits<int>::min()};

    for (const auto& [neighbor, distance] : graph.at(start)) {
        if (!remaining_nodes.contains(neighbor)) continue;

        remaining_nodes.erase(neighbor);
        const int current_remaining_path{longest_hamiltonian_path(graph, remaining_nodes, neighbor)};

        if (current_remaining_path != std::numeric_limits<int>::min()) {
            max_remaining_path = std::max(
                max_remaining_path,
                distance + current_remaining_path
            );
        }
        remaining_nodes.insert(neighbor);
    }

    return max_remaining_path;
}

auto longest_hamiltonian_path(const Graph& graph) -> int {
    const auto nodes{std::ranges::to<std::unordered_set<std::string>>(std::views::keys(graph))};
    auto remaining_nodes = nodes;

    return std::ranges::max(nodes | std::views::transform([&](const auto& node) {
        remaining_nodes.erase(node);
        int result{longest_hamiltonian_path(graph, remaining_nodes, node)};
        remaining_nodes.insert(node);

        return result;
    }));
}

auto main() -> int {
    std::println("{}", longest_hamiltonian_path(parse_graph()));
}
