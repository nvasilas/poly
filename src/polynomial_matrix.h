#ifndef POLYNOMIAL_MATRIX_H
#define POLYNOMIAL_MATRIX_H

#include <algorithm>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <vector>

#include "polynomial.h"

namespace poly {
template <typename T> class PolynomialMatrix
{
  public:
    PolynomialMatrix() = default;
    PolynomialMatrix(std::size_t rows_, std::size_t cols_)
        : m_rows(rows_), m_cols(cols_), m_vec(rows_ * cols_)
    {}

    PolynomialMatrix(std::size_t rows_, std::size_t cols_,
                     const std::vector<Polynomial<T>> &vec_)
        : m_rows(rows_), m_cols(cols_), m_vec{vec_}
    {
        if (m_rows * m_cols != m_vec.size())
            throw std::invalid_argument("rows * cols != poly_vec.size()");
    }

    PolynomialMatrix(std::size_t rows_, std::size_t cols_,
                     std::vector<Polynomial<T>> &&vec_)
        : m_rows(rows_), m_cols(cols_), m_vec{std::move(vec_)}
    {
        if (m_rows * m_cols != m_vec.size())
            throw std::invalid_argument("rows * cols != poly_vec.size()");
    }

    PolynomialMatrix(std::size_t rows_, const std::vector<Polynomial<T>> &vec_)
        : PolynomialMatrix(
              rows_, (vec_.size() / rows_) ? vec_.size() / rows_ : 1, vec_)
    {}

    PolynomialMatrix(std::size_t rows_, std::vector<Polynomial<T>> &&vec_)
        : PolynomialMatrix(rows_,
                           (vec_.size() / rows_) ? vec_.size() / rows_ : 1,
                           std::move(vec_))
    {}

    PolynomialMatrix(const PolynomialMatrix &) = default;
    PolynomialMatrix(PolynomialMatrix &&) = default;
    PolynomialMatrix &operator=(const PolynomialMatrix &) = default;
    PolynomialMatrix &operator=(PolynomialMatrix &&) = default;

    ~PolynomialMatrix() = default;

    PolynomialMatrix &operator+=(const PolynomialMatrix &other)
    {
        if (m_rows != other.m_rows || m_cols != other.m_cols)
            throw std::logic_error(
                "rows and cols must match to add PolynomialMatrices");

        std::transform(m_vec.cbegin(), m_vec.cend(), other.m_vec.cbegin(),
                       m_vec.begin(), std::plus<>{});
        return *this;
    }

    PolynomialMatrix &operator-=(const PolynomialMatrix &other)
    {
        if (m_rows != other.m_rows || m_cols != other.m_cols)
            throw std::logic_error(
                "rows and cols must match to minus PolynomialMatrices");

        std::transform(m_vec.cbegin(), m_vec.cend(), other.m_vec.cbegin(),
                       m_vec.begin(), std::minus<>{});
        return *this;
    }

    PolynomialMatrix &operator*=(const T val)
    {
        std::transform(m_vec.cbegin(), m_vec.cend(), m_vec.begin(),
                       [val](const auto &poly) { return val * poly; });
        return *this;
    }

    friend PolynomialMatrix operator*(const PolynomialMatrix &lhs,
                                      const PolynomialMatrix &rhs)
    {
        if (lhs.m_cols != rhs.m_rows)
            throw std::logic_error(
                "lhs cols must match rhs rows to multiply PolynomialMatrices");

        const auto rows_ = lhs.m_rows;
        const auto cols_ = rhs.m_cols;
        const auto inner = lhs.m_cols;
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
        std::vector<T> result(m_vec.size());
        std::transform(m_vec.cbegin(), m_vec.cend(), result.begin(),
                       [x](const auto &poly) { return poly(x); });
        return result;
    }

    auto &operator()(std::size_t row, std::size_t col)
    {
        return m_vec[row * m_cols + col];
    }
    const auto &operator()(std::size_t row, std::size_t col) const
    {
        return m_vec[row * m_cols + col];
    }

    auto &operator[](std::size_t idx) { return m_vec[idx]; }
    const auto &operator[](std::size_t idx) const { return m_vec[idx]; }

    auto max_degree() const
    {
        return std::max_element(m_vec.cbegin(), m_vec.cend(),
                                [](const auto &lhs, const auto &rhs) {
                                    return lhs.degree() < rhs.degree();
                                })
            ->degree();
    }

    auto to_matrix() const
    {
        const auto max_col = max_degree() + 1;
        std::vector<T> mat(m_rows * max_col * m_cols, T{});

        auto position = mat.begin();
        for (const auto &poly : m_vec) {
            std::copy(poly.coefficients().cbegin(), poly.coefficients().cend(),
                      position);
            std::advance(position, max_col);
        }
        return mat;
    }

    auto to_coeff_matrix() const
    {
        if (m_rows == 1 || m_cols == 1)
            return to_matrix();
        const auto mat = to_matrix();
        std::vector<int> ret;
        ret.reserve(mat.size());

        const auto max_col = max_degree() + 1;
        for (std::size_t i = 0; i < m_rows; ++i)
            for (std::size_t j = 0; j < max_col; ++j) {
                const auto row_ = i * m_cols * max_col;
                ret.push_back(mat[j + row_]);
                ret.push_back(mat[j + max_col + row_]);
            }
        return ret;
    }

    auto size() const { return m_vec.size(); }
    auto rows() const { return m_rows; }
    auto cols() const { return m_cols; }
    auto &polynomial_vector() const { return m_vec; };

  private:
    std::size_t m_rows;
    std::size_t m_cols;

    std::vector<Polynomial<T>> m_vec;
};

template <typename T>
inline auto operator==(const PolynomialMatrix<T> &lhs,
                       const PolynomialMatrix<T> &rhs)
{
    return lhs.rows() == rhs.rows() && lhs.cols() == rhs.cols() &&
           lhs.polynomial_vector() == rhs.polynomial_vector();
}

template <typename T>
inline auto operator!=(const PolynomialMatrix<T> &lhs,
                       const PolynomialMatrix<T> &rhs)
{
    return !(lhs == rhs);
}

template <typename T>
inline PolynomialMatrix<T> operator+(const PolynomialMatrix<T> &lhs,
                                     const PolynomialMatrix<T> &rhs)
{
    auto ret(lhs);
    ret += rhs;
    return ret;
}

template <typename T>
inline PolynomialMatrix<T> operator-(const PolynomialMatrix<T> &lhs,
                                     const PolynomialMatrix<T> &rhs)
{
    auto ret(lhs);
    ret -= rhs;
    return ret;
}

template <typename T>
inline PolynomialMatrix<T> operator*(const PolynomialMatrix<T> &lhs,
                                     const T val)
{
    auto ret(lhs);
    ret *= val;
    return ret;
}

template <typename T>
inline PolynomialMatrix<T> operator*(const T val,
                                     const PolynomialMatrix<T> &rhs)
{
    return rhs * val;
}

template <typename T>
std::ostream &operator<<(std::ostream &os, const PolynomialMatrix<T> &poly_mat)
{
    for (std::size_t i = 0; i < poly_mat.size(); ++i) {
        if (i && i % poly_mat.cols() == 0)
            os << "\n";
        os << "[ " << poly_mat[i] << " ]";
    }
    return os;
}

} // namespace poly
#endif // POLYNOMIAL_MATRIX_H
