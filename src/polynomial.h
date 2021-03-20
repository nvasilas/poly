#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <type_traits>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>
#include <cmath>

namespace poly {
template<typename T> class Polynomial
{
  public:
    using CoeffType = std::vector<T>;

    Polynomial() = default;
    explicit Polynomial(const std::vector<T> &coeff_) : m_coeff(coeff_) {}
    explicit Polynomial(std::vector<T> &&coeff_) : m_coeff(std::move(coeff_)) {}
    Polynomial(const T c[], std::size_t deg) : m_coeff(c, c + deg + 1) {}
    Polynomial(std::initializer_list<T> init) : m_coeff(init) {}

    Polynomial(const Polynomial &) = default;
    Polynomial(Polynomial &&) = default;
    Polynomial &operator=(const Polynomial &) = default;
    Polynomial &operator=(Polynomial &&) = default;

    Polynomial &operator=(std::initializer_list<T> init)
    {
        m_coeff.assign(std::begin(init), std::end(init));
        return *this;
    }

    ~Polynomial() = default;

    Polynomial &operator+=(const Polynomial &other)
    {
        const auto &[small, large] = std::minmax(m_coeff,
            other.m_coeff,
            [](const auto &a, const auto &b) { return a.size() < b.size(); });

        auto result = large;
        auto it = result.begin();
        std::advance(it, large.size() - small.size());
        std::transform(small.cbegin(), small.cend(), it, it, std::plus<>{});
        m_coeff = std::move(result);
        return *this;
    }

    Polynomial &operator-=(const Polynomial &other)
    {
        auto result(other.m_coeff);
        std::transform(
            result.cbegin(), result.cend(), result.begin(), std::negate<>{});
        *this += Polynomial{ std::move(result) };
        return *this;
    }

    Polynomial &operator*=(const Polynomial &other)
    {
        CoeffType result(degree() + other.degree() + 1);
        for (std::size_t i = 0; i <= degree(); ++i)
            for (std::size_t j = 0; j <= other.degree(); ++j)
                result[i + j] += m_coeff[i] * other[j];
        m_coeff = std::move(result);
        return *this;
    }

    Polynomial &operator*=(const T val)
    {
        std::transform(
            m_coeff.cbegin(), m_coeff.cend(), m_coeff.begin(), [val](auto i) {
                return i * val;
            });
        return *this;
    }

    template<typename X> auto operator()(const X x) const
    {
        static_assert(std::is_arithmetic<X>::value, "Not an arithmetic type");
        X result{};
        for (auto i : m_coeff) result = result * x + i;
        return result;
    }

    auto &operator[](std::size_t idx) { return m_coeff[idx]; }
    const auto &operator[](std::size_t idx) const { return m_coeff[idx]; }

    auto size() const { return m_coeff.size(); }
    auto degree() const { return m_coeff.size() - 1; }
    auto &coefficients() const { return m_coeff; };

  private:
    // m_coeff[0] * x ** deg + m_coeff[1] * x ** (deg-1) + ... + m_coeff.back()
    CoeffType m_coeff;
};

template<typename T>
inline auto operator==(const Polynomial<T> &lhs, const Polynomial<T> &rhs)
{
    if constexpr (std::is_floating_point_v<T>)
        // return lhs.coefficients() == rhs.coefficients();
        return std::equal(lhs.coefficients().cbegin(),
            lhs.coefficients().cend(),
            rhs.coefficients().cbegin(),
            [](T x, T y) {
                constexpr double epsilon = 1e-6;
                return std::fabs(x - y) < epsilon;
            });
    else
        return lhs.coefficients() == rhs.coefficients();
}

template<typename T>
inline auto operator!=(const Polynomial<T> &lhs, const Polynomial<T> &rhs)
{
    return !(lhs == rhs);
}

template<typename T>
inline Polynomial<T> operator+(const Polynomial<T> &lhs,
    const Polynomial<T> &rhs)
{
    auto ret(lhs);
    ret += rhs;
    return ret;
}

template<typename T>
inline Polynomial<T> operator+(Polynomial<T> &&lhs, const Polynomial<T> &rhs)
{
    lhs += rhs;
    return lhs;
}

template<typename T>
inline Polynomial<T> operator+(const Polynomial<T> &lhs, Polynomial<T> &&rhs)
{
    rhs += lhs;
    return rhs;
}

template<typename T>
inline Polynomial<T> operator+(Polynomial<T> &&lhs, Polynomial<T> &&rhs)
{
    lhs += rhs;
    return lhs;
}

template<typename T>
inline Polynomial<T> operator-(const Polynomial<T> &lhs,
    const Polynomial<T> &rhs)
{
    auto ret(lhs);
    ret -= rhs;
    return ret;
}

template<typename T>
inline Polynomial<T> operator-(Polynomial<T> &&lhs, const Polynomial<T> &rhs)
{
    lhs -= rhs;
    return lhs;
}

template<typename T>
inline Polynomial<T> operator-(const Polynomial<T> &lhs, Polynomial<T> &&rhs)
{
    rhs -= lhs;
    return rhs;
}

template<typename T>
inline Polynomial<T> operator-(Polynomial<T> &&lhs, Polynomial<T> &&rhs)
{
    lhs -= rhs;
    return lhs;
}

template<typename T>
inline Polynomial<T> operator*(const Polynomial<T> &lhs,
    const Polynomial<T> &rhs)
{
    auto ret(lhs);
    ret *= rhs;
    return ret;
}

template<typename T>
inline Polynomial<T> operator*(const Polynomial<T> &lhs, const T val)
{
    auto ret(lhs);
    ret *= val;
    return ret;
}

template<typename T>
inline Polynomial<T> operator*(const T val, const Polynomial<T> &rhs)
{
    return rhs * val;
}

template<typename T>
auto divide(const Polynomial<T> &dividend, const Polynomial<T> &divisor)
{
    const auto size_dividend = dividend.size();
    const auto size_divisor = divisor.size();
    if (size_dividend < size_divisor) {
        return std::make_pair(Polynomial<T>{}, dividend);
    }

    using CoeffType = typename Polynomial<T>::CoeffType;
    CoeffType remainder = dividend.coefficients();
    CoeffType quotient(size_dividend - size_divisor + 1);
    for (std::size_t lead_coeff = 0; lead_coeff < quotient.size();
         ++lead_coeff) {
        const auto quo = remainder[lead_coeff] / divisor[0];
        quotient[lead_coeff] = quo;
        remainder[lead_coeff] = 0;
        for (std::size_t k = 1; k < size_divisor; ++k)
            remainder[k + lead_coeff] -= quo * divisor[k];
    }
    remainder.erase(remainder.begin(),
        std::find_if(remainder.cbegin(), remainder.cend(), [](T v) {
            return v != T(0);
        }));
    return std::make_pair(Polynomial<T>{ std::move(quotient) },
        Polynomial<T>{ std::move(remainder) });
}

template<typename T>
inline Polynomial<T> operator/(const Polynomial<T> &lhs,
    const Polynomial<T> &rhs)
{
    return divide(lhs, rhs).second;
}

template<typename T>
inline Polynomial<T> operator%(const Polynomial<T> &lhs,
    const Polynomial<T> &rhs)
{
    return divide(lhs, rhs).first;
}

template<typename T>
std::ostream &operator<<(std::ostream &os, const Polynomial<T> &poly)
{
    const auto coeff = poly.coefficients();
    std::copy(std::begin(coeff),
        std::prev(std::end(coeff)),
        std::ostream_iterator<T>(os, ", "));
    os << coeff.back();
    return os;
}

}// namespace poly
#endif// POLYNOMIAL_H
