#include "../src/polynomial.h"
#include "gtest/gtest.h"

using namespace poly;

TEST(TestPolynomial, operator_equal_int)
{
    Polynomial<int> p1({1, 4});
    Polynomial<int> p2({1, 4});
    ASSERT_EQ(p1, p2);
    ASSERT_EQ(p2, p1);
}

TEST(TestPolynomial, operator_equal_double)
{
    Polynomial<double> p1({1.000000000001, 4});
    Polynomial<double> p2({1, 4});
    ASSERT_EQ(p1, p2);

    Polynomial<double> p3({1.0001, 4});
    ASSERT_NE(p2, p3);
}

TEST(TestPolynomial, operator_not_equal)
{
    Polynomial<int> p1({1, 4});
    Polynomial<int> p2({1, 1});
    ASSERT_NE(p1, p2);
    ASSERT_NE(p2, p1);
}

TEST(TestPolynomial, operator_eval)
{
    Polynomial<int> p({1, 4, -3, 2});
    ASSERT_EQ(p(0), 1);
    ASSERT_EQ(p(3), 40);
    ASSERT_EQ(p(10), 1741);
}

TEST(TestPolynomial, operator_plus)
{
    Polynomial<int> p1({1, 4, -3, 2});
    Polynomial<int> p2({-1, 2});
    Polynomial<int> expected({0, 6, -3, 2});
    ASSERT_EQ(p1 + p2, expected);
    ASSERT_EQ(expected, p2 + p1);
}

TEST(TestPolynomial, operator_compound_plus)
{
    Polynomial<int> p1({1, 4, -3, 2});
    Polynomial<int> p2({-1, 2});
    p1 += p2;
    Polynomial<int> expected({0, 6, -3, 2});
    ASSERT_EQ(p1, expected);
}

TEST(TestPolynomial, operator_minus)
{
    Polynomial<int> p1({1, 4, -3, 2});
    Polynomial<int> p2({-1, 2});
    Polynomial<int> expected({2, 2, -3, 2});
    ASSERT_EQ(p1 - p2, expected);

    Polynomial<int> p3({-1, 2});
    Polynomial<int> p4({1, 4, -3, 2});
    Polynomial<int> expected2({-2, -2, 3, -2});
    ASSERT_EQ(p3 - p4, expected2);
}

TEST(TestPolynomial, operator_compound_minus)
{
    Polynomial<int> p1({1, 4, -3, 2});
    Polynomial<int> p2({-1, 2});
    p1 -= p2;
    Polynomial<int> expected({2, 2, -3, 2});
    ASSERT_EQ(p1, expected);

    Polynomial<int> p3({-1, 2});
    Polynomial<int> p4({1, 4, -3, 2});
    p3 -= p4;
    Polynomial<int> expected2({-2, -2, 3, -2});
    ASSERT_EQ(p3, expected2);
}

TEST(TestPolynomial, operator_multiply_poly)
{
    Polynomial<int> p1({1, 4, -3, 2});
    Polynomial<int> p2({-1, 2});
    Polynomial<int> expected({-1, -2, 11, -8, 4});
    ASSERT_EQ(p1 * p2, expected);
    ASSERT_EQ(expected, p2 * p1);
}

TEST(TestPolynomial, operator_multiply_poly_constant_poly)
{
    Polynomial<int> p1({1, 4, -3, 2});
    Polynomial<int> p2({-1});
    Polynomial<int> expected({-1, -4, 3, -2});
    ASSERT_EQ(p1 * p2, expected);
    ASSERT_EQ(expected, p2 * p1);
}

TEST(TestPolynomial, operator_compound_multiply_poly)
{
    Polynomial<int> p1({1, 4, -3, 2});
    Polynomial<int> p2({-1, 2});
    p1 *= p2;
    Polynomial<int> expected({-1, -2, 11, -8, 4});
    ASSERT_EQ(p1, expected);
    ASSERT_EQ(expected, p1);
}

TEST(TestPolynomial, operator_compound_multiply_poly_constant_poly)
{
    Polynomial<int> p1({1, 4, -3, 2});
    Polynomial<int> p2({-1});
    p1 *= p2;
    Polynomial<int> expected({-1, -4, 3, -2});
    ASSERT_EQ(p1, expected);
    ASSERT_EQ(expected, p1);
}

TEST(TestPolynomial, operator_multiply_constant)
{
    Polynomial<int> p({1, 4, -3, 2});
    Polynomial<int> expected({2, 8, -6, 4});
    ASSERT_EQ(p * 2, expected);
    ASSERT_EQ(expected, 2 * p);
}

TEST(TestPolynomial, operator_compound_multiply_constant)
{
    Polynomial<int> p({1, 4, -3, 2});
    Polynomial<int> expected({2, 8, -6, 4});
    p *= 2;
    ASSERT_EQ(p, expected);
}

TEST(TestPolynomial, operator_get_index)
{
    Polynomial<int> p({1, 4, -3, 2});
    ASSERT_EQ(p[0], 1);
    ASSERT_EQ(-3, p[2]);
}

TEST(TestPolynomial, test_degree)
{
    Polynomial<int> p({1, 4, -3, 2});
    ASSERT_EQ(p.degree(), 3);
}

TEST(TestPolynomial, divide)
{
    Polynomial<double> N = {9, 4, 7, 2};
    Polynomial<double> D = {1, 0, 1};
    auto [quotient, remainder] = divide(N, D);
    ASSERT_EQ(D * quotient + remainder, N);
}

TEST(TestPolynomial, operator_divide_remainder)
{
    Polynomial<double> N = {9, 4, 7, 2};
    Polynomial<double> D = {1, 0, 1};
    Polynomial<double> remainder({2, 2});
    ASSERT_EQ(N / D, remainder);
}

TEST(TestPolynomial, operator_divide_quotient)
{
    Polynomial<double> N = {9, 4, 7, 2};
    Polynomial<double> D = {1, 0, 1};
    Polynomial<double> quotient({7, 2});
    ASSERT_EQ(N % D, quotient);
}
