#include "../base.hpp"

#include <gtest/gtest.h>

TEST(graph_access, graph_test)
{
    const std::size_t dim = 10;

    SparseGraph<int, int> graph;
    for (int i = 0; i < dim; i++)
    {
        graph.push_node(i);
    }

    ASSERT_EQ(graph.size(), dim);
    ASSERT_TRUE(graph.neighbor(0).empty());
}