#ifndef POLYNOMIAL_MATRIX_H
#define POLYNOMIAL_MATRIX_H

#include <algorithm>
#include <blaze/Blaze.h>

#include "polynomial.h"

namespace poly {

template <typename T>
using PolynomialMatrix = typename blaze::DynamicMatrix<poly::Polynomial<T>>;

template <typename T> using Matrix = typename blaze::DynamicMatrix<T>;

template <typename T> auto to_matrix(const PolynomialMatrix<T> &poly_matrix)
{
    const auto rows = poly_matrix.rows();
    const auto max_degree = max(poly_matrix).degree() + 1;
    Matrix<T> ret(rows, max_degree * poly_matrix.columns(), T(0));

    for (std::size_t i = 0; i < rows; ++i) {
        auto position = ret.begin(i);
        for (auto it = poly_matrix.cbegin(i); it != poly_matrix.cend(i); ++it) {
            std::copy(it->data().cbegin(), it->data().cend(), position);
            std::advance(position, max_degree);
        }
    }
    return ret;
}

// template <typename T>
// auto to_coeff_matrix(const PolynomialMatrix<T> &poly_matrix)
// {
//     if (poly_matrix.rows() == 1 || poly_matrix.columns() == 1)
//         return to_matrix(poly_matrix);
//     const auto mat = to_matrix(poly_matrix);
//     Matrix<T> ret(mat.rows(), mat.columns());

//     // Matrix<int> in{
//     //     {1, 0, -7, 6, | 0, 0, 0, 0, | 1, 2, 0, 0},
//     //     {1, -1, -4, 4,| 1, 0, 0, 0, | 1, 0, 0, 0},
//     //     {1, 5, 6, 0,  | 0, 0, 0, 0, | 0, 0, 0 ,0}
//     // };
//     // Matrix<int> out{
//     //     {1, 0, 1, | 0, 0, 2, | -7, 0, 0, | 6, 0, 0},
//     //     {1, 1, 1, | -1, 0, 0,| -4, 0, 0, | 4, 0, 0},
//     //     {1, 0, 0, | 5, 0, 0, | 0, 0, 6,  | 0, 0, 0}
//     // };
//     // ret: 0, 1, 2 | 3, 4, 5 | 6, 7, 8  | 9, 10, 11
//     // mat: 0, 4, 8 | 1, 5, 9 | 2, 6, 10 | 3, 7, 11

//     const auto max_degree = max(poly_matrix).degree() + 1;
//     for (std::size_t i = 0; i < mat.rows(); ++i) {
//         std::size_t ret_column = 0;
//         for (std::size_t deg = 0; deg < max_degree; ++deg) {
//             for (std::size_t j = 0; j < poly_matrix.columns(); ++j) {
//                 ret(i, ret_column++) = mat(i, deg + j * max_degree);
//             }
//         }
//     }
//     return ret;
// }

template <typename T>
auto to_coeff_matrix(const PolynomialMatrix<T> &poly_matrix)
{
    const auto rows = poly_matrix.rows();
    const auto cols = poly_matrix.columns();
    if (rows == 1 || cols == 1)
        return to_matrix(poly_matrix);
    const auto max_size = max(poly_matrix).degree() + 1;
    Matrix<T> ret(rows, max_size * cols);

    for (std::size_t i = 0; i < rows; ++i) {
        auto it_ret = ret.begin(i);
        for (std::size_t k = 0; k < max_size; ++k) {
            for (auto it = poly_matrix.cbegin(i); it != poly_matrix.cend(i);
                 ++it) {
                *it_ret++ = (it->size() > k) ? (*it)[k] : T(0);
            }
        }
    }
    return ret;
}

} // namespace poly
#endif // POLYNOMIAL_MATRIX_H
