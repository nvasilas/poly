#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <type_traits>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>
#include <cmath>

namespace {
template<typename T> class Polynomial
{
    using CoeffType = std::vector<T>;

  public:
    Polynomial() = default;
    explicit Polynomial(const std::vector<T> &coeff_) : coeff(coeff_) {}
    explicit Polynomial(std::vector<T> &&coeff_) : coeff(std::move(coeff_)) {}
    Polynomial(const T c[], std::size_t deg) : coeff(c, c + deg + 1) {}
    explicit Polynomial(std::initializer_list<T> l) : coeff(l) {}

    Polynomial(const Polynomial &) = default;
    Polynomial(Polynomial &&) = default;
    Polynomial &operator=(const Polynomial &) = default;
    Polynomial &operator=(Polynomial &&) = default;

    ~Polynomial() = default;

    Polynomial &operator+=(const Polynomial &other)
    {
        const auto &[small, large] = std::minmax(coeff,
            other.coeff,
            [](const auto &a, const auto &b) { return a.size() < b.size(); });

        auto result = large;
        auto it = result.begin();
        std::advance(it, large.size() - small.size());
        std::transform(small.cbegin(), small.cend(), it, it, std::plus<>{});
        coeff = std::move(result);
        return *this;
    }

    Polynomial &operator-=(const Polynomial &other)
    {
        auto result(other.coeff);
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
                result[i + j] += coeff[i] * other[j];
        coeff = std::move(result);
        return *this;
    }

    Polynomial &operator*=(const T val)
    {
        std::transform(
            coeff.cbegin(), coeff.cend(), coeff.begin(), [val](auto i) {
                return i * val;
            });
        return *this;
    }

    auto operator()(const T x) const
    {
        T result{};
        for (auto i : coeff) result = result * x + i;
        return result;
    }

    auto &operator[](std::size_t idx) { return coeff[idx]; }
    const auto &operator[](std::size_t idx) const { return coeff[idx]; }

    auto degree() const { return coeff.size() - 1; }
    auto &coefficients() const { return coeff; };

  private:
    // coeff[0] * x ** deg + coeff[1] * x ** (deg-1) + ... + coeff.back()
    CoeffType coeff;
};

template<typename T>
inline auto operator==(const Polynomial<T> &lhs, const Polynomial<T> &rhs)
{
    if constexpr (std::is_floating_point_v<T>)
        // return coeff == other.coeff;
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
inline Polynomial<T> operator-(const Polynomial<T> &lhs,
    const Polynomial<T> &rhs)
{
    auto ret(lhs);
    ret -= rhs;
    return ret;
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
std::ostream &operator<<(std::ostream &os, const Polynomial<T> &poly)
{
    const auto coeff = poly.coefficients();
    std::copy(std::begin(coeff),
        std::prev(std::end(coeff)),
        std::ostream_iterator<T>(os, ", "));
    os << coeff.back();
    return os;
}

}// namespace
#endif// POLYNOMIAL_H
