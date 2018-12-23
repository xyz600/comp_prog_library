#include "geometry/base.hpp"
#include "geometry/delaunay_graph.hpp"

#include <gtest/gtest.h>
#include <random>

constexpr double eps = 1e-7;

/**
TEST(delaunay_easy_test, delaunay_graph_test)
{
    std::vector<Point2D> position_list = {
        Point2D({ 0.0, 0.0 }),
        Point2D({ 10.0, 0.0 }),
        Point2D({ 0.0, 10.0 }),
        Point2D({ 10.0, 10.0 }),
        Point2D({ 5.0, 2.5 }),
        Point2D({ 5.0, 7.5 }),
    };

    auto graph = GetDelaunayGraph(position_list);

    ASSERT_EQ(graph.size(), 6);
}
**/

TEST(delaunay_easy_test, delaunay_graph_test)
{
    std::vector<Point2D> position_list;

    std::mt19937 mt;
    std::uniform_real_distribution<double> rand(0, 10);

    std::size_t size = 50;

    for (std::size_t i = 0; i < size; i++)
    {
        Point2D p;
        p[0] = rand(mt);
        p[1] = rand(mt);
        position_list.push_back(p);
    }

    auto graph = GetDelaunayGraph(position_list);

    ASSERT_EQ(graph.size(), size);
}
