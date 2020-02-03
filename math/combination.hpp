template <typename T>
class Combination
{
public:
    Combination(int n);

    T query(int n, int m);

private:
    std::vector<std::vector<T>> table_;
};

template <typename T>
Combination<T>::Combination(int n)
{
    const T InitialValue = T(0);

    table_.resize(n + 1);
    for (auto& row : table_)
    {
        row.resize(n + 1, InitialValue);
    }
}

template <typename T>
T Combination<T>::query(int n, int m)
{
    const T InitialValue(0);

    if (table_[n][m] == InitialValue)
    {
        if (m == 0 || m == n)
        {
            table_[n][m] = 1;
        }
        else
        {
            table_[n][m] = query(n - 1, m - 1) + query(n - 1, m);
        }
    }
    return table_[n][m];
}