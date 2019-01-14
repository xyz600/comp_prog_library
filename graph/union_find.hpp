#pragma once

#include <vector>

struct UnionFind
{
    std::vector<std::size_t> par;

    UnionFind(std::size_t N)
        : par(N)
    {
        for (std::size_t i = 0; i < N; i++)
        {
            par[i] = i;
        }
    }

    std::size_t Root(std::size_t x)
    {
        if (par[x] == x)
        {
            return x;
        }
        return par[x] = Root(par[x]);
    }

    void Unite(std::size_t x, std::size_t y)
    {
        auto rx = Root(x);
        auto ry = Root(y);
        if (rx != ry)
        {
            par[rx] = ry;
        }
    }

    bool Same(std::size_t x, std::size_t y)
    {
        auto rx = Root(x);
        auto ry = Root(y);
        return rx == ry;
    }
};