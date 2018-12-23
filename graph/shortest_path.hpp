#pragma once

#include <vector>

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
            distance[i][j] = distance[j][i] = graph.edge(i, j);
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
