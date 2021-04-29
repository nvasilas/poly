#include "matrix_fraction_description.h"
#include "polynomial.h"
#include "polynomial_fraction.h"
#include "polynomial_matrix.h"

#include <iostream>

using namespace poly;

int main()
{
    // const Polynomial<int> p1({1, 4});
    // const Polynomial<int> p2({1, 4});
    // const PolynomialFraction<int> f1(p1, p2);
    // const PolynomialFraction<int> f2(p1, p2);

    // PolynomialFractionMatrix<int> mfd1{{f1, f2}, {f2, f1}};
    // PolynomialFractionMatrix<int> mfd2{{f2, f1}, {f1, f2}};

    PolynomialFractionMatrix<int> mf{
        {{{1}, {2, 1}}, {{0}, {1}}},
        {{{1}, {3, 1}}, {{1}, {-3, 1}}},
        {{{1}, {2, -3, 1}}, {{0}, {1}}},
    };
    prm(mf);

    return 0;
}
