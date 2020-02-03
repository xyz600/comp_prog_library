#include <cstdint>
#include <ctime>

// clock -> sec は、CLOCKS_PER_SEC
int64_t getCycle()
{
    uint32_t low, high;
    __asm__ volatile("rdtsc"
                     : "=a"(low), "=d"(high));
    return ((int64_t)low) | ((int64_t)high << 32);
}

int64_t getMillisecTime(const int64_t start_clock)
{
    return (getCycle() - start_clock) * 1000ull / CLOCKS_PER_SEC;
}

int64_t getMicrosecTime(const int64_t start_clock)
{
    return (getCycle() - start_clock) * 1000000ull / CLOCKS_PER_SEC;
}