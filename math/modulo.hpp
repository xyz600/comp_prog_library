#include <cstdint>

template <std::size_t MOD>
class ModuloInteger
{
public:
    ModuloInteger(const std::size_t init)
        : value(init)
    {
    }

    using value_t = std::size_t;

    ModuloInteger operator+(const ModuloInteger value) const noexcept;
    void operator+=(const ModuloInteger value) noexcept;

    ModuloInteger operator-(const ModuloInteger value) const noexcept;
    void operator-=(const ModuloInteger value) noexcept;

    ModuloInteger operator*(const ModuloInteger value) const noexcept;
    void operator*=(const ModuloInteger value) noexcept;

    ModuloInteger operator/(const ModuloInteger value) const noexcept;
    void operator/=(const ModuloInteger value) noexcept;

    ModuloInteger operator+(const std::size_t value) const noexcept;
    void operator+=(const std::size_t value) noexcept;

    ModuloInteger operator-(const std::size_t value) const noexcept;
    void operator-=(const std::size_t value) noexcept;

    ModuloInteger operator*(const std::size_t value) const noexcept;
    void operator*=(const std::size_t value) noexcept;

    ModuloInteger operator/(const std::size_t value) const noexcept;
    void operator/=(const std::size_t value) noexcept;

    ModuloInteger inv() const noexcept;

    ModuloInteger powi(const std::size_t index) const noexcept;

    std::size_t value;
};

template <std::size_t MOD>
ModuloInteger<MOD> ModuloInteger<MOD>::powi(const std::size_t index) const noexcept
{
    if (index == 0)
    {
        return 1;
    }
    else if (index == 1)
    {
        return *this;
    }
    else if (index % 2 == 0)
    {
        auto v = powi(index / 2);
        return v * v;
    }
    else
    {
        return powi(index - 1) * (*this);
    }
}

template <std::size_t MOD>
ModuloInteger<MOD> ModuloInteger<MOD>::inv() const noexcept
{
    return powi(MOD - 2);
}

template <std::size_t MOD>
ModuloInteger<MOD> ModuloInteger<MOD>::operator+(const ModuloInteger value) const noexcept
{
    return (this->value + value.value) % MOD;
}

template <std::size_t MOD>
void ModuloInteger<MOD>::operator+=(const ModuloInteger value) noexcept
{
    this->value = (this->value + value.value) % MOD;
}

template <std::size_t MOD>
ModuloInteger<MOD> ModuloInteger<MOD>::operator-(const ModuloInteger value) const noexcept
{
    return (this->value + MOD - value.value) % MOD;
}

template <std::size_t MOD>
void ModuloInteger<MOD>::operator-=(const ModuloInteger value) noexcept
{
    this->value = (this->value - value.value) % MOD;
}

template <std::size_t MOD>
ModuloInteger<MOD> ModuloInteger<MOD>::operator*(const ModuloInteger value) const noexcept
{
    return (this->value * value.value) % MOD;
}

template <std::size_t MOD>
void ModuloInteger<MOD>::operator*=(const ModuloInteger value) noexcept
{
    this->value = (this->value * value.value) % MOD;
}

template <std::size_t MOD>
ModuloInteger<MOD> ModuloInteger<MOD>::operator/(const ModuloInteger value) const noexcept
{
    return (this->value * value.inv().value) % MOD;
}

template <std::size_t MOD>
void ModuloInteger<MOD>::operator/=(const ModuloInteger value) noexcept
{
    this->value = (this->value * value.inv().value) % MOD;
}

template <std::size_t MOD>
ModuloInteger<MOD> ModuloInteger<MOD>::operator+(const std::size_t value) const noexcept
{
    return (this->value + value) % MOD;
}

template <std::size_t MOD>
void ModuloInteger<MOD>::operator+=(const std::size_t value) noexcept
{
    this->value = (this->value + value) % MOD;
}

template <std::size_t MOD>
ModuloInteger<MOD> ModuloInteger<MOD>::operator-(const std::size_t value) const noexcept
{
    return (this->value + MOD - value) % MOD;
}

template <std::size_t MOD>
void ModuloInteger<MOD>::operator-=(const std::size_t value) noexcept
{
    this->value = (this->value - value) % MOD;
}

template <std::size_t MOD>
ModuloInteger<MOD> ModuloInteger<MOD>::operator*(const std::size_t value) const noexcept
{
    return (this->value * value) % MOD;
}

template <std::size_t MOD>
void ModuloInteger<MOD>::operator*=(const std::size_t value) noexcept
{
    this->value = (this->value * value) % MOD;
}

template <std::size_t MOD>
ModuloInteger<MOD> ModuloInteger<MOD>::operator/(const std::size_t value) const noexcept
{
    return (this->value * ModuloInteger<MOD>(value).inv().value) % MOD;
}

template <std::size_t MOD>
void ModuloInteger<MOD>::operator/=(const std::size_t value) noexcept
{
    this->value = (this->value * ModuloInteger<MOD>(value).inv().value) % MOD;
}

template <std::size_t MOD>
std::ostream& operator<<(std::ostream& out, const ModuloInteger<MOD> value) noexcept
{
    out << value.value;
    return out;
}
