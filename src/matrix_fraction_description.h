#ifndef MATRIX_FRACTION_DESCRIPTION_H
#define MATRIX_FRACTION_DESCRIPTION_H

#include <algorithm>
#include <vector>

#include "matrix.h"
#include "polynomial_fraction.h"
#include "polynomial_matrix.h"

namespace poly {
template <typename T>
class PolynomialFractionMatrix : public Matrix<PolynomialFraction<T>>
{
    using _Matrix = Matrix<PolynomialFraction<T>>;

  public:
    PolynomialFractionMatrix() = default;
    PolynomialFractionMatrix(std::size_t rows_, std::size_t cols_)
        : _Matrix(rows_, cols_)
    {}
    PolynomialFractionMatrix(std::size_t rows_, std::size_t cols_,
                             const std::vector<PolynomialFraction<T>> &vec_)
        : _Matrix(rows_, cols_, vec_)
    {}
    PolynomialFractionMatrix(std::size_t rows_, std::size_t cols_,
                             std::vector<PolynomialFraction<T>> &&vec_)
        : _Matrix(rows_, cols_, std::move(vec_))
    {}
    PolynomialFractionMatrix(std::size_t rows_,
                             const std::vector<PolynomialFraction<T>> &vec_)
        : _Matrix(rows_, vec_)
    {}
    PolynomialFractionMatrix(std::size_t rows_,
                             std::vector<PolynomialFraction<T>> &&vec_)
        : _Matrix(rows_, std::move(vec_))
    {}

    PolynomialFractionMatrix(const PolynomialFractionMatrix &) = default;
    PolynomialFractionMatrix(PolynomialFractionMatrix &&) = default;
    PolynomialFractionMatrix &
    operator=(const PolynomialFractionMatrix &) = default;
    PolynomialFractionMatrix &operator=(PolynomialFractionMatrix &&) = default;
    ~PolynomialFractionMatrix() = default;

    auto polynomial_matrices() const
    {
        const auto _rows = this->rows();
        const auto _cols = this->cols();
        const auto _data = this->data();
        PolynomialMatrix<T> num(_rows, _cols);
        PolynomialMatrix<T> den(_rows, _cols);
        std::size_t idx = 0;
        for (const auto &i : _data) {
            num[idx] = i.num();
            den[idx] = i.den();
            ++idx;
        }
        return std::make_pair(num, den);
    }

    auto get_left() const
    {
        const auto _rows = this->rows();
        const auto _cols = this->cols();
        const auto _size = this->size();
        auto [Nl, den] = this->polynomial_matrices();
        auto Dl = PolynomialMatrix<T>(_rows, _rows);

        auto is_new_row = [_cols](auto i) { return i % _cols == 0; };

        std::size_t diag_idx = 0;
        std::size_t same_row_idx = 0;
        for (std::size_t i = 0; i < _size; ++i) {
            if (is_new_row(i)) {
                if (i) {
                    ++diag_idx;
                    same_row_idx += _cols;
                }
                Dl(diag_idx, diag_idx) = den[i];
            } else {
                Dl(diag_idx, diag_idx) *= den[i];
            }
            for (std::size_t j = same_row_idx; j < same_row_idx + _cols; ++j) {
                if (i != j)
                    Nl[i] *= den[j];
            }
        }
        // Nl.remove_trailing_zeros();
        // Dl.remove_trailing_zeros();
        return std::make_pair(std::move(Nl), std::move(Dl));
    }
};

template <typename T>
inline PolynomialFractionMatrix<T>
operator*(const PolynomialFractionMatrix<T> &lhs, const T val)
{
    auto ret(lhs);
    ret *= val;
    return ret;
}

template <typename T>
inline PolynomialFractionMatrix<T>
operator*(const T val, const PolynomialFractionMatrix<T> &rhs)
{
    return rhs * val;
}

} // namespace poly
#endif // MATRIX_FRACTION_DESCRIPTION_H
