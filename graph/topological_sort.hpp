#pragma once

#include <stack>
#include <tuple>
#include <vector>

#include "base.hpp"
// verified @ https://nikkei2019-qual.contest.atcoder.jp/tasks/nikkei2019_qual_d
template <typename GraphType>
std::vector<std::size_t> topological_order(const GraphType& graph)
{
    std::vector<std::size_t> order;
    stack<int> st;

    vector<int> hs(graph.size());

    for (int v = 0; v < graph.size(); v++)
    {
        for (int nv : graph.neighbor(v))
        {
            hs[nv]++;
        }
    }
    for (int i = 0; i < graph.size(); i++)
    {
        if (hs[i] == 0)
        {
            st.push(i);
        }
    }

    vector<int> finished(graph.size(), false);

    while (!st.empty())
    {
        const int v = st.top();
        st.pop();
        order.push_back(v);
        finished[v] = true;

        for (int nv : graph.neighbor(v))
        {
            hs[nv]--;
            if (hs[nv] == 0 && !finished[nv])
            {
                st.push(nv);
            }
        }
    }
    assert(order.size() == graph.size());
    return order;
}