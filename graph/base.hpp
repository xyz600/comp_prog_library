#pragma once

#include <algorithm>
#include <cstdint>
#include <map>
#include <unordered_set>
#include <vector>

template <typename N = std::size_t, typename E = std::size_t>
class SparseGraph
{
public:
    using NodeType = N;
    using EdgeType = E;
    using NeighborListType = std::unordered_set<std::size_t>;

    SparseGraph(const std::size_t size, bool undirected = true) noexcept
        : neighbor_list_(size)
        , undirected_(undirected)
        , nodes_(size)
    {
    }

    void connect(const std::size_t from, const std::size_t to, const EdgeType edge) noexcept
    {
        const std::size_t new_size = std::max(from, to) + 1;

        if (neighbor_list_.size() < new_size)
        {
            neighbor_list_.resize(new_size);
            nodes_.resize(new_size);
        }

        if (undirected_)
        {
            neighbor_list_[from].insert(to);
            edges_[std::make_pair(std::min(from, to), std::max(from, to))] = edge;
        }
        else
        {
            neighbor_list_[from].insert(to);
            neighbor_list_[to].insert(from);
            edges_[std::make_pair(from, to)] = edge;
        }
    }

    void disconnect(const std::size_t from, const std::size_t to) noexcept
    {
        if (undirected_)
        {
            neighbor_list_[from].erase(to);
            edges_.erase(std::make_pair(std::min(from, to), std::max(from, to)));
        }
        else
        {
            neighbor_list_[from].erase(to);
            neighbor_list_[to].erase(from);
            edges_.erase(std::make_pair(from, to));
        }
    }

    const NodeType& node(const std::size_t index) const noexcept
    {
        return nodes_.at(index);
    }

    const EdgeType& edge(const std::size_t from, const std::size_t to) const noexcept
    {
        if (undirected_)
        {
            auto key = std::make_pair(std::min(from, to), std::max(from, to));
            return edges_.at(key);
        }
        else
        {
            auto key = std::make_pair(from, to);
            return edges_.at(key);
        }
    }

    const NeighborListType&
    neighbor(const std::size_t index) const noexcept
    {
        return neighbor_list_[index];
    }

    std::size_t size() const noexcept
    {
        return neighbor_list_.size();
    }

private:
    using EdgeMap = std::map<std::pair<std::size_t, std::size_t>, EdgeType>;

    std::vector<NeighborListType> neighbor_list_;

    EdgeMap edges_;

    std::vector<NodeType> nodes_;

    bool undirected_;
};

template <typename N = std::size_t, typename E = std::size_t>
class DenseGraph
{
public:
    using NodeType = N;
    using EdgeType = E;
    using NeighborListType = std::vector<std::size_t>;

    DenseGraph(const std::size_t size, bool undirected = true) noexcept
        : neighbor_list_(size)
        , undirected_(undirected)
        , nodes_(size)
    {
    }

    void connect(const std::size_t from, const std::size_t to, const EdgeType edge) noexcept
    {
        const std::size_t new_size = std::max(from, to) + 1;

        if (neighbor_list_.size() < new_size)
        {
            neighbor_list_.resize(new_size);
            nodes_.resize(new_size);
        }

        if (undirected_)
        {
            neighbor_list_[from].push_back(to);
            edges_[std::make_pair(std::min(from, to), std::max(from, to))] = edge;
        }
        else
        {
            neighbor_list_[from].push_back(to);
            neighbor_list_[to].push_back(from);
            edges_[std::make_pair(from, to)] = edge;
        }
    }

    void disconnect(const std::size_t from, const std::size_t to) noexcept
    {
        if (undirected_)
        {
            neighbor_list_[from].erase(std::find(neighbor_list_[from].begin(), neighbor_list_[from].end(), to));
            edges_.erase(std::make_pair(std::min(from, to), std::max(from, to)));
        }
        else
        {
            neighbor_list_[from].erase(std::find(neighbor_list_[from].begin(), neighbor_list_[from].end(), to));
            neighbor_list_[to].erase(std::find(neighbor_list_[to].begin(), neighbor_list_[to].end(), from));
            edges_.erase(std::make_pair(from, to));
        }
    }

    const NodeType& node(const std::size_t index) const noexcept
    {
        return nodes_.at(index);
    }

    const EdgeType& edge(const std::size_t from, const std::size_t to) const noexcept
    {
        if (undirected_)
        {
            auto key = std::make_pair(std::min(from, to), std::max(from, to));
            return edges_.at(key);
        }
        else
        {
            auto key = std::make_pair(from, to);
            return edges_.at(key);
        }
    }

    const NeighborListType&
    neighbor(const std::size_t index) const noexcept
    {
        return neighbor_list_[index];
    }

    std::size_t size() const noexcept
    {
        return neighbor_list_.size();
    }

private:
    using EdgeMap = std::map<std::pair<std::size_t, std::size_t>, EdgeType>;

    std::vector<NeighborListType> neighbor_list_;

    EdgeMap edges_;

    std::vector<NodeType> nodes_;

    bool undirected_;
};