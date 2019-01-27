#include <cstdint>
#include <ctime>

// clock -> sec は、CLOCKS_PER_SEC
uint64_t getCycle()
{
    uint32_t low, high;
    __asm__ volatile("rdtsc"
                     : "=a"(low), "=d"(high));
    return ((uint64_t)low) | ((uint64_t)high << 32);
}