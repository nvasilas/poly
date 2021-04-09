#ifndef MATRIX_FRACTION_DESCRIPTION_H
#define MATRIX_FRACTION_DESCRIPTION_H

#include <algorithm>
#include <vector>

#include "polynomial_fraction.h"
#include "polynomial_matrix.h"

namespace poly {

template <typename T>
using PolynomialFractionMatrix =
    typename blaze::DynamicMatrix<poly::PolynomialFraction<T>>;

template <typename T>
auto num_den(const PolynomialFractionMatrix<T> &mf)
{
    const auto rows = mf.rows();
    const auto cols = mf.columns();
    PolynomialMatrix<T> num(rows, cols);
    PolynomialMatrix<T> den(rows, cols);
    for (std::size_t i = 0; i < rows; ++i) {
        auto it_num = num.begin(i);
        auto it_den = den.begin(i);
        for (auto it = mf.cbegin(i); it != mf.cend(i); ++it) {
            *it_num++ = it->num();
            *it_den++ = it->den();
        }
    }
    return std::make_pair(num, den);
}

template <typename T>
auto get_left(const PolynomialFractionMatrix<T> &mf)
{
    const auto rows = mf.rows();
    const auto cols = mf.columns();
    auto [Nl, den] = num_den(mf);
    auto Dl = PolynomialMatrix<T>(rows, rows);

    auto is_new_row = [cols](auto i) { return i % cols == 0; };
    // TODO

    // std::size_t diag_idx = 0;
    // std::size_t same_row_idx = 0;
    // for (std::size_t i = 0; i < size(mf); ++i) {
    //     if (is_new_row(i)) {
    //         if (i) {
    //             ++diag_idx;
    //             same_row_idx += cols;
    //         }
    //         Dl(diag_idx, diag_idx) = den[i];
    //     } else {
    //         Dl(diag_idx, diag_idx) *= den[i];
    //     }
    //     for (std::size_t j = same_row_idx; j < same_row_idx + cols; ++j) {
    //         if (i != j)
    //             Nl[i] *= den[j];
    //     }
    // }
    // // Nl.remove_trailing_zeros();
    // // Dl.remove_trailing_zeros();
    return std::make_pair(std::move(Nl), std::move(Dl));
}

} // namespace poly
#endif // MATRIX_FRACTION_DESCRIPTION_H
