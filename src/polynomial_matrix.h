#ifndef POLYNOMIAL_MATRIX_H
#define POLYNOMIAL_MATRIX_H

#include <algorithm>
#include <vector>

#include "matrix.h"
#include "polynomial.h"

namespace poly {
template <typename T> class PolynomialMatrix : public Matrix<Polynomial<T>>
{
    using _Matrix = Matrix<Polynomial<T>>;

  public:
    PolynomialMatrix() = default;
    PolynomialMatrix(std::size_t rows_, std::size_t cols_)
        : _Matrix(rows_, cols_)
    {}
    PolynomialMatrix(std::size_t rows_, std::size_t cols_,
                     const std::vector<Polynomial<T>> &vec_)
        : _Matrix(rows_, cols_, vec_)
    {}
    PolynomialMatrix(std::size_t rows_, std::size_t cols_,
                     std::vector<Polynomial<T>> &&vec_)
        : _Matrix(rows_, cols_, std::move(vec_))
    {}
    PolynomialMatrix(std::size_t rows_, const std::vector<Polynomial<T>> &vec_)
        : _Matrix(rows_, vec_)
    {}
    PolynomialMatrix(std::size_t rows_, std::vector<Polynomial<T>> &&vec_)
        : _Matrix(rows_, std::move(vec_))
    {}

    PolynomialMatrix(const PolynomialMatrix &) = default;
    PolynomialMatrix(PolynomialMatrix &&) = default;
    PolynomialMatrix &operator=(const PolynomialMatrix &) = default;
    PolynomialMatrix &operator=(PolynomialMatrix &&) = default;
    ~PolynomialMatrix() = default;

    void remove_trailing_zeros()
    {
        for (auto &poly : this->data())
            poly.remove_trailing_zeros();
    }

    auto max_degree() const
    {
        const auto _data = this->data();
        return std::max_element(_data.cbegin(), _data.cend(),
                                [](const auto &lhs, const auto &rhs) {
                                    return lhs.degree() < rhs.degree();
                                })
            ->degree();
    }

    auto to_matrix() const
    {
        const auto max_col = this->max_degree() + 1;
        std::vector<T> mat(this->rows() * max_col * this->cols(), T{});

        auto position = mat.begin();
        const auto _data = this->data();
        for (const auto &poly : _data) {
            std::copy(poly.data().cbegin(), poly.data().cend(), position);
            std::advance(position, max_col);
        }
        return mat;
    }

    auto to_coeff_matrix() const
    {
        const auto _rows = this->rows();
        const auto _cols = this->cols();
        if (_rows == 1 || _cols == 1)
            return to_matrix();
        const auto mat = to_matrix();
        std::vector<int> ret;
        ret.reserve(mat.size());

        const auto max_col = max_degree() + 1;
        for (std::size_t i = 0; i < _rows; ++i)
            for (std::size_t j = 0; j < max_col; ++j) {
                const auto row_ = i * _cols * max_col;
                ret.push_back(mat[j + row_]);
                ret.push_back(mat[j + max_col + row_]);
            }
        return ret;
    }
};

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

} // namespace poly
#endif // POLYNOMIAL_MATRIX_H
