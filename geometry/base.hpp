#pragma once

#include <array>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <functional>
#include <initializer_list>

template <class T, std::size_t Dimension>
struct Point
{
public:
    Point() {}

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

    std::size_t size() const noexcept
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

    Point<T, Dimension> Min(const Point<T, Dimension>& p) const noexcept
    {
        return Apply(p, std::min<T>);
    }

    Point<T, Dimension> Max(const Point<T, Dimension>& p) const noexcept
    {
        return Apply(p, std::max<T>);
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
    auto tmp = p1 * p2;
    return tmp.Reduce(std::plus<T>());
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

using Point3D = Point<double, 3>;
using Point2D = Point<double, 2>;

using Point3I = Point<std::int64_t, 3>;
using Point2I = Point<std::int64_t, 2>;
