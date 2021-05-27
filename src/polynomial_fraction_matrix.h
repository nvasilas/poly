#ifndef POLYNOMIAL_FRACTION_MATRIX_H
#define POLYNOMIAL_FRACTION_MATRIX_H

#include <Eigen/Core>
#include <utility>

#include "polynomial_fraction.h"
#include "polynomial_matrix.h"

namespace Eigen
{
template <>
struct NumTraits<poly::PolynomialFraction<double>>
    : GenericNumTraits<poly::PolynomialFraction<double>> {
    using Real = double;
};
} // namespace Eigen

namespace poly
{
using PolynomialFractionMatrix =
    Eigen::Matrix<poly::PolynomialFraction<double>, Eigen::Dynamic,
                  Eigen::Dynamic, Eigen::RowMajor>;

std::pair<PolynomialMatrix<double>, PolynomialMatrix<double>>
get_left(const PolynomialFractionMatrix &);

} // namespace poly
#endif // POLYNOMIAL_FRACTION_MATRIX_H
