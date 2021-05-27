#include "polynomial_fraction_matrix.h"

namespace poly
{
auto num_den(const PolynomialFractionMatrix &mf)
{
    const auto rows = mf.rows();
    const auto cols = mf.cols();
    PolynomialMatrix<double> num(rows, cols);
    PolynomialMatrix<double> den(rows, cols);

    for (std::size_t i = 0; i < rows; ++i) {
        for (std::size_t j = 0; j < cols; ++j) {
            num(i, j) = mf(i, j).num();
            den(i, j) = mf(i, j).den();
        }
    }
    return std::make_pair(num, den);
}

std::pair<PolynomialMatrix<double>, PolynomialMatrix<double>>
get_left(const PolynomialFractionMatrix &mf)
{
    const auto rows = mf.rows();
    const auto cols = mf.cols();
    auto [Nl, den] = num_den(mf);
    auto Dl = PolynomialMatrix<double>(rows, rows);

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
} // namespace poly
