#include "../src/polynomial_fraction.h"
#include "gtest/gtest.h"

using namespace poly;

TEST(TestPolynomialFraction, operator_equal_int)
{
    Polynomial<int> p1({1, 4});
    Polynomial<int> p2({1, 4});
    PolynomialFraction<int> f1(p1, p2);
    PolynomialFraction<int> f2(p1, p2);
    ASSERT_EQ(f1, f2);
    ASSERT_EQ(f2, f1);
}

TEST(TestPolynomialFraction, operator_equal_double)
{
    Polynomial<double> p1({1.000000000001, 4});
    Polynomial<double> p2({1, 4});
    PolynomialFraction<double> f1(p1, p2);
    PolynomialFraction<double> f2(p1, p2);
    ASSERT_EQ(f1, f2);

    Polynomial<double> p3({1.0001, 4});
    PolynomialFraction<double> f3(p1, p3);
    ASSERT_NE(f2, f3);
}

TEST(TestPolynomialFraction, operator_not_equal)
{
    Polynomial<int> p1({1, 4});
    Polynomial<int> p2({1, 1});
    PolynomialFraction<int> f1(p1, p2);
    PolynomialFraction<int> f2(p2, p2);
    ASSERT_NE(f1, f2);
    ASSERT_NE(f2, f1);
}

TEST(TestPolynomialFraction, operator_eval)
{
    Polynomial<double> p1({1, 4, -3, 2});
    Polynomial<double> p2({1, 4, 2});
    PolynomialFraction<double> f(p1, p2);
    ASSERT_EQ(f(0.), 1.);
    ASSERT_EQ(f(3.), 40. / 31.);
    ASSERT_EQ(f(10.), 1741. / 241.);
}

TEST(TestPolynomialFraction, operator_plus)
{
    PolynomialFraction<int> f1({1, 4, -3, 2}, {0, 2, 3});
    PolynomialFraction<int> f2({-1, 2}, {1, 1, 5});

    PolynomialFraction<int> expected({0, 6, -3, 2}, {1, 3, 8});
    ASSERT_EQ(f1 + f2, expected);
    ASSERT_EQ(expected, f2 + f1);
}

TEST(TestPolynomialFraction, operator_compound_plus)
{
    PolynomialFraction<int> f1({1, 4, -3, 2}, {0, 2, 3});
    PolynomialFraction<int> f2({-1, 2}, {1, 1, 5});
    f1 += f2;
    PolynomialFraction<int> expected({0, 6, -3, 2}, {1, 3, 8});
    ASSERT_EQ(f1, expected);
}

TEST(TestPolynomialFraction, operator_minus)
{
    PolynomialFraction<int> f1({1, 4, -3, 2}, {0, 2, 3});
    PolynomialFraction<int> f2({-1, 2}, {1, 1, 5});
    PolynomialFraction<int> expected({2, 2, -3, 2}, {-1, 1, -2});
    ASSERT_EQ(f1 - f2, expected);

    PolynomialFraction<int> f3({-1, 2}, {0, 2, 3});
    PolynomialFraction<int> f4({1, 4, -3, 2}, {1, 1, 5});
    PolynomialFraction<int> expected2({-2, -2, 3, -2}, {-1, 1, -2});
    ASSERT_EQ(f3 - f4, expected2);
}

TEST(TestPolynomialFraction, operator_compound_minus)
{
    PolynomialFraction<int> f1({1, 4, -3, 2}, {0, 2, 3});
    PolynomialFraction<int> f2({-1, 2}, {1, 1, 5});
    f1 -= f2;
    PolynomialFraction<int> expected({2, 2, -3, 2}, {-1, 1, -2});
    ASSERT_EQ(f1, expected);

    PolynomialFraction<int> f3({-1, 2}, {0, 2, 3});
    PolynomialFraction<int> f4({1, 4, -3, 2}, {1, 1, 5});
    f3 -= f4;
    PolynomialFraction<int> expected2({-2, -2, 3, -2}, {-1, 1, -2});
    ASSERT_EQ(f3, expected2);
}

TEST(TestPolynomialFraction, operator_multiply_poly_frac)
{
    PolynomialFraction<int> f1({1, 4, -3, 2}, {0, 2, 3});
    PolynomialFraction<int> f2({-1, 2}, {1, 1, 5});
    PolynomialFraction<int> expected({-1, -2, 11, -8, 4}, {0, 2, 5, 13, 15});
    ASSERT_EQ(f1 * f2, expected);
    ASSERT_EQ(expected, f1 * f2);
}

TEST(TestPolynomialFraction, operator_multiply_poly_constant_poly_frac)
{
    PolynomialFraction<int> f1({1, 4, -3, 2}, {0, 2, 3});
    PolynomialFraction<int> f2({-1}, {-2});
    PolynomialFraction<int> expected({-1, -4, 3, -2}, {0, -4, -6});
    ASSERT_EQ(f1 * f2, expected);
    ASSERT_EQ(expected, f1 * f2);
}

TEST(TestPolynomialFraction, operator_compound_multiply_poly_frac)
{
    PolynomialFraction<int> f1({1, 4, -3, 2}, {0, 2, 3});
    PolynomialFraction<int> f2({-1, 2}, {1, 1, 5});
    f1 *= f2;
    PolynomialFraction<int> expected({-1, -2, 11, -8, 4}, {0, 2, 5, 13, 15});
    ASSERT_EQ(f1, expected);
    ASSERT_EQ(expected, f1);
}

TEST(TestPolynomialFraction,
     operator_compound_multiply_poly_frac_constant_poly_frac)
{
    PolynomialFraction<int> f1({1, 4, -3, 2}, {0, 2, 3});
    PolynomialFraction<int> f2({-1}, {-2});
    f1 *= f2;
    PolynomialFraction<int> expected({-1, -4, 3, -2}, {0, -4, -6});
    ASSERT_EQ(f1, expected);
    ASSERT_EQ(expected, f1);
}

TEST(TestPolynomialFraction, operator_multiply_constant)
{
    PolynomialFraction<int> f({1, 4, -3, 2}, {0, 2, 3});
    PolynomialFraction<int> expected({2, 8, -6, 4}, {0, 4, 6});
    ASSERT_EQ(f * 2, expected);
    ASSERT_EQ(expected, 2 * f);
}

TEST(TestPolynomialFraction, operator_compound_multiply_constant)
{
    PolynomialFraction<int> f({1, 4, -3, 2}, {0, 2, 3});
    PolynomialFraction<int> expected({2, 8, -6, 4}, {0, 4, 6});
    f *= 2;
    ASSERT_EQ(f, expected);
}
