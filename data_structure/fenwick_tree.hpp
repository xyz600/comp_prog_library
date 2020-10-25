#include <cassert>
#include <vector>

/**
 * @brief 1-indexed fenwick tree
 * verified @ https://atcoder.jp/contests/abc174/submissions/17648464
 * @tparam T data type
 */
template <typename T>
class FenwickTree
{
public:
    FenwickTree(const std::size_t size)
        : data_(size + 1, 0)
    {
    }

    void add(int i, const T& v)
    {
        assert(i > 0);
        for (; i < size(); i += i & -i)
        {
            data_[i] += v;
        }
    }

    T sum(int i)
    {
        T res(0);
        for (; i > 0; i -= i & -i)
            res += data_[i];
        return res;
    }

    /**
     * @brief
     *
     * @param l range left(inclusive)
     * @param r range right(exclusive)
     * @return T
     */
    T range_sum(int l, int r) { return sum(r - 1) - sum(l - 1); }

    std::size_t size() const { return data_.size(); }

private:
    std::vector<T> data_;
};