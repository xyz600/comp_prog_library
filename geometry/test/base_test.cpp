#include "base.hpp"

#include <gtest/gtest.h>
#include <sstream>

// test Point

constexpr double eps = 1e-7;

TEST(point_test, geometry_base_test)
{
    Point3D p1({ 1.0, 2.0, 4.0 });

    ASSERT_EQ(p1.size(), 3);
    ASSERT_EQ(p1[0], 1.0);
    ASSERT_EQ(p1[1], 2.0);
    ASSERT_EQ(p1[2], 4.0);
}

TEST(point_operator_plus, geometry_base_test)
{
    Point3D p1({ 1.0, 2.0, 4.0 });
    Point3D p2({ 8.0, 16.0, 32.0 });

    auto p3 = p1 + p2;

    ASSERT_EQ(p3[0], 9.0);
    ASSERT_EQ(p3[1], 18.0);
    ASSERT_EQ(p3[2], 36.0);

    auto p4 = p1 + 1.0;

    ASSERT_EQ(p4[0], 2.0);
    ASSERT_EQ(p4[1], 3.0);
    ASSERT_EQ(p4[2], 5.0);
}

TEST(point_operator_minus, geometry_base_test)
{
    Point3D p1 = { 1.0, 2.0, 4.0 };
    Point3D p2 = { 8.0, 16.0, 32.0 };

    auto p3 = p1 - p2;

    ASSERT_EQ(p3[0], -7.0);
    ASSERT_EQ(p3[1], -14.0);
    ASSERT_EQ(p3[2], -28.0);

    auto p4 = p1 - 1.0;

    ASSERT_EQ(p4[0], 0.0);
    ASSERT_EQ(p4[1], 1.0);
    ASSERT_EQ(p4[2], 3.0);
}

TEST(point_operator_mult, geometry_base_test)
{
    Point3D p1 = { 1.0, 2.0, 4.0 };
    Point3D p2 = { 8.0, 16.0, 32.0 };

    auto p3 = p1 * p2;

    ASSERT_EQ(p3[0], 8.0);
    ASSERT_EQ(p3[1], 32.0);
    ASSERT_EQ(p3[2], 128.0);

    auto p4 = p1 * 2.0;

    ASSERT_EQ(p4[0], 2.0);
    ASSERT_EQ(p4[1], 4.0);
    ASSERT_EQ(p4[2], 8.0);
}

TEST(point_operator_div, geometry_base_test)
{
    Point3D p1 = { 1.0, 2.0, 4.0 };
    Point3D p2 = { 8.0, 16.0, 32.0 };

    auto p3 = p1 / p2;

    ASSERT_EQ(p3[0], 0.125);
    ASSERT_EQ(p3[1], 0.125);
    ASSERT_EQ(p3[2], 0.125);

    auto p4 = p1 / 2.0;

    ASSERT_EQ(p4[0], 0.5);
    ASSERT_EQ(p4[1], 1.0);
    ASSERT_EQ(p4[2], 2.0);
}

TEST(point_dot, geometry_base_test)
{
    Point3D p1 = { 1.0, 2.0, 4.0 };
    Point3D p2 = { 1.0, 2.0, 4.0 };

    ASSERT_EQ(dot(p1, p2), 21.0);
}

TEST(point_cross, geometry_base_test)
{
    Point3D p1 = { 1.0, 2.0, 4.0 };
    Point3D p2 = { 4.0, 3.0, 2.0 };

    auto p3 = cross(p1, p2);

    ASSERT_EQ(p3[0], -8.0);
    ASSERT_EQ(p3[1], 14.0);
    ASSERT_EQ(p3[2], -5.0);
}

TEST(point_normalize, geometry_base_test)
{
    Point3D p({ 1.0, 2.0, 2.0 });

    ASSERT_EQ(p.Norm2(), 9.0);

    auto p2 = Point3D({ 1.0, 2.0, 2.0 }).Normalize();

    ASSERT_TRUE(abs(1.0 - p2.Norm2()) < eps);
    ASSERT_TRUE(abs(1.0 / 3.0 - p2[0]) < eps);
    ASSERT_TRUE(abs(2.0 / 3.0 - p2[1]) < eps);
    ASSERT_TRUE(abs(2.0 / 3.0 - p2[2]) < eps);
}

TEST(point_ostream, geometry_base_test)
{
    std::stringstream ss;

    Point3I p({ 1, 2, 4 });
    ss << p;
    ASSERT_EQ(ss.str(), "(1, 2, 4)");
}

// test Line

TEST(line_constructor, geometry_base_test)
{
    const Point2D p1({ 0.0, 1.0 });
    const Point2D p2({ 2.0, 5.0 });

    const Point2D coef = p2 - p1;
    Line2D line(p1, p2);

    ASSERT_TRUE(abs(dot(coef, line.coef)) < eps);
}

TEST(line_intersect, geometry_base_test)
{
    const Line2D line1(Point2D({ 10.0, 0.0 }), Point2D({ 0.0, 10.0 }));
    const Line2D line2(Point2D({ 0.0, 0.0 }), Point2D({ 10.0, 10.0 }));

    auto inter = Intersect(line1, line2);
    ASSERT_EQ(inter[0], 5.0);
    ASSERT_EQ(inter[1], 5.0);
}