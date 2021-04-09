#ifndef POLYNOMIAL_FRACTION_H
#define POLYNOMIAL_FRACTION_H

#include <algorithm>
#include <cmath>
#include <iostream>
#include <iterator>
#include <type_traits>
#include <vector>

#include "polynomial.h"

namespace poly {
template <typename T> class PolynomialFraction
{
  public:
    PolynomialFraction() = default;
    PolynomialFraction(T val) : m_num{val}, m_den{val} {}
    PolynomialFraction(const Polynomial<T> &num_, const Polynomial<T> &den_)
        : m_num(num_), m_den(den_)
    {}
    PolynomialFraction(Polynomial<T> &&num_, const Polynomial<T> &den_)
        : m_num(std::move(num_)), m_den(den_)
    {}
    PolynomialFraction(const Polynomial<T> &num_, Polynomial<T> &&den_)
        : m_num(num_), m_den(std::move(den_))
    {}
    PolynomialFraction(Polynomial<T> &&num_, Polynomial<T> &&den_)
        : m_num(std::move(num_)), m_den(std::move(den_))
    {}

    PolynomialFraction(const std::vector<T> &num_, const std::vector<T> &den_)
        : m_num(num_), m_den(den_)
    {}
    PolynomialFraction(std::vector<T> &num_, const std::vector<T> &den_)
        : m_num(std::move(num_)), m_den(den_)
    {}
    PolynomialFraction(const std::vector<T> &num_, std::vector<T> &&den_)
        : m_num(num_), m_den(std::move(den_))
    {}
    PolynomialFraction(std::vector<T> &&num_, std::vector<T> &&den_)
        : m_num(std::move(num_)), m_den(std::move(den_))
    {}

    PolynomialFraction(std::initializer_list<T> init_num,
                       std::initializer_list<T> init_den)
        : m_num(init_num), m_den(init_den)
    {}

    PolynomialFraction(const PolynomialFraction &) = default;
    PolynomialFraction(PolynomialFraction &&) noexcept = default;
    PolynomialFraction &operator=(const PolynomialFraction &) = default;
    PolynomialFraction &operator=(PolynomialFraction &&) noexcept = default;
    ~PolynomialFraction() = default;

    PolynomialFraction &operator+=(const PolynomialFraction &other)
    {
        m_num += other.m_num;
        m_den += other.m_den;
        return *this;
    }

    PolynomialFraction &operator-=(const PolynomialFraction &other)
    {
        m_num -= other.m_num;
        m_den -= other.m_den;
        return *this;
    }

    PolynomialFraction &operator*=(const PolynomialFraction &other)
    {
        m_num *= other.m_num;
        m_den *= other.m_den;
        return *this;
    }

    PolynomialFraction &operator*=(const T val)
    {
        m_num *= val;
        m_den *= val;
        return *this;
    }

    template <typename X> auto operator()(const X x) const
    {
        static_assert(std::is_arithmetic<X>::value, "Not an arithmetic type");
        return m_num(x) / m_den(x);
    }

    void swap(PolynomialFraction &other) noexcept
    {
        m_num.swap(other.m_num);
        m_den.swap(other.m_den);
    }

    // auto &operator[](std::size_t idx) { return m_coeff[idx]; }
    // const auto &operator[](std::size_t idx) const { return m_coeff[idx]; }

    // auto size() const noexcept { return m_coeff.size(); }
    // auto degree() const noexcept { return m_coeff.size() - 1; }
    const auto &num() const noexcept { return m_num; };
    auto &num() noexcept { return m_num; };
    const auto &den() const noexcept { return m_den; };
    auto &den() noexcept { return m_den; };

  private:
    Polynomial<T> m_num;
    Polynomial<T> m_den;
};

template <typename T>
inline auto operator==(const PolynomialFraction<T> &lhs,
                       const PolynomialFraction<T> &rhs)
{
    return lhs.num() == rhs.num() && lhs.den() == rhs.den();
}

template <typename T>
inline auto operator!=(const PolynomialFraction<T> &lhs,
                       const PolynomialFraction<T> &rhs)
{
    return !(lhs == rhs);
}

template <typename T>
inline PolynomialFraction<T> operator+(const PolynomialFraction<T> &lhs,
                                       const PolynomialFraction<T> &rhs)
{
    auto ret(lhs);
    ret += rhs;
    return ret;
}

template <typename T>
inline PolynomialFraction<T> operator-(const PolynomialFraction<T> &lhs,
                                       const PolynomialFraction<T> &rhs)
{
    auto ret(lhs);
    ret -= rhs;
    return ret;
}

template <typename T>
inline PolynomialFraction<T> operator*(const PolynomialFraction<T> &lhs,
                                       const PolynomialFraction<T> &rhs)
{
    auto ret(lhs);
    ret *= rhs;
    return ret;
}

template <typename T>
inline PolynomialFraction<T> operator*(const PolynomialFraction<T> &lhs,
                                       const T val)
{
    auto ret(lhs);
    ret *= val;
    return ret;
}

template <typename T>
inline PolynomialFraction<T> operator*(const T val,
                                       const PolynomialFraction<T> &rhs)
{
    return rhs * val;
}

template <typename T>
std::ostream &operator<<(std::ostream &os, const PolynomialFraction<T> &frac)
{
    os << "[ " << frac.num() << " ] / [ " << frac.den() << " ]";
    return os;
}

template <typename T>
inline PolynomialFraction<T> conj(const PolynomialFraction<T> &frac) noexcept
{
    static_assert(true && "conj shouldn't be called\n");
    return frac;
}

} // namespace poly
#endif // POLYNOMIAL_FRACTION_H
