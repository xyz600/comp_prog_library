#include "../base.hpp"

#include <gtest/gtest.h>

TEST(graph_access, graph_test)
{
    const std::size_t dim = 10;

    SparseGraph graph(dim);

    ASSERT_EQ(graph.size(), dim);
    ASSERT_TRUE(graph[0].empty());

    graph.connect(0, 1);
    ASSERT_NE(graph[0].find(1), graph[0].end());
    ASSERT_EQ(graph[1].find(0), graph[1].end());

    graph.connect(0, 1, true);
    ASSERT_NE(graph[1].find(0), graph[1].end());
}