#pragma once

#include <algorithm>
#include <cstdint>
#include <map>
#include <queue>
#include <unordered_set>
#include <vector>

template <typename N = std::size_t, typename E = std::size_t>
class SparseGraph
{
public:
    using NodeType = N;
    using EdgeType = E;
    using NeighborListType = std::unordered_set<std::size_t>;

    SparseGraph(bool undirected = true) noexcept
        : undirected_(undirected)
    {
    }

    void push_node(NodeType n) noexcept
    {
        nodes_.push_back(n);
        neighbor_list_.emplace_back();
    }

    void connect(const std::size_t from, const std::size_t to, const EdgeType edge = EdgeType()) noexcept
    {
        if (undirected_)
        {
            neighbor_list_[from].insert(to);
            neighbor_list_[to].insert(from);
            edges_[std::make_pair(std::min(from, to), std::max(from, to))] = edge;
        }
        else
        {
            neighbor_list_[from].insert(to);
            edges_[std::make_pair(from, to)] = edge;
        }
    }

    void disconnect(const std::size_t from, const std::size_t to) noexcept
    {
        if (undirected_)
        {
            neighbor_list_[from].erase(to);
            neighbor_list_[to].erase(from);
            edges_.erase(std::make_pair(std::min(from, to), std::max(from, to)));
        }
        else
        {
            neighbor_list_[from].erase(to);
            edges_.erase(std::make_pair(from, to));
        }
    }

    const NodeType& node(const std::size_t index) const noexcept
    {
        return nodes_.at(index);
    }

    NodeType& node(const std::size_t index) noexcept
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

    EdgeType& edge(const std::size_t from, const std::size_t to) noexcept
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

    SparseGraph<N, E> decide_root(const int node_index) const noexcept
    {
        SparseGraph<N, E> graph(false);
        for (int i = 0; i < nodes_.size(); i++)
        {
            graph.push_node(node(i));
        }

        std::queue<int> que;
        que.push(node_index);
        vector<bool> visited(size(), false);
        visited[node_index] = false;

        while (!que.empty())
        {
            const int node = que.front();
            que.pop();
            for (auto next : neighbor(node))
            {
                if (!visited[next])
                {
                    graph.connect(node, next, edge(node, next));
                    visited[next] = true;
                    que.push(next);
                }
            }
        }
        return graph;
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

    DenseGraph(bool undirected = true) noexcept
        : undirected_(undirected)
    {
    }

    void push_node(NodeType n) noexcept
    {
        nodes_.push_back(n);
        neighbor_list_.emplace_back();
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
            neighbor_list_[to].push_back(from);
            edges_[std::make_pair(std::min(from, to), std::max(from, to))] = edge;
        }
        else
        {
            neighbor_list_[from].push_back(to);
            edges_[std::make_pair(from, to)] = edge;
        }
    }

    void disconnect(const std::size_t from, const std::size_t to) noexcept
    {
        if (undirected_)
        {
            neighbor_list_[from].erase(std::find(neighbor_list_[from].begin(), neighbor_list_[from].end(), to));
            neighbor_list_[to].erase(std::find(neighbor_list_[to].begin(), neighbor_list_[to].end(), from));
            edges_.erase(std::make_pair(std::min(from, to), std::max(from, to)));
        }
        else
        {
            neighbor_list_[from].erase(std::find(neighbor_list_[from].begin(), neighbor_list_[from].end(), to));
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

template <typename GraphType>
void SaveGraphViz(std::ostream& out, const GraphType& graph)
{
    const double scale = 1;

    out << "graph {" << std::endl;
    out << "  layout=neato;" << std::endl;

    out << "  edge[" << std::endl;
    out << "    color = \"#859900\"," << std::endl;
    out << "    labelfloat = false," << std::endl; // ラベルの重なりを許さない（デフォルト）
    out << "  ];" << std::endl;

    double min_x = 1e100;
    double min_y = 1e100;
    for (int i = 0; i < graph.size(); i++)
    {
        min_x = std::min(min_x, graph.node(i)[0]);
        min_y = std::min(min_y, graph.node(i)[1]);
    }

    for (std::size_t i = 0; i < graph.size(); i++)
    {
        out << "  " << i << " [pos=\"" << ((graph.node(i)[0] - min_x) * scale) << "," << ((graph.node(i)[1] - min_y) * scale) << "!\"];" << std::endl;
    }
    out << "  " << std::endl;
    for (std::size_t i = 0; i < graph.size(); i++)
    {
        for (auto j : graph.neighbor(i))
        {
            if (i < j)
            {
                out << "  " << i << " -- " << j << ";" << std::endl;
            }
        }
    }
    out << "}" << std::endl;
}