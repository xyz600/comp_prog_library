#pragma once

#include <stack>
#include <tuple>
#include <vector>

#include "base.hpp"

template <typename GraphType>
std::vector<std::size_t> topological_order(const GraphType& graph)
{
    std::vector<bool> decided(graph.size(), false);
    std::vector<std::size_t> order;

    for (int ns = 0; ns < graph.size(); ns++)
    {
        if (!decided[ns])
        {
            // (node, neihgbor index)
            stack<std::pair<int, int>> s;
            s.emplace(ns, 0);

            while (!s.empty())
            {
                int n, i;
                tie(n, i) = s.top();
                s.pop();

                if (!decided[n])
                {
                    // 帰りがけのタイミング
                    if (i == graph.neighbor(n).size())
                    {
                        decided[n] = true;
                        order.push_back(n);
                    }
                    else
                    {
                        s.emplace(n, i + 1);
                        for (auto next : graph.neighbor(n))
                        {
                            if (!decided[next])
                            {
                                s.emplace(next, 0);
                            }
                        }
                    }
                }
            }
        }
    }
    assert(order.size() == graph.size());
    return order;
}
