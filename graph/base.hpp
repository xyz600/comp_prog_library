#pragma once

#include <cstdint>
#include <unordered_set>
#include <vector>

class SparseGraph
{
public:
    using NeighborListType = std::unordered_set<std::size_t>;

    SparseGraph(const std::size_t size) noexcept
        : neighbor_list(size)
    {
    }

    void connect(const std::size_t from, const std::size_t to, const bool undirected = false) noexcept
    {
        neighbor_list[from].insert(to);
        if (undirected)
        {
            neighbor_list[to].insert(from);
        }
    }

    const NeighborListType& operator[](const std::size_t index) const noexcept
    {
        return neighbor_list[index];
    }

    std::size_t size() const noexcept
    {
        return neighbor_list.size();
    }

private:
    std::vector<NeighborListType> neighbor_list;
};