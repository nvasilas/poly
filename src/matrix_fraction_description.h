#ifndef MATRIX_FRACTION_DESCRIPTION_H
#define MATRIX_FRACTION_DESCRIPTION_H

#include <Eigen/Dense>
#include <algorithm>
#include <iterator>

#include "polynomial_fraction.h"
#include "polynomial_matrix.h"

namespace Eigen {
template <typename T>
struct NumTraits<poly::PolynomialFraction<T>>
    : GenericNumTraits<poly::PolynomialFraction<T>>
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
using PolynomialFractionMatrix =
    typename Eigen::Matrix<poly::PolynomialFraction<T>, Eigen::Dynamic,
                           Eigen::Dynamic, Eigen::RowMajor>;

template <typename T> auto num_den(const PolynomialFractionMatrix<T> &mf)
{
    const auto rows = mf.rows();
    const auto cols = mf.cols();
    PolynomialMatrix<T> num(rows, cols);
    PolynomialMatrix<T> den(rows, cols);

    for (std::size_t i = 0; i < rows; ++i) {
        for (std::size_t j = 0; j < cols; ++j) {
            num(i, j) = mf(i, j).num();
            den(i, j) = mf(i, j).den();
        }
    }
    return std::make_pair(num, den);
}

template <typename T> auto get_left(const PolynomialFractionMatrix<T> &mf)
{
    const auto rows = mf.rows();
    const auto cols = mf.cols();
    auto [Nl, den] = num_den(mf);
    auto Dl = PolynomialMatrix<T>(rows, rows);

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
    Nl.transposeInPlace();
    Dl.transposeInPlace();
    const auto Nl_T = to_coeff_matrix(Nl);
    const auto Dl_T = to_coeff_matrix(Dl);

    Matrix<T> ret{/*matrix*/ {Nl_T.rows() + Dl_T.rows(),
                              std::max(Nl_T.cols(), Dl_T.cols())},
                  /*deg*/ std::max(Nl_T.deg, Dl_T.deg)};

    // ret = [ Dl_T;
    //         Nl_T ]
    ret.matrix.topLeftCorner(Dl_T.rows(), Dl_T.cols()) = Dl_T.matrix;
    ret.matrix.bottomLeftCorner(Nl_T.rows(), Nl_T.cols()) = Nl_T.matrix;

    return ret;
}

#if 0
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
#endif

} // namespace poly
#endif // MATRIX_FRACTION_DESCRIPTION_H
