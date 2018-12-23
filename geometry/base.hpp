#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <functional>
#include <initializer_list>
#include <iostream>

template <class T, std::size_t D>
struct Point
{
public:
    using value_type = T;
    static constexpr std::size_t Dimension()
    {
        return D;
    }

    Point()
    {
        std::fill(array_.begin(), array_.end(), 0);
    }

    Point(std::initializer_list<T>&& list)
    {
        std::size_t index = 0;
        for (auto it = list.begin(); index < Dimension() && it != list.end(); it++)
        {
            array_[index] = *it;
            index++;
        }
        for (std::size_t i = index; i < Dimension(); i++)
        {
            array_[i] = 0;
        }
    }

    Point(const Point& src)
    {
        for (std::size_t i = 0; i < std::min(src.size(), Dimension()); i++)
        {
            array_[i] = src[i];
        }
    }

    constexpr std::size_t size() const noexcept
    {
        return Dimension();
    }

    T& operator[](std::size_t index) noexcept { return array_[index]; }
    const T& operator[](std::size_t index) const noexcept { return array_[index]; }

    T& x() noexcept { return array_[0]; }
    const T& x() const noexcept { return array_[0]; };

    T& y() noexcept { return array_[1]; }
    const T& y() const noexcept { return array_[1]; }

    T& z() noexcept { return array_[2]; };
    const T& z() const noexcept { return array_[2]; };

    Point<T, Dimension()> operator+(const Point<T, Dimension()>& p) const noexcept
    {
        return Apply(p, std::plus<T>());
    }

    Point<T, Dimension()> operator+(const T& val) const noexcept
    {
        return Apply(val, std::plus<T>());
    }

    Point<T, Dimension()> operator-(const Point<T, Dimension()>& p) const noexcept
    {
        return Apply(p, std::minus<T>());
    }

    Point<T, Dimension()> operator-(const T& val) const noexcept
    {
        return Apply(val, std::minus<T>());
    }

    Point<T, Dimension()> operator*(const Point<T, Dimension()>& p) const noexcept
    {
        return Apply(p, std::multiplies<T>());
    }

    Point<T, Dimension()> operator*(const T& val) const noexcept
    {
        return Apply(val, std::multiplies<T>());
    }

    Point<T, Dimension()> operator/(const Point<T, Dimension()>& p) const noexcept
    {
        return Apply(p, std::divides<T>());
    }

    Point<T, Dimension()> operator/(const T& val) const noexcept
    {
        return Apply(val, std::divides<T>());
    }

    Point<T, Dimension()> Min(const Point<T, Dimension()>& p) const noexcept
    {
        return Apply(p, [](const T val1, const T val2) {
            return std::min(val1, val2);
        });
    }

    Point<T, Dimension()> Max(const Point<T, Dimension()>& p) const noexcept
    {
        return Apply(p, [](const T val1, const T val2) {
            return std::max(val1, val2);
        });
    }

    T Norm2() const noexcept
    {
        T ret = 0;
        for (auto v : array_)
        {
            ret += v * v;
        }
        return ret;
    }

    Point<T, Dimension()>&& Normalize() noexcept
    {
        const T norm = std::sqrt(Norm2());
        for (auto& v : array_)
        {
            v /= norm;
        }
        return std::move(*this);
    }

    template <class BinaryFunction>
    Point<T, Dimension()> Apply(const Point<T, Dimension()>& p, BinaryFunction func) const noexcept
    {
        Point<T, Dimension()> ret;
        for (std::size_t i = 0; i < size(); i++)
        {
            ret[i] = func((*this)[i], p[i]);
        }
        return ret;
    }

    template <class BinaryFunction>
    Point<T, Dimension()> Apply(const T& val, BinaryFunction func) const noexcept
    {
        Point<T, Dimension()> ret;
        for (std::size_t i = 0; i < size(); i++)
        {
            ret[i] = func((*this)[i], val);
        }
        return ret;
    }

    template <class UnaryFunction>
    Point<T, Dimension()>&& Map(UnaryFunction func) noexcept
    {
        for (std::size_t i = 1; i < Dimension(); i++)
        {
            array_[i] = func(array_[i]);
        }
        return std::move(*this);
    }

    template <class BinaryFunction>
    T Reduce(BinaryFunction func) const noexcept
    {
        T ret(array_[0]);
        for (std::size_t i = 1; i < Dimension(); i++)
        {
            ret = func(ret, array_[i]);
        }
        return ret;
    }

private:
    std::array<T, Dimension()> array_;
};

template <typename T, std::size_t Dimension>
T dot(const Point<T, Dimension>& p1, const Point<T, Dimension>& p2) noexcept
{
    T ret = 0;
    for (std::size_t i = 0; i < Dimension; i++)
    {
        ret += p1[i] * p2[i];
    }
    return ret;
}

template <typename T>
Point<T, 3> cross(const Point<T, 3>& p1, const Point<T, 3>& p2) noexcept
{
    return {
        p1.y() * p2.z() - p1.z() * p2.y(),
        p1.z() * p2.x() - p1.x() * p2.z(),
        p1.x() * p2.y() - p1.y() * p2.x()
    };
}

template <typename Container, typename BinaryFunction>
Point<typename Container::value_type::value_type, Container::value_type::Dimension()>
Reduce(const Container& container, BinaryFunction func)
{
    using Iterator = typename Container::const_iterator;
    using T = typename Container::value_type::value_type;
    constexpr std::size_t Dimension = Container::value_type::Dimension();

    Point<T, Dimension> ret = *container.begin();
    for (Iterator it = container.begin() + 1; it != container.end(); it++)
    {
        ret = func(ret, *it);
    }
    return ret;
}

template <typename Container>
Point<typename Container::value_type::value_type, Container::value_type::Dimension()> ReduceMin(const Container& container)
{
    using T = typename Container::value_type::value_type;
    constexpr std::size_t Dimension = Container::value_type::Dimension();

    return Reduce(container, [](const Point<T, Dimension>& p1, const Point<T, Dimension>& p2) {
        return p1.Min(p2);
    });
}

template <typename Container>
Point<typename Container::value_type::value_type, Container::value_type::Dimension()> ReduceMax(const Container& container)
{
    using T = typename Container::value_type::value_type;
    constexpr std::size_t Dimension = Container::value_type::Dimension();

    return Reduce(container, [](const Point<T, Dimension>& p1, const Point<T, Dimension>& p2) {
        return p1.Max(p2);
    });
}

template <typename T, std::size_t Dimension>
std::ostream& operator<<(std::ostream& out, const Point<T, Dimension>& p)
{
    out << "(";
    for (std::size_t i = 0; i < p.size(); i++)
    {
        if (i > 0)
        {
            out << ", ";
        }
        out << p[i];
    }
    out << ")";
    return out;
}

using Point3D = Point<double, 3>;
using Point2D = Point<double, 2>;

using Point3I = Point<std::int64_t, 3>;
using Point2I = Point<std::int64_t, 2>;

template <typename T, std::size_t Dimension>
struct Line
{
public:
    using PointType = Point<T, Dimension>;

    Line() noexcept
    {
    }

    Line(const PointType& from, const PointType& to) noexcept
    {
        for (std::size_t i = 1; i < Dimension; i += 2)
        {
            coef[i - 1] = to[i] - from[i];
            coef[i] = from[i - 1] - to[i - 1];
        }
        if (Dimension % 2 == 1)
        {
            coef[Dimension - 1] = 0;
        }
        offset = -dot(coef, from);
    }

    PointType coef;
    T offset;
};

template <class T, std::size_t Dimension>
std::ostream& operator<<(std::ostream& out, const Line<T, Dimension>& line)
{
    out << "Line:[" << line.coef << ", " << line.offset << "]";
    return out;
}

using Line3D = Line<double, 3>;
using Line2D = Line<double, 2>;

Point2D Intersect(const Line2D& l1, const Line2D& l2) noexcept;

template <class T, std::size_t Dimension>
class Circle
{
public:
    using PointType = Point<T, Dimension>;

    Circle(const PointType& center, const T radius) noexcept
        : center_(center)
        , radius_(radius)
    {
    }

    const PointType& Center() const noexcept
    {
        return center_;
    }

    const T Radius() const noexcept
    {
        return radius_;
    }

    bool Contain(const PointType& p) const noexcept
    {
        return (center_ - p).Norm2() < radius_ * radius_;
    }

private:
    PointType center_;
    T radius_;
};

using Circle2D = Circle<double, 2>;
using Circle3D = Circle<double, 3>;

template <class T, std::size_t Dimension>
class Triangle
{
public:
    using PointType = Point<T, Dimension>;

    Triangle(const PointType& p1, const PointType& p2, const PointType& p3) noexcept
    {
        point_array_[0] = p1;
        point_array_[1] = p2;
        point_array_[2] = p3;
    }

    static bool Contain(const PointType& p1, const PointType& p2, const PointType& p3, const PointType& p) noexcept
    {
        const PointType pb = p - p1;

        const auto dir1_ = p2 - p1;
        const auto dir2_ = p3 - p1;

        const auto a = dot(pb, dir1_);
        const auto b = dot(dir1_, dir1_);
        const auto c = dot(dir1_, dir2_);
        const auto d = dot(pb, dir2_);
        const auto e = dot(dir2_, dir2_);

        const auto denom = b * e - c * c;
        const auto c1 = (a * e - c * d) / denom;
        const auto c2 = -(a * c - b * d) / denom;

        constexpr double eps = 1e-7;

        return -eps <= c1 && -eps <= c2 && c1 + c2 <= 1.0 + eps;
    }

    bool Contain(const PointType& p) const noexcept
    {
        return Contain(at(0), at(1), at(2), p);
    }

    static Circle<T, Dimension> GetCircumscribedCircle(const PointType& p1, const PointType& p2, const PointType& p3) noexcept
    {
        const auto dir1_ = p2 - p1;
        const auto dir2_ = p3 - p1;

        const auto a = dot(dir1_, dir1_);
        const auto b = dot(dir1_, dir2_);
        const auto c = dot(dir2_, dir2_);

        const auto s = c * (b - a) / (2 * (b * b - a * c));
        const auto t = a * (b - c) / (2 * (b * b - a * c));

        const auto relo = (dir1_ * s) + (dir2_ * t);
        const auto o = p1 + relo;
        const auto r = sqrt(dot(relo, relo));

        return Circle<T, Dimension>(o, r);
    }

    Circle<T, Dimension> GetCircumscribedCircle() const noexcept
    {
        return GetCircumscribedCircle(at(0), at(1), at(2));
    }

    const PointType& operator[](const std::size_t index) const noexcept
    {
        return point_array_[index];
    }

    PointType& operator[](const std::size_t index) noexcept
    {
        return point_array_[index];
    }

    const PointType& at(const std::size_t index) const noexcept
    {
        return point_array_[index];
    }

    PointType& at(const std::size_t index) noexcept
    {
        return point_array_[index];
    }

private:
    std::array<PointType, 3> point_array_;
};

using Triangle3D = Triangle<double, 3>;
using Triangle2D = Triangle<double, 2>;