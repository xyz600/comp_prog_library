#include "geometry/base.hpp"

Point2D Intersect(const Line2D& l1, const Line2D& l2) noexcept
{
    const double denom = l1.coef[0] * l2.coef[1] - l1.coef[1] * l2.coef[0];
    assert(denom != 0.0);

    const double num0 = -l1.offset * l2.coef[1] + l1.coef[1] * l2.offset;
    const double num1 = l1.offset * l2.coef[0] - l1.coef[0] * l2.offset;
    return Point2D({ num0 / denom, num1 / denom });
}
