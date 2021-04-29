#ifndef MATRIX_FRACTION_DESCRIPTION_H
#define MATRIX_FRACTION_DESCRIPTION_H

#include <algorithm>
#include <blaze/Blaze.h>
#include <iterator>

#include "polynomial_fraction.h"
#include "polynomial_matrix.h"

namespace poly {

template <typename T>
using PolynomialFractionMatrix =
    typename blaze::DynamicMatrix<poly::PolynomialFraction<T>>;

template <typename T> auto num_den(const PolynomialFractionMatrix<T> &mf)
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

template <typename T> auto get_left(const PolynomialFractionMatrix<T> &mf)
{
    const auto rows = mf.rows();
    const auto cols = mf.columns();
    auto [Nl, den] = num_den(mf);
    auto Dl = PolynomialMatrix<T>(rows, rows);
    blaze::decldiag(Dl);

    for (std::size_t i = 0; i < rows; ++i) {
        Dl(i, i) = den(i, 0);
        for (std::size_t j = 0; j < cols; ++j) {
            if (j > 0) {
                Dl(i, j) *= den(i, j);
            }
            for (std::size_t k = 0; k < cols; ++k) {
                if (k != j) {
                    Nl(i, j) *= den(i, k);
                }
            }
        }
    }
    // Nl.remove_trailing_zeros();
    // Dl.remove_trailing_zeros();
    return std::make_pair(std::move(Nl), std::move(Dl));
}

template <typename T> auto get_F(const PolynomialFractionMatrix<T> &mf)
{
    auto [Nl, Dl] = get_left(mf);
    const auto Nl_T =
        to_coeff_matrix(static_cast<decltype(Nl)>(trans(std::move(Nl))));
    const auto Dl_T =
        to_coeff_matrix(static_cast<decltype(Dl)>(trans(std::move(Dl))));

    // ret = [ Dl_T;
    //         Nl_T ]
    Matrix<T> ret{/*matrix*/ {Nl_T.rows() + Dl_T.rows(),
                              std::max(Nl_T.columns(), Dl_T.columns()), T(0)},
                  /*deg*/ std::max(Nl_T.deg, Dl_T.deg)};

    for (std::size_t i = 0; i < Dl_T.rows(); ++i) {
        for (std::size_t j = 0; j < Dl_T.columns(); ++j) {
            ret(i, j) = Dl_T(i, j);
        }
    }
    for (std::size_t i = 0; i < Nl_T.rows(); ++i) {
        for (std::size_t j = 0; j < Nl_T.columns(); ++j) {
            ret(i + Dl_T.rows(), j) = Nl_T(i, j);
        }
    }

    return ret;
}

template <typename T> void prm(const PolynomialFractionMatrix<T> &mf)
{
    auto F = get_F(mf);
    // std::cout << F << std::endl;

    auto S = F;
    blaze::DynamicVector<T> pr;
    auto sel = blaze::uniform(F.rows(), 1);

    const auto Fbcols = F.coef_columns();
    blaze::DynamicVector<int> abar;
    abar = blaze::generate(
        F.rows() - (Fbcols + 1) + 1,
        [Fbcols](std::size_t index) { return Fbcols + 1 + index; });

    auto shft = 0;

    Matrix<T> T1{/*matrix*/ {},
                 /*deg*/ 1};

    if (blaze::isEmpty(T1.matrix))
        std::cout << "empty T1\n";
    else
        std::cout << T1 << std::endl;

    while (blaze::isEmpty(T1.matrix) ||
           T1.rows() < F.rows() - F.coef_columns()) {
        auto Srows = F.rows() * T1.coef_columns();
        // [T1,T1rows,sel,pr] = ...
        //     t1calc(S, Srows, T1, T1rows, sel, pr, F.rows(), Fbcols, abar,
        //     gap);
        break; // FIXME
    }
}

} // namespace poly
#endif // MATRIX_FRACTION_DESCRIPTION_H
