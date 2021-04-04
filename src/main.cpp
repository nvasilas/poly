// #include "matrix_fraction_description.h"
#include "polynomial.h"
#include "polynomial_fraction.h"
#include "polynomial_matrix.h"

#include <iostream>

using namespace poly;

int main()
{
    // PolynomialMatrix<int> m{{{1, 0, -7, 6}, {0}, {1, 2}},
    //                         {{1, -1, -4, 4}, {1, 0}, {1}},
    //                         {{1, 5, 6}, {0}, {0}}};
    PolynomialMatrix<int> m{
        {{1, 0, -7, 6}, {0}},
        {{1, -1, -4, 4}, {1, 0}},
        {{1, 5, 6}, {0}}
    };
    // auto mat = to_matrix(m);
    // std::cout << mat << std::endl;

    auto mat_coeff = to_coeff_matrix(m);
    std::cout << mat_coeff << std::endl;
    return 0;
}
