#include "matrix_fraction_description.h"
#include "polynomial.h"
#include "polynomial_fraction.h"
#include "polynomial_matrix.h"

#include <iostream>
#include <vector>

using namespace poly;

int main()
{

    // const Polynomial<int> N = {9, 4, 7, 2};
    // const Polynomial<int> D = {1, 0, 1};
    // PolynomialFraction<int> f1(N, D);
    PolynomialFraction<int> f1({9, 4, 7, 2}, {1, 0, 1});
    // std::cout << f1 << std::endl;
    // clang-format off
    PolynomialFractionMatrix<int> mf(3, 2, {
                                     {{1}, {2, 1}}, {{0}, {1}},
                                     {{1}, {3, 1}}, {{1}, {-3, 1}},
                                     {{1}, {2, -3, 1}}, {{0}, {1}},
                                     });
    // clang-format on
    auto [Nl, Dl] = mf.get_left();
    std::cout << Nl << std::endl;
    std::cout << std::endl;
    std::cout << Dl << std::endl;

    // PolynomialMatrix<int> N(3, 2, {{1}, {0}, {1}, {1}, {1}, {0}});
    // PolynomialMatrix<int> D(3, 2,
    //                         {{2, 1}, {1}, {3, 1}, {-3, 1}, {2, -3, 1}, {1}});

    // Create a third degree polynomial: 1x^3 + 4x^2 - 3x + 2
    // Polynomial<int> p1({ 1, 4, -3, 2, 2 });
    // Polynomial<int> p2({ -1, 2 });
    // auto p3 = p1 + p2;
    // std::cout << p3 << std::endl;
    // auto p3 = p1 - p2;// p3({2, 2, -3, 2});
    // Polynomial<int> p3({2, 2, -3, 2});
    // auto p3 = p1 - p2;// p3({2, 2, -3, 2});

    // std::cout << p1(2) << std::endl;
    // std::cout << p2(2) << std::endl;
    // std::cout << p3(2) << std::endl;

    // PolynomialMatrix<int> m1(2, { p1, p3, p2, p1 });
    // PolynomialMatrix<int> m2(2, { p1, p3, p2, p1 });
    // auto m3 = m1 * m2;

    // PolynomialMatrix<int> m(3, 2, {
    //                         {1, 0, -7, 6}, {0},
    //                         {1, -1, -4, 4}, {1, 0},
    //                         {1, 5, 6}, {0}
    //                         });
    // auto mat = m.to_matrix();
    // for (auto i : mat)
    //     std::cout << i << std::endl;
    // auto mat_coeff = m.to_coeff_matrix();
    // for (auto i : mat_coeff)
    //     std::cout << i << std::endl;

    return 0;
}
