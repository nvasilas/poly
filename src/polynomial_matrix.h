#ifndef POLYNOMIAL_MATRIX_H
#define POLYNOMIAL_MATRIX_H

#include <Eigen/Dense>
#include <algorithm>

#include "polynomial.h"

namespace Eigen {
template <typename T>
struct NumTraits<poly::Polynomial<T>> : GenericNumTraits<poly::Polynomial<T>>
{
    using Real = T;

    // typedef double Real;
    // typedef double NonInteger;
    // typedef double Nested;

    // enum
    // {
    //     IsInteger = 0,
    //     IsSigned = 0,
    //     IsComplex = 0,
    //     RequireInitialization = 1,
    //     ReadCost = 1,
    //     AddCost = 3,
    //     MulCost = 3
    // };
};
} // namespace Eigen

namespace poly {

template <typename T>
using PolynomialMatrix =
    typename Eigen::Matrix<poly::Polynomial<T>, Eigen::Dynamic, Eigen::Dynamic,
                           Eigen::RowMajor>;

template <typename T> struct Matrix
{
    using EigenMatrix =
        typename Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>;

    auto rows() const noexcept { return matrix.rows(); }
    auto cols() const noexcept { return matrix.cols(); }
    auto coef_columns() const noexcept { return matrix.cols() / deg; }

    auto &operator()(int row, int col) { return matrix(row, col); }
    const auto &operator()(int row, int col) const { return matrix(row, col); }

    EigenMatrix matrix;
    std::size_t deg;
};

template <typename T>
inline auto operator==(const Matrix<T> &lhs, const Matrix<T> &rhs)
{
    return lhs.deg == rhs.deg && lhs.matrix == rhs.matrix;
}

template <typename T>
inline auto operator!=(const Matrix<T> &lhs, const Matrix<T> &rhs)
{
    return !(lhs == rhs);
}

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
    const auto cols = poly_matrix.cols();
    // TODO
    // if (rows == 1 || cols == 1)
    //     return to_matrix(poly_matrix);
    const auto max_size = poly_matrix.maxCoeff().size();

    Matrix<T> ret{/*matrix*/ {rows, max_size * cols},
                  /*deg*/ max_size};

    for (auto row = 0; row < rows; ++row) {
        auto col = 0;
        for (auto k = 0; k < max_size; ++k) {
            for (auto j = 0; j < cols; ++j) {
                const auto poly = poly_matrix(row, j);
                ret(row, col++) = (k < poly.size()) ? poly[k] : T(0);
            }
        }
    }
    return ret;
}

} // namespace poly
#endif // POLYNOMIAL_MATRIX_H
