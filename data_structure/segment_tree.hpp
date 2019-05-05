#include <algorithm>
#include <cstdint>
#include <limits>
#include <vector>

// verified @ http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_A
template <typename T>
class segtree
{
private:
    std::size_t N;
    std::vector<T> data;

    T query(std::size_t a, std::size_t b, std::size_t k, std::size_t l, std::size_t r)
    {
        if (r <= a || b <= l)
        {
            return std::numeric_limits<T>::max();
        }
        if (a <= l && r <= b)
        {
            return data[k];
        }
        size_t m = (l + r) / 2;
        return std::min(query(a, b, k * 2 + 1, l, m), query(a, b, k * 2 + 2, m, r));
    }

public:
    segtree(const std::size_t n, const T init)
    {
        N = 1;
        while (N < n)
        {
            N *= 2;
        }
        data.resize(N * 2, init);
    }

    void update(std::size_t k, T a)
    {
        k += N - 1;
        data[k] = a;
        while (k > 0)
        {
            k = (k - 1) / 2;
            data[k] = std::min(data[k * 2 + 1], data[k * 2 + 2]);
        }
    }

    T query(std::size_t a, std::size_t b)
    {
        return query(a, b, 0, 0, N);
    }
};