#include "../src/matrix_fraction_description.h"
#include "gtest/gtest.h"

using namespace poly;

const Polynomial<int> p1({1, 4, -3, 2});
const Polynomial<int> p2({-1, 2});
const auto p3 = p1 + p2;
const Polynomial<int> p4({3, -5, 0, 1, 6});

const PolynomialFraction<int> f1(p1, p2);
const PolynomialFraction<int> f2(p3, p4);

TEST(TestPolynomialFractionMatrix, constructor_rows_cols_size)
{
    PolynomialFractionMatrix<int> m1{{f1, f2}, {f2, f1}};
    ASSERT_EQ(m1.rows(), 2);
    ASSERT_EQ(m1.columns(), 2);
    ASSERT_EQ(size(m1), 4);

    PolynomialFractionMatrix<int> m2{{f1, f2, f2, f1}};
    ASSERT_EQ(m2.rows(), 1);
    ASSERT_EQ(m2.columns(), 4);
    ASSERT_EQ(size(m2), 4);
}

TEST(TestPolynomialFractionMatrix, operator_equal)
{
    PolynomialFractionMatrix<int> m1{{f1, f2}, {f2, f1}};
    PolynomialFractionMatrix<int> m2{{f1, f2}, {f2, f1}};
    ASSERT_EQ(m1, m2);
    ASSERT_EQ(m2, m1);
}

TEST(TestPolynomialFractionMatrix, operator_not_equal)
{
    PolynomialFractionMatrix<int> m1{{f1, f2}, {f2, f1}};
    PolynomialFractionMatrix<int> m2{{f2, f2}, {f2, f1}};
    ASSERT_NE(m1, m2);
    ASSERT_NE(m2, m1);

    PolynomialFractionMatrix<int> m3{{f1, f2}, {f2, f1}};
    PolynomialFractionMatrix<int> m4{{f1, f2, f2, f1}};
    ASSERT_NE(m3, m4);
}

TEST(TestPolynomialFractionMatrix, operator_plus)
{
    PolynomialFractionMatrix<int> m{{f1, f2}, {f2, f1}};
    PolynomialFractionMatrix<int> expected{{2 * f1, 2 * f2}, {2 * f2, 2 * f1}};
    ASSERT_EQ(m + m, expected);
}

TEST(TestPolynomialFractionMatrix, operator_compound_plus)
{
    PolynomialFractionMatrix<int> m{{f1, f2}, {f2, f1}};
    m += m;
    PolynomialFractionMatrix<int> expected{{2 * f1, 2 * f2}, {2 * f2, 2 * f1}};
    ASSERT_EQ(m, expected);
}

TEST(TestPolynomialFractionMatrix, operator_minus)
{
    PolynomialFractionMatrix<int> m1{{2 * f1, 2 * f2}, {2 * f2, 2 * f1}};
    PolynomialFractionMatrix<int> m2{{f1, f2}, {f2, f1}};
    PolynomialFractionMatrix<int> expected{{f1, f2}, {f2, f1}};
    ASSERT_EQ(m1 - m2, expected);
}

TEST(TestPolynomialFractionMatrix, operator_compound_minus)
{
    PolynomialFractionMatrix<int> m1{{2 * f1, 2 * f2}, {2 * f2, 2 * f1}};
    PolynomialFractionMatrix<int> m2{{f1, f2}, {f2, f1}};
    PolynomialFractionMatrix<int> expected{{f1, f2}, {f2, f1}};
    m1 -= m2;
    ASSERT_EQ(m1, expected);
}

TEST(TestPolynomialFractionMatrix, operator_multiply_constant)
{
    const PolynomialFractionMatrix<int> m{{f1, f2}, {f2, f1}};
    PolynomialFractionMatrix<int> expected{{2 * f1, 2 * f2}, {2 * f2, 2 * f1}};
    ASSERT_EQ(m * 2, expected);
}

TEST(TestPolynomialFractionMatrix, operator_compound_multiply_constant)
{
    PolynomialFractionMatrix<int> m{{f1, f2}, {f2, f1}};
    PolynomialFractionMatrix<int> expected{{2 * f1, 2 * f2}, {2 * f2, 2 * f1}};
    m *= 2;
    ASSERT_EQ(m, expected);
}

TEST(TestPolynomialFractionMatrix, operator_get_index_by_current_row_and_col)
{
    PolynomialFractionMatrix<int> m{{f1, f2}, {f2, f1}};
    ASSERT_EQ(m(0, 0), f1);
    ASSERT_EQ(m(0, 1), f2);
    ASSERT_EQ(m(1, 0), f2);
    ASSERT_EQ(m(1, 1), f1);
}

TEST(TestPolynomialFractionMatrix, operator_multiply_2x2)
{
    PolynomialFractionMatrix<int> m1{{f1, f2}, {f2, f1}};
    auto m2 = m1;
    PolynomialFractionMatrix<int> expected{
        {f1 * f1 + f2 * f2, f1 * f2 + f2 * f1},
        {f2 * f1 + f1 * f2, f2 * f2 + f1 * f1}};
    ASSERT_EQ(m1 * m2, expected);
    ASSERT_EQ(m2 * m1, expected);
}

TEST(TestPolynomialFractionMatrix, operator_multiply_1x2_2x1)
{
    PolynomialFractionMatrix<int> m1{{f1, f2}};
    PolynomialFractionMatrix<int> m2{{f2}, {f1}};
    PolynomialFractionMatrix<int> expected{{f1 * f2 + f2 * f1}};
    ASSERT_EQ(m1 * m2, expected);
}

TEST(TestPolynomialFractionMatrix, get_left)
{
    PolynomialFractionMatrix<int> mf{
        {{{1}, {2, 1}}, {{0}, {1}}},
        {{{1}, {3, 1}}, {{1}, {-3, 1}}},
        {{{1}, {2, -3, 1}}, {{0}, {1}}},
    };
    auto [Nl, Dl] = get_left(mf);

    PolynomialMatrix<int> expected_Nl{
        {{1}, {0}},
        {{-3, 1}, {3, 1}},
        {{1}, {0}},
    };
    PolynomialMatrix<int> expected_Dl{
        {{2, 1}, {0}, {0}},
        {{0}, {-9, 0, 1}, {0}},
        {{0}, {0}, {2, -3, 1}},
    };

    ASSERT_EQ(Nl, expected_Nl);
    ASSERT_EQ(Dl, expected_Dl);
}
