#include "base.hpp"

#include <algorithm>
#include <cstdint>
#include <map>
#include <queue>
#include <unordered_set>
#include <vector>
#include <limits>

template<typename NodeType>
class Dinic
{
public:
    int doIt(const SparseGraph<NodeType, std::size_t> &orig_graph, const int s, const int t)
    {
        SparseGraph<NodeType, std::size_t> graph(orig_graph);
        visited_.resize(graph.size(), false);
        level_.resize(graph.size(), -1);

        // 逆辺を貼る
        for (std::size_t i = 0; i < orig_graph.size(); i++)
        {
            for (auto nv : orig_graph.neighbor(i))
            {
                graph.connect(nv, i, 0);
            }
        }
        
        std::size_t flow = 0;
        while (true)
        {
            bfs(graph, s);
            if (level_[t] == -1)
            {
                return flow;
            }
            
            while (true)
            {
                fill(visited_.begin(), visited_.end(), false);
                const auto sub_flow = dfs(graph, s, t, std::numeric_limits<std::size_t>::max());
                if (sub_flow == 0)
                {
                    break;
                }
                flow += sub_flow;
            }
        }
        return flow;
    }
private:
    
    std::vector<bool> visited_;
    std::vector<std::int64_t> level_;
    
    void bfs(const SparseGraph<NodeType, std::size_t> &graph, const int s)
    {
        fill(level_.begin(), level_.end(), -1);
        std::queue<int> que;
        que.push(s);
        level_[s] = 0;
        
        while (!que.empty())
        {
            const auto v = que.front();
            que.pop();
            for (const auto nv : graph.neighbor(v))
            {
                if (graph.edge(v, nv) > 0 && level_[nv] == -1)
                {
                    level_[nv] = level_[v] + 1;
                    que.push(nv);
                }
            }
        }
    }
    
    std::size_t dfs(SparseGraph<NodeType, std::size_t> &graph, const int s, const int t, std::size_t flow)
    {
        if (s == t)
        {
            return flow;
        }
        visited_[s] = true;
        for (auto nv : graph.neighbor(s))
        {
            const auto &e = graph.edge(s, nv);
            if (e > 0 && !visited_[nv] && level_[s] < level_[nv])
            {
                const auto ret = dfs(graph, nv, t, std::min(e, flow));
                if (ret > 0)
                {
                    // 容量の調整
                    graph.edge(s, nv) -= ret;
                    graph.edge(nv, s) += ret;
                    return ret;
                }
            }
        }
        return 0;
    }
};