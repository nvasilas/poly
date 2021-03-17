#include "polynomial.h"
#include "polynomial_matrix.h"

#include <iostream>
#include <vector>

int main()
{
    // Create a third degree polynomial: 1x^3 + 4x^2 - 3x + 2
    Polynomial<int> p1({ 1, 4, -3, 2, 2 });
    Polynomial<int> p2({ -1, 2 });
    auto p3 = p1 + p2;
    // std::cout << p3 << std::endl;
    // auto p3 = p1 - p2;// p3({2, 2, -3, 2});
    // Polynomial<int> p3({2, 2, -3, 2});
    // auto p3 = p1 - p2;// p3({2, 2, -3, 2});

    // std::cout << p1(2) << std::endl;
    // std::cout << p2(2) << std::endl;
    // std::cout << p3(2) << std::endl;

    PolynomialMatrix<int> m1(2, { p1, p3, p2, p1 });
    PolynomialMatrix<int> m2(2, { p1, p3, p2, p1 });
    auto m3 = m1 * m2;

    PolynomialMatrix<int> m(3, 2, {
                            {1, 0, -7, 6}, {0, 0},
                            {1, -1, -4, 4}, {1, 0},
                            {0, 1, 5, 6}, {0, 0}
                            });
    std::cout << m << std::endl;

    return 0;
}
