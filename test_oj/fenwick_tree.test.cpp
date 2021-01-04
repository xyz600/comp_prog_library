#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/courses/library/3/DSL/all/DSL_2_B"

#include "../data_structure/fenwick_tree.hpp"

#include <cstdint>
#include <iostream>

using namespace std;

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    size_t n, q;
    cin >> n >> q;

    FenwickTree<int64_t> tree(n);
    for (size_t i = 0; i < q; i++)
    {
        size_t com, x, y;
        cin >> com >> x >> y;
        if (com == 0)
        {
            tree.add(x, y);
        }
        else
        {
            cout << tree.range_sum(x, y + 1) << endl;
        }
    }
}