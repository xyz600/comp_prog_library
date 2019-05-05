#pragma once

#include <iostream>
#include <vector>

struct UnionFind
{
    std::vector<std::size_t> _par;
    std::vector<std::size_t> _size;
    std::vector<std::size_t> _rank;

    UnionFind(std::size_t N)
        : _par(N)
        , _size(N, 1)
        , _rank(N, 0)
    {
        for (std::size_t i = 0; i < N; i++)
        {
            _par[i] = i;
        }
    }

    std::size_t Root(std::size_t x)
    {
        if (_par[x] == x)
        {
            return x;
        }
        return _par[x] = Root(_par[x]);
    }

    void Unite(std::size_t x, std::size_t y)
    {
        auto rx = Root(x);
        auto ry = Root(y);
        if (rx != ry)
        {
            if (_rank[rx] < _rank[ry])
            {
                _par[rx] = ry;
                _size[ry] += _size[rx];
            }
            else
            {
                _par[ry] = rx;
                _size[rx] += _size[ry];
                if (_rank[rx] == _rank[ry])
                {
                    _rank[ry]++;
                }
            }
        }
    }

    size_t Size(std::size_t x)
    {
        return _size[Root(x)];
    }

    bool Same(std::size_t x, std::size_t y)
    {
        auto rx = Root(x);
        auto ry = Root(y);
        return rx == ry;
    }
};