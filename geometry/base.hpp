#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <functional>
#include <initializer_list>
#include <iostream>

template <class T, std::size_t Dimension>
struct Point
{
public:
    Point()
    {
        std::fill(array_.begin(), array_.end(), 0);
    }

    Point(std::initializer_list<T>&& list)
    {
        std::size_t index = 0;
        for (auto it = list.begin(); index < Dimension && it != list.end(); it++)
        {
            array_[index] = *it;
            index++;
        }
        for (std::size_t i = index; i < Dimension; i++)
        {
            array_[i] = 0;
        }
    }

    Point(const Point& src)
    {
        for (std::size_t i = 0; i < std::min(src.size(), Dimension); i++)
        {
            array_[i] = src[i];
        }
    }

    constexpr std::size_t size() const noexcept
    {
        return Dimension;
    }

    T& operator[](std::size_t index) noexcept { return array_[index]; }
    const T& operator[](std::size_t index) const noexcept { return array_[index]; }

    T& x() noexcept { return array_[0]; }
    const T& x() const noexcept { return array_[0]; };

    T& y() noexcept { return array_[1]; }
    const T& y() const noexcept { return array_[1]; }

    T& z() noexcept { return array_[2]; };
    const T& z() const noexcept { return array_[2]; };

    Point<T, Dimension> operator+(const Point<T, Dimension>& p) const noexcept
    {
        return Apply(p, std::plus<T>());
    }

    Point<T, Dimension> operator+(const T& val) const noexcept
    {
        return Apply(val, std::plus<T>());
    }

    Point<T, Dimension> operator-(const Point<T, Dimension>& p) const noexcept
    {
        return Apply(p, std::minus<T>());
    }

    Point<T, Dimension> operator-(const T& val) const noexcept
    {
        return Apply(val, std::minus<T>());
    }

    Point<T, Dimension> operator*(const Point<T, Dimension>& p) const noexcept
    {
        return Apply(p, std::multiplies<T>());
    }

    Point<T, Dimension> operator*(const T& val) const noexcept
    {
        return Apply(val, std::multiplies<T>());
    }

    Point<T, Dimension> operator/(const Point<T, Dimension>& p) const noexcept
    {
        return Apply(p, std::divides<T>());
    }

    Point<T, Dimension> operator/(const T& val) const noexcept
    {
        return Apply(val, std::divides<T>());
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

    Point<T, Dimension>&& Normalize() noexcept
    {
        const T norm = std::sqrt(Norm2());
        for (auto& v : array_)
        {
            v /= norm;
        }
        return std::move(*this);
    }

    template <class BinaryFunction>
    Point<T, Dimension> Apply(const Point<T, Dimension>& p, BinaryFunction func) const noexcept
    {
        Point<T, Dimension> ret;
        for (std::size_t i = 0; i < size(); i++)
        {
            ret[i] = func((*this)[i], p[i]);
        }
        return ret;
    }

    template <class BinaryFunction>
    Point<T, Dimension> Apply(const T& val, BinaryFunction func) const noexcept
    {
        Point<T, Dimension> ret;
        for (std::size_t i = 0; i < size(); i++)
        {
            ret[i] = func((*this)[i], val);
        }
        return ret;
    }

    template <class UnaryFunction>
    Point<T, Dimension>&& Map(UnaryFunction func) noexcept
    {
        for (std::size_t i = 1; i < Dimension; i++)
        {
            array_[i] = func(array_[i]);
        }
        return std::move(*this);
    }

    template <class BinaryFunction>
    T Reduce(BinaryFunction func) const noexcept
    {
        T ret(array_[0]);
        for (std::size_t i = 1; i < Dimension; i++)
        {
            ret = func(ret, array_[i]);
        }
        return ret;
    }

private:
    std::array<T, Dimension> array_;
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
    Line() noexcept
    {
    }

    Line(const Point<T, Dimension>& from, const Point<T, Dimension>& to) noexcept
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

    Point<T, Dimension> coef;
    T offset;

    T Apply(const Point<T, Dimension>& input) const noexcept
    {
        return dot(coef, input) + offset;
    }
};

template <class T, std::size_t Dimension>
std::ostream& operator<<(std::ostream& out, const Line<T, Dimension>& line)
{
    out << "Line:[" << line.coef << ", " << line.offset << "]";
    return out;
}

using Line3D = Line<double, 3>;
using Line2D = Line<double, 2>;

Point2D Intersect(const Line2D& l1, const Line2D& l2)
{
    const double denom = l1.coef[0] * l2.coef[1] - l1.coef[1] * l2.coef[0];
    assert(denom != 0.0);

    const double num0 = -l1.offset * l2.coef[1] + l1.coef[1] * l2.offset;
    const double num1 = l1.offset * l2.coef[0] - l1.coef[0] * l2.offset;
    return Point2D({ num0 / denom, num1 / denom });
}