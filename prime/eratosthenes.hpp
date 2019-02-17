#include <algorithm>
#include <cmath>
#include <cstdint>
#include <vector>

template <typename T>
std::vector<T> eratosthenes(const T upperbound)
{
    vector<bool> is_prime(T + 1, true);
    vector<T> result;

    const T iter_upperbound = std::sqrt(upperbound);
    for (T val = 2; val * val <= upperbound; val++)
    {
        if (is_prime[val])
        {
            result.push_back(val);
            for (T nonprime = val * val; nonprime <= T; nonprime += val)
            {
                is_prime[nonprime] = false;
            }
        }
    }
    return result;
}

template <typename T>
std::vector<T> divisor_list(const T N)
{
    std::vector<T> result;

    for (T val = 2; val * val <= N; val++)
    {
        if (N % val == 0)
        {
            result.push_back(val);
            result.push_back(N / val);
        }
    }
    sort(result.begin(), result.end());

    return result;
}