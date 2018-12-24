#include "geometry/base.hpp"
#include "graph/base.hpp"

#include <array>
#include <fstream>
#include <limits>
#include <map>
#include <random>
#include <set>
#include <stack>
#include <tuple>
#include <vector>

struct IndexTriangle
{
public:
    IndexTriangle(const std::size_t i1, const std::size_t i2, const std::size_t i3)
    {
        array_[0] = i1;
        array_[1] = i2;
        array_[2] = i3;
    }

    std::size_t
    operator[](const std::size_t index) const noexcept
    {
        return array_[index];
    }

    bool Contain(const Point2D& point, const std::vector<Point2D>& position_list, const double eps) const noexcept
    {
        return Triangle2D::Contain(position_list[array_[0]], position_list[array_[1]], position_list[array_[2]], point, eps);
    }

    Circle2D GetCircumscribedCircle(const std::vector<Point2D>& position_list) const noexcept
    {
        return Triangle2D::GetCircumscribedCircle(position_list[array_[0]], position_list[array_[1]], position_list[array_[2]]);
    }

private:
    std::array<std::size_t, 3> array_;
};

class HistoryGraph
{
public:
    static constexpr std::size_t None()
    {
        return std::numeric_limits<std::size_t>::max();
    }

    HistoryGraph(std::vector<Point2D>&& position_list) noexcept
        : position_list_(position_list)
        , history_(false)
    {
    }

    const IndexTriangle& operator[](const std::size_t index) const noexcept
    {
        return history_.node(index);
    }

    std::size_t push(IndexTriangle&& another) noexcept
    {
        history_.push_node(another);

        tri2index_[std::make_tuple(another[0], another[1], another[2])] = history_.size() - 1;
        tri2index_[std::make_tuple(another[0], another[2], another[1])] = history_.size() - 1;
        tri2index_[std::make_tuple(another[1], another[0], another[2])] = history_.size() - 1;
        tri2index_[std::make_tuple(another[1], another[2], another[0])] = history_.size() - 1;
        tri2index_[std::make_tuple(another[2], another[0], another[1])] = history_.size() - 1;
        tri2index_[std::make_tuple(another[2], another[1], another[0])] = history_.size() - 1;

        return history_.size() - 1;
    }

    void connect(std::size_t from, std::size_t to) noexcept
    {
        history_.connect(from, to);
    }

    const Point2D& Pos(const std::size_t idx) const noexcept
    {
        return position_list_[idx];
    }

    std::size_t GetContainedTriangle(const Point2D& p)
    {
        std::size_t index = 0;
        constexpr double init_eps = 1e-7;
        double eps = init_eps;

        while (!is_leaf(index))
        {
            bool success = false;
            const int prev_index = index;
            for (auto next_index : history_.neighbor(index))
            {
                if (history_.node(next_index).Contain(p, position_list_, eps))
                {
                    index = next_index;
                    success = true;
                    break;
                }
            }
            assert(prev_index != index);
            if (!success)
            {
                eps *= 2;
            }
            else
            {
                eps = init_eps;
            }
        }
        return index;
    }

    const std::size_t FindTriangle(std::size_t i1, std::size_t i2, std::size_t i3) const
    {
        auto key = std::make_tuple(i1, i2, i3);
        if (tri2index_.find(key) == tri2index_.end())
        {
            return None();
        }
        else
        {
            const auto idx = tri2index_.at(key);
            return is_leaf(idx) ? idx : None();
        }
    }

    bool is_leaf(const std::size_t index) const noexcept
    {
        return history_.neighbor(index).empty();
    }

private:
    std::vector<Point2D> position_list_;

    SparseGraph<IndexTriangle> history_;

    std::map<std::tuple<std::size_t, std::size_t, std::size_t>, std::size_t> tri2index_;
};

bool is_convex(std::vector<Point2D>& points)
{
    const std::size_t size = points.size();
    points.push_back(points[0]);
    points.push_back(points[1]);
    std::vector<bool> vs(size);
    for (int i = 0; i < size; i++)
    {
        auto d1 = points[i + 1] - points[i];
        auto d2 = points[i + 2] - points[i + 1];
        vs[i] = d1.x() * d2.y() - d1.y() * d2.x() > 0;
    }
    for (auto b : vs)
    {
        if (b != vs[0])
        {
            return false;
        }
    }
    return true;
}

template <typename T, typename U>
std::vector<std::size_t> GetCommonVertex(const SparseGraph<T, U>& graph, const HistoryGraph& history, const std::size_t i1, const std::size_t i2, const std::size_t prohibited)
{
    std::vector<std::size_t> ret;

    auto lst = graph.neighbor(i2);
    for (std::size_t n : graph.neighbor(i1))
    {
        if (n != i2 && n != prohibited && history.FindTriangle(n, i1, i2) != HistoryGraph::None())
        {
            std::vector<Point2D> pos = { graph.node(n), graph.node(i1), graph.node(prohibited), graph.node(i2) };
            if (is_convex(pos))
            {
                ret.push_back(n);
            }
        }
    }
    return ret;
}

