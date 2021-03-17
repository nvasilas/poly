#ifndef POLYNOMIAL_MATRIX_H
#define POLYNOMIAL_MATRIX_H

#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>
#include <stdexcept>

#include "polynomial.h"

namespace poly {
template<typename T> class PolynomialMatrix
{
  public:
    using EvalType = std::vector<T>;

    PolynomialMatrix() = default;
    PolynomialMatrix(std::size_t rows_, std::size_t cols_)
        : rows(rows_), cols(cols_), poly_vec(rows_ * cols_)
    {}

    PolynomialMatrix(std::size_t rows_, const std::vector<Polynomial<T>> &p_)
        : rows(rows_),
          cols((p_.size() / rows_) ? p_.size() / rows_ : 1), poly_vec{ p_ }
    {}

    PolynomialMatrix(std::size_t rows_, std::vector<Polynomial<T>> &&p_)
        : rows(rows_),
          cols((p_.size() / rows_) ? p_.size() / rows_ : 1), poly_vec{ p_ }
    {}

    PolynomialMatrix(std::size_t rows_,
        std::size_t cols_,
        const std::vector<Polynomial<T>> &pol_)
        : rows(rows_), cols(cols_), poly_vec{ pol_ }
    {
        if (rows * cols != poly_vec.size())
            throw std::invalid_argument("rows * cols != poly_vec.size()");
    }

    PolynomialMatrix(std::size_t rows_,
        std::size_t cols_,
        std::vector<Polynomial<T>> &&pol_)
        : rows(rows_), cols(cols_), poly_vec{ pol_ }
    {
        if (rows * cols != poly_vec.size())
            throw std::invalid_argument("rows * cols != poly_vec.size()");
    }

    PolynomialMatrix(const PolynomialMatrix &) = default;
    PolynomialMatrix(PolynomialMatrix &&) = default;
    PolynomialMatrix &operator=(const PolynomialMatrix &) = default;
    PolynomialMatrix &operator=(PolynomialMatrix &&) = default;

    ~PolynomialMatrix() = default;

    auto operator==(const PolynomialMatrix &other) const
    {
        return rows == other.rows && cols == other.cols
               && poly_vec == other.poly_vec;
    }

    auto operator!=(const PolynomialMatrix &other) const
    {
        return !(*this == other);
    }

    PolynomialMatrix &operator+=(const PolynomialMatrix &other)
    {
        if (rows != other.rows || cols != other.cols)
            throw std::logic_error(
                "rows and cols must match to add PolynomialMatrices");

        std::transform(poly_vec.cbegin(),
            poly_vec.cend(),
            other.poly_vec.cbegin(),
            poly_vec.begin(),
            std::plus<>{});
        return *this;
    }

    PolynomialMatrix &operator-=(const PolynomialMatrix &other)
    {
        if (rows != other.rows || cols != other.cols)
            throw std::logic_error(
                "rows and cols must match to minus PolynomialMatrices");

        std::transform(poly_vec.cbegin(),
            poly_vec.cend(),
            other.poly_vec.cbegin(),
            poly_vec.begin(),
            std::minus<>{});
        return *this;
    }

    PolynomialMatrix &operator*=(const T val)
    {
        std::transform(poly_vec.cbegin(),
            poly_vec.cend(),
            poly_vec.begin(),
            [val](const auto &poly) { return val * poly; });
        return *this;
    }

    friend PolynomialMatrix operator*(const PolynomialMatrix &lhs,
        const PolynomialMatrix &rhs)
    {
        if (lhs.cols != rhs.rows)
            throw std::logic_error(
                "lhs cols must match rhs rows to multiply PolynomialMatrices");

        const auto rows_ = lhs.rows;
        const auto cols_ = rhs.cols;
        const auto inner = lhs.cols;
        auto result = PolynomialMatrix(rows_, cols_);

        for (std::size_t i = 0; i < rows_; ++i)
            for (std::size_t j = 0; j < cols_; ++j) {
                Polynomial<T> sum;
                for (std::size_t k = 0; k < inner; ++k)
                    sum += lhs(i, k) * rhs(k, j);
                result(i, j) = std::move(sum);
            }
        return result;
    }

    PolynomialMatrix &operator*=(const PolynomialMatrix &other)
    {
        return *this = *this * other;
    }

    auto operator()(const T x) const
    {
        EvalType result(poly_vec.size());
        std::transform(poly_vec.cbegin(),
            poly_vec.cend(),
            result.begin(),
            [x](const auto &poly) { return poly(x); });
        return result;
    }

    auto &operator()(std::size_t row, std::size_t col)
    {
        return poly_vec[row * cols + col];
    }
    const auto &operator()(std::size_t row, std::size_t col) const
    {
        return poly_vec[row * cols + col];
    }

    auto &operator[](std::size_t idx) { return poly_vec[idx]; }
    const auto &operator[](std::size_t idx) const { return poly_vec[idx]; }

    auto max_degree() const
    {
        return std::max_element(poly_vec.cbegin(),
            poly_vec.cend(),
            [](const auto &lhs, const auto &rhs) {
                return lhs.degree() < rhs.degree();
            })
            ->degree();
    }

    auto size() const { return poly_vec.size(); }
    auto get_rows() const { return rows; }
    auto get_cols() const { return cols; }

  private:
    std::size_t rows;
    std::size_t cols;

    std::vector<Polynomial<T>> poly_vec;
};

template<typename T>
inline PolynomialMatrix<T> operator+(const PolynomialMatrix<T> &lhs,
    const PolynomialMatrix<T> &rhs)
{
    auto ret(lhs);
    ret += rhs;
    return ret;
}

template<typename T>
inline PolynomialMatrix<T> operator-(const PolynomialMatrix<T> &lhs,
    const PolynomialMatrix<T> &rhs)
{
    auto ret(lhs);
    ret -= rhs;
    return ret;
}

template<typename T>
inline PolynomialMatrix<T> operator*(const PolynomialMatrix<T> &lhs,
    const T val)
{
    auto ret(lhs);
    ret *= val;
    return ret;
}

template<typename T>
inline PolynomialMatrix<T> operator*(const T val,
    const PolynomialMatrix<T> &rhs)
{
    return rhs * val;
}

template<typename T>
std::ostream &operator<<(std::ostream &os, const PolynomialMatrix<T> &poly_mat)
{
    for (std::size_t i = 0; i < poly_mat.size(); ++i) {
        if (i && i % poly_mat.get_cols() == 0) os << "\n";
        os << "[ " << poly_mat[i] << " ]";
    }
    return os;
}

}// namespace poly
#endif// POLYNOMIAL_MATRIX_H
