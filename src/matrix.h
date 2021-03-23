#ifndef MATRIX_H
#define MATRIX_H

#include <algorithm>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <vector>

namespace poly {
template <typename T> class Matrix
{
  public:
    Matrix() = default;
    Matrix(std::size_t rows_, std::size_t cols_)
        : m_rows(rows_), m_cols(cols_), m_data(rows_ * cols_)
    {}

    Matrix(std::size_t rows_, std::size_t cols_, const std::vector<T> &vec_)
        : m_rows(rows_), m_cols(cols_), m_data{vec_}
    {
        if (m_rows * m_cols != m_data.size())
            throw std::invalid_argument("rows * cols != data.size()");
    }

    Matrix(std::size_t rows_, std::size_t cols_, std::vector<T> &&vec_)
        : m_rows(rows_), m_cols(cols_), m_data{std::move(vec_)}
    {
        if (m_rows * m_cols != m_data.size())
            throw std::invalid_argument("rows * cols != data.size()");
    }

    Matrix(std::size_t rows_, const std::vector<T> &vec_)
        : Matrix(rows_, get_cols_(rows_, vec_.size()), vec_)
    {}

    Matrix(std::size_t rows_, std::vector<T> &&vec_)
        : Matrix(rows_, get_cols_(rows_, vec_.size()), std::move(vec_))
    {}

    Matrix(const Matrix &) = default;
    Matrix(Matrix &&) = default;
    Matrix &operator=(const Matrix &) = default;
    Matrix &operator=(Matrix &&) = default;
    ~Matrix() = default;

    Matrix &operator+=(const Matrix &other)
    {
        if (m_rows != other.m_rows || m_cols != other.m_cols)
            throw std::logic_error("rows and cols must match to add Matrices");

        std::transform(m_data.cbegin(), m_data.cend(), other.m_data.cbegin(),
                       m_data.begin(), std::plus<>{});
        return *this;
    }

    Matrix &operator-=(const Matrix &other)
    {
        if (m_rows != other.m_rows || m_cols != other.m_cols)
            throw std::logic_error(
                "rows and cols must match to minus Matrices");

        std::transform(m_data.cbegin(), m_data.cend(), other.m_data.cbegin(),
                       m_data.begin(), std::minus<>{});
        return *this;
    }

    template <typename X> Matrix &operator*=(const X val)
    {
        static_assert(std::is_arithmetic<X>::value, "Not an arithmetic type ");
        std::transform(m_data.cbegin(), m_data.cend(), m_data.begin(),
                       [val](const auto &elem) { return val * elem; });
        return *this;
    }

    friend Matrix operator*(const Matrix &lhs, const Matrix &rhs)
    {
        if (lhs.m_cols != rhs.m_rows)
            throw std::logic_error(
                "lhs cols must match rhs rows to multiply PolynomialMatrices");

        const auto rows_ = lhs.m_rows;
        const auto cols_ = rhs.m_cols;
        const auto inner = lhs.m_cols;
        auto result = Matrix(rows_, cols_);

        for (std::size_t i = 0; i < rows_; ++i)
            for (std::size_t j = 0; j < cols_; ++j) {
                T sum{};
                for (std::size_t k = 0; k < inner; ++k)
                    sum += lhs(i, k) * rhs(k, j);
                result(i, j) = std::move(sum);
            }
        return result;
    }

    Matrix &operator*=(const Matrix &other) { return *this = *this * other; }

    template <typename X> auto operator()(const X x) const
    {
        static_assert(std::is_arithmetic<X>::value, "Not an arithmetic type");
        std::vector<X> result(m_data.size());
        std::transform(m_data.cbegin(), m_data.cend(), result.begin(),
                       [x](const auto &elem) { return elem(x); });
        return result;
    }

    auto &operator()(std::size_t row, std::size_t col)
    {
        return m_data[row * m_cols + col];
    }
    const auto &operator()(std::size_t row, std::size_t col) const
    {
        return m_data[row * m_cols + col];
    }

    auto &operator[](std::size_t idx) { return m_data[idx]; }
    const auto &operator[](std::size_t idx) const { return m_data[idx]; }

    auto size() const noexcept { return m_data.size(); }
    auto rows() const noexcept { return m_rows; }
    auto cols() const noexcept { return m_cols; }
    const auto &data() const { return m_data; };
    auto &data() { return m_data; };

  protected:
    std::size_t m_rows;
    std::size_t m_cols;

    std::vector<T> m_data;

  private:
    static auto get_cols_(std::size_t rows, std::size_t size)
    {
        const auto _cols = size / rows;
        return _cols ? _cols : 1;
    }
};

template <typename T>
inline auto operator==(const Matrix<T> &lhs, const Matrix<T> &rhs)
{
    return lhs.rows() == rhs.rows() && lhs.cols() == rhs.cols() &&
           lhs.data() == rhs.data();
}

template <typename T>
inline auto operator!=(const Matrix<T> &lhs, const Matrix<T> &rhs)
{
    return !(lhs == rhs);
}

template <typename T>
inline Matrix<T> operator+(const Matrix<T> &lhs, const Matrix<T> &rhs)
{
    auto ret(lhs);
    ret += rhs;
    return ret;
}

template <typename T>
inline Matrix<T> operator-(const Matrix<T> &lhs, const Matrix<T> &rhs)
{
    auto ret(lhs);
    ret -= rhs;
    return ret;
}

// template <typename T>
// inline Matrix<T> operator*(const Matrix<T> &lhs, const T val)
// {
//     auto ret(lhs);
//     ret *= val;
//     return ret;
// }

// template <typename T>
// inline Matrix<T> operator*(const T val, const Matrix<T> &rhs)
// {
//     return rhs * val;
// }

template <typename T>
std::ostream &operator<<(std::ostream &os, const Matrix<T> &mat)
{
    for (std::size_t i = 0; i < mat.size(); ++i) {
        if (i && i % mat.cols() == 0)
            os << '\n';
        os << "[ " << mat[i] << " ]";
    }
    return os;
}

} // namespace poly
#endif // MATRIX_H
