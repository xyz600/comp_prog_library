#include <algorithm>
#include <cmath>
#include <cstdint>
#include <vector>

template <typename T>
std::vector<T> eratosthenes(const T upperbound)
{
    vector<bool> is_prime(upperbound + 1, true);

    for (T val = 2; val * val <= upperbound; val++)
    {
        if (is_prime[val])
        {
            for (T nonprime = val * val; nonprime <= upperbound; nonprime += val)
            {
                is_prime[nonprime] = false;
            }
        }
    }
    vector<T> result;
    for (T val = 2; val <= upperbound; val++)
    {
        if (is_prime[val])
        {
            result.push_back(val);
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