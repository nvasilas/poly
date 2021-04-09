#include "polynomial.h"
#include "polynomial_fraction.h"
#include "polynomial_matrix.h"
#include "matrix_fraction_description.h"

#include <iostream>

using namespace poly;

int main()
{
    const Polynomial<int> p1({1, 4});
    const Polynomial<int> p2({1, 4});
    const PolynomialFraction<int> f1(p1, p2);
    const PolynomialFraction<int> f2(p1, p2);

    PolynomialFractionMatrix<int> mfd1{{f1, f2}, {f2, f1}};
    PolynomialFractionMatrix<int> mfd2{{f2, f1}, {f1, f2}};

    PolynomialFractionMatrix<int> mf{
        { {{1}, {2, 1}}, {{0}, {1}} },
        { {{1}, {3, 1}}, {{1}, {-3, 1}} },
        { {{1}, {2, -3, 1}}, {{0}, {1}} },
    };
    std::cout << mf << std::endl;
    auto left_right = get_left(mf);
    std::cout << left_right.first << std::endl;
    std::cout << left_right.second << std::endl;

    // std::cout << mfd << std::endl;
    // std::cout << mfd2 << std::endl;

    // PolynomialMatrix<int> m{{{1, 0, -7, 6}, {0}, {1, 2}},
    //                         {{1, -1, -4, 4}, {1, 0}, {1}},
    //                         {{1, 5, 6}, {0}, {0}}};
    // PolynomialMatrix<int> m{
    //     {{1, 0, -7, 6}, {0}},
    //     {{1, -1, -4, 4}, {1, 0}},
    //     {{1, 5, 6}, {0}}
    // };
    // auto mat = to_matrix(m);
    // std::cout << mat << std::endl;

    // auto mat_coeff = to_coeff_matrix(m);
    // std::cout << mat_coeff << std::endl;

    return 0;
}
