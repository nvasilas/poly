#ifndef POLYNOMIAL_MATRIX_H
#define POLYNOMIAL_MATRIX_H

#include <algorithm>
#include <blaze/Blaze.h>

#include "polynomial.h"

namespace poly {

template <typename T>
using PolynomialMatrix = typename blaze::DynamicMatrix<poly::Polynomial<T>>;

template <typename T> using Matrix = typename blaze::DynamicMatrix<T>;

template <typename T> auto to_matrix(const PolynomialMatrix<T> &other)
{
    const auto rows = other.rows();
    const auto max_col = max(other).degree() + 1;
    Matrix<T> result(rows, max_col * other.columns(), T(0));

    for (std::size_t i = 0; i < rows; ++i) {
        auto position = result.begin(i);
        for (auto it = other.cbegin(i); it != other.cend(i); ++it) {
            std::copy(it->data().cbegin(), it->data().cend(), position);
            std::advance(position, max_col);
        }
    }
    return result;
}

template <typename T> auto to_coeff_matrix(const PolynomialMatrix<T> &other)
{
    if (other.rows() == 1 || other.columns() == 1)
        return to_matrix(other);
    const auto mat = to_matrix(other);
    Matrix<T> result(mat.rows(), mat.columns());

    // Matrix<int> in{
    //     {1, 0, -7, 6, 0, 0, 0, 0},
    //     {1, -1, -4, 4, 1, 0, 0, 0},
    //     {1, 5, 6, 0, 0, 0, 0, 0}
    // };
    // Matrix<int> out{
    //     {1, 0, 0, 0, -7, 0, 6, 0},
    //     {1, 1, -1, 0, -4, 0, 4, 0},
    //     {1, 0, 5, 0, 6, 0, 0, 0}
    // };

    // result: 0, 1 | 2, 3 | 4, 5 | 6, 7
    // mat:    0, 4 | 1, 5 | 2, 6 | 3, 7 -> j, j + max_col

    const auto max_col = max(other).degree() + 1;
    for (std::size_t i = 0; i < mat.rows(); ++i) {
        for (std::size_t k = 0, j = 0; j < max_col; k += 2, ++j) {
            result(i, k) = mat(i, j);
            result(i, k + 1) = mat(i, j + max_col);
            // if (i == 1) {
            //     std::cout << "k, k+1 = " << k << ", " << k+1 <<
            //     " j, j+max_col = " << j << " ," << j+max_col << std::endl;
            // }
        }
    }
    return result;
}

} // namespace poly
#endif // POLYNOMIAL_MATRIX_H
