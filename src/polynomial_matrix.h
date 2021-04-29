#ifndef POLYNOMIAL_MATRIX_H
#define POLYNOMIAL_MATRIX_H

#include <algorithm>
#include <blaze/Blaze.h>

#include "polynomial.h"

namespace poly {

template <typename T>
using PolynomialMatrix = typename blaze::DynamicMatrix<poly::Polynomial<T>>;

// template <typename T> using Matrix = typename blaze::DynamicMatrix<T>;

template <typename T> struct Matrix
{
    auto rows() const noexcept { return matrix.rows(); }
    auto columns() const noexcept { return matrix.columns(); }
    auto coef_columns() const noexcept { return matrix.columns() / deg; }

    auto &operator()(std::size_t row, std::size_t col)
    {
        return matrix(row, col);
    }
    const auto &operator()(std::size_t row, std::size_t col) const
    {
        return matrix(row, col);
    }

    blaze::DynamicMatrix<T> matrix;
    std::size_t deg;
};

template <typename T>
std::ostream &operator<<(std::ostream &os, const Matrix<T> &m)
{
    os << "deg: " << m.deg << ", matrix:\n";
    os << m.matrix;
    return os;
}

// TODO
// template <typename T> auto to_matrix(const PolynomialMatrix<T> &poly_matrix)
// {
//     const auto rows = poly_matrix.rows();
//     const auto max_size = max(poly_matrix).size();
//     Matrix<T> ret(rows, max_size * poly_matrix.columns(), T(0));

//     for (std::size_t i = 0; i < rows; ++i) {
//         auto position = ret.begin(i);
//         for (auto it = poly_matrix.cbegin(i); it != poly_matrix.cend(i);
//         ++it) {
//             std::copy(it->data().cbegin(), it->data().cend(), position);
//             std::advance(position, max_size);
//         }
//     }
//     return ret;
// }

// Matrix<int> in{
//     {1, 0, -7, 6, | 0, 0, 0, 0, | 1, 2, 0, 0},
//     {1, -1, -4, 4,| 1, 0, 0, 0, | 1, 0, 0, 0},
//     {1, 5, 6, 0,  | 0, 0, 0, 0, | 0, 0, 0 ,0}
// };
// Matrix<int> out{
//     {1, 0, 1, | 0, 0, 2, | -7, 0, 0, | 6, 0, 0},
//     {1, 1, 1, | -1, 0, 0,| -4, 0, 0, | 4, 0, 0},
//     {1, 0, 0, | 5, 0, 0, | 0, 0, 6,  | 0, 0, 0}
// };
// ret: 0, 1, 2 | 3, 4, 5 | 6, 7, 8  | 9, 10, 11
// mat: 0, 4, 8 | 1, 5, 9 | 2, 6, 10 | 3, 7, 11
template <typename T>
auto to_coeff_matrix(const PolynomialMatrix<T> &poly_matrix)
{
    const auto rows = poly_matrix.rows();
    const auto cols = poly_matrix.columns();
    // TODO
    // if (rows == 1 || cols == 1)
    //     return to_matrix(poly_matrix);
    const auto max_size = max(poly_matrix).size();

    Matrix<T> ret{/*matrix*/ {rows, max_size * cols},
                  /*deg*/ max_size};

    for (std::size_t i = 0; i < rows; ++i) {
        auto it = ret.matrix.begin(i);
        for (std::size_t k = 0; k < max_size; ++k) {
            for (auto cit = poly_matrix.cbegin(i); cit != poly_matrix.cend(i);
                 ++cit) {
                *it++ = (k < cit->size()) ? (*cit)[k] : T(0);
            }
        }
    }
    return ret;
}

} // namespace poly
#endif // POLYNOMIAL_MATRIX_H
