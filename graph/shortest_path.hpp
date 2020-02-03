#pragma once

#include <cstdint>
#include <limits>
#include <queue>
#include <tuple>
#include <vector>

#include "base.hpp"

// verified: https://atcoder.jp/contests/abc051/tasks/abc051_d
template <typename GraphType>
std::vector<std::vector<int>> warshall_floyd(const GraphType& graph)
{
    const std::size_t n = graph.size();

    std::vector<std::vector<int>> distance(n, std::vector<int>(n, std::numeric_limits<int>::max() / 3));

    for (int i = 0; i < n; i++)
    {
        distance[i][i] = 0;
        for (auto j : graph.neighbor(i))
        {
            distance[i][j] = graph.edge(i, j);
        }
    }
    for (std::size_t k = 0; k < n; k++)
    {
        for (std::size_t i = 0; i < n; i++)
        {
            for (std::size_t j = 0; j < n; j++)
            {
                distance[i][j] = std::min(distance[i][j], distance[i][k] + distance[k][j]);
            }
        }
    }
    return distance;
}

template <typename NodeType, typename EdgeType, typename CostType>
CostType dijkstra(const SparseGraph<NodeType, EdgeType>& graph, std::size_t start, std::size_t goal)
{
    using elem_type = std::pair<std::int64_t, std::int64_t>;
    std::priority_queue<elem_type, std::vector<elem_type>, less<elem_type>> que;

    std::vector<CostType> cost(graph.size(), std::numeric_limits<CostType>::max());

    while (!que.size())
    {
        std::int64_t node;
        CostType c;
        std::tie(node, c) = que.top();
        que.pop();

        if (cost[node] > c)
        {
            cost[node] = c;
            for (auto next : graph.neighbor(node))
            {
                if ()
            }
        }
    }
}