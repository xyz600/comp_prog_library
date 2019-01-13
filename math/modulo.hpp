#include <cstdint>

template <std::size_t MOD>
class ModuloInteger
{
public:
    ModuloInteger(const std::size_t init)
        : value(init)
    {
    }

    ModuloInteger operator+(const ModuloInteger& value);
    void operator+=(const ModuloInteger& value);

    ModuloInteger operator-(const ModuloInteger& value);
    void operator-=(const ModuloInteger& value);

    ModuloInteger operator*(const ModuloInteger& value);
    void operator*=(const ModuloInteger& value);

    std::size_t value;
};

template <std::size_t MOD>
ModuloInteger<MOD> ModuloInteger<MOD>::operator+(const ModuloInteger& value)
{
    return (this->value + value.value) % MOD;
}

template <std::size_t MOD>
void ModuloInteger<MOD>::operator+=(const ModuloInteger& value)
{
    this->value = (this->value + value.value) % MOD;
}

template <std::size_t MOD>
ModuloInteger<MOD> ModuloInteger<MOD>::operator-(const ModuloInteger& value)
{
    return (this->value + MOD - value.value) % MOD;
}

template <std::size_t MOD>
void ModuloInteger<MOD>::operator-=(const ModuloInteger& value)
{
    this->value = (this->value - value.value) % MOD;
}

template <std::size_t MOD>
ModuloInteger<MOD> ModuloInteger<MOD>::operator*(const ModuloInteger& value)
{
    return (this->value * value.value) % MOD;
}

template <std::size_t MOD>
void ModuloInteger<MOD>::operator*=(const ModuloInteger& value)
{
    this->value = (this->value * value.value) % MOD;
}