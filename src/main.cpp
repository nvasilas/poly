#include "matrix_fraction_description.h"
#include "polynomial.h"
#include "polynomial_fraction.h"
#include "polynomial_matrix.h"

#include <iostream>
#include <math.h>
#include <ostream>

using namespace poly;

auto make_polynomial_fraction_matrix()
{
    const PolynomialFraction<int> f11{{1}, {2, 1}};
    const PolynomialFraction<int> f12{{0}, {1}};
    const PolynomialFraction<int> f21{{1}, {3, 1}};
    const PolynomialFraction<int> f22{{1}, {-3, 1}};
    const PolynomialFraction<int> f31{{1}, {2, -3, 1}};
    const PolynomialFraction<int> f32{{0}, {1}};
    return (PolynomialFractionMatrix<int>(3, 2) << f11, f12, f21, f22, f32, f32)
        .finished();
}

int main()
{
    auto mf = make_polynomial_fraction_matrix();
    auto F = get_F(mf);
    std::cout << F << std::endl;

    // prm(mf);

    // using P = Polynomial<int>;
    // auto m1 = (PolynomialMatrix<int>(3, 2) <<
    //            P{1, 0, -7, 6}, P{0},
    //            P{1, -1, -4, 4}, P{1, 0},
    //            P{1, 5, 6}, P{0})
    // .finished();
    // auto mat_coeff = to_coeff_matrix(m1);

    // Matrix<int> expected1{
    //     /*matrix*/
    //     {(Matrix<int>::EigenMatrix(3, 8) << 1, 0, 0, 0, -7, 0, 6, 0, /* 0 row
    //     */
    //       1, 1, -1, 0, -4, 0, 4, 0,                                  /* 1 row
    //       */ 1, 0, 5, 0, 6, 0, 0, 0 /* 2 row */)
    //          .finished()},
    //     /*deg*/ 4};

    return 0;
}