void save(int turn, const SparseGraph<Point2D, int>& graph)
{
    std::string file_path = "result_" + std::to_string(turn) + ".dot";
    std::ofstream fout(file_path);
    SaveGraphViz(fout, graph);
    fout.close();
}

bool CheckInternal(const Point2D& p1, const Point2D& p2, const Point2D& p3, const Point2D& p4)
{
    return Triangle2D::GetCircumscribedCircle(p1, p2, p3).Contain(p4) || Triangle2D::GetCircumscribedCircle(p4, p2, p3).Contain(p1);
}

SparseGraph<int, int> GetDelaunayGraph(const std::vector<Point2D>& position_list)
{
    constexpr double eps = 1e-6;

    const Point2D Mins = ReduceMin(position_list);
    const Point2D Maxs = ReduceMax(position_list);
    const double dx = Maxs.x() - Mins.x();
    const double dy = Maxs.y() - Mins.y();

    // 巨大な三角形 T0 を作る
    std::vector<Point2D> init_list = {
        Mins + Point2D({ -dx - eps, 0.0 }),
        Mins + Point2D({ 2.0 * dx + eps, 0.0 }),
        Mins + Point2D({ dx / 2.0, dy * 2.0 + eps })
    };

    init_list.reserve(position_list.size() + init_list.size());
    for (auto v : position_list)
    {
        init_list.push_back(v);
    }

    SparseGraph<Point2D, int> graph;

    for (int i = 0; i < 3; i++)
    {
        graph.push_node(init_list[i]);
    }
    graph.connect(0, 1);
    graph.connect(1, 2);
    graph.connect(2, 0);

    HistoryGraph history(std::move(init_list));
    history.push(IndexTriangle(0, 1, 2));

    for (std::size_t id = 3; id < position_list.size() + 3; id++)
    {
        graph.push_node(position_list[id - 3]);

        const auto& pos = position_list[id - 3];

        // HistoryGraph から、D が属する三角形 ABC を取得
        const std::size_t before_tri_id = history.GetContainedTriangle(pos);
        const auto tri = history[before_tri_id];

        // ABC を、ABD, BCD, CAD に分割
        std::vector<IndexTriangle> divided = {
            IndexTriangle(tri[0], tri[1], id),
            IndexTriangle(tri[1], tri[2], id),
            IndexTriangle(tri[2], tri[0], id)
        };

        std::stack<std::pair<std::size_t, std::size_t>> flip_edge_stack;

        // HistoryGraph [ABC -> (ABD, BCD, CAD)] を登録
        for (auto&& t : divided)
        {
            flip_edge_stack.emplace(t[0], t[1]);
            auto tri_id = history.push(std::move(t));
            history.connect(before_tri_id, tri_id);
            assert(!history.is_leaf(before_tri_id));

            graph.connect(id, tri[0]);
            graph.connect(id, tri[1]);
            graph.connect(id, tri[2]);
        }

        while (!flip_edge_stack.empty())
        {
            std::size_t from, to;
            std::tie(from, to) = flip_edge_stack.top();
            flip_edge_stack.pop();

            auto candidate = GetCommonVertex(graph, history, from, to, id);
            if (!candidate.empty())
            {
                const auto np = candidate[0];
                if (CheckInternal(graph.node(id), graph.node(from), graph.node(to), graph.node(np)))
                {
                    graph.disconnect(from, to);
                    graph.connect(id, np);

                    std::vector<IndexTriangle> divided = { IndexTriangle(np, from, id), IndexTriangle(np, to, id) };
                    std::vector<std::size_t> old_tris = { history.FindTriangle(from, to, id), history.FindTriangle(from, to, np) };

                    for (auto&& tri : divided)
                    {
                        flip_edge_stack.emplace(tri[0], tri[1]);

                        auto tri_id = history.push(std::move(tri));
                        for (auto&& old_id : old_tris)
                        {
                            history.connect(old_id, tri_id);
                        }
                    }
                }
            }
        }
    }

    SparseGraph<int, int> ret_graph;
    const std::size_t size = position_list.size();

    for (int i = 0; i < size; i++)
    {
        ret_graph.push_node(i);
    }

    for (int i = 0; i < size; i++)
    {
        for (int j : graph.neighbor(i + 3))
        {
            if (3 <= j)
            {
                ret_graph.connect(i, j - 3);
            }
        }
    }

    return ret_graph;
}