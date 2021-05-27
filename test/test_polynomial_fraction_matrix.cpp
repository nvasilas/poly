#include "../src/polynomial_fraction_matrix.h"
#include "gtest/gtest.h"

using namespace poly;

static const Polynomial<double> p1({1, 4, -3, 2});
static const Polynomial<double> p2({-1, 2});
static const auto p3 = p1 + p2;
static const Polynomial<double> p4({3, -5, 0, 1, 6});

static const PolynomialFraction<double> f1(p1, p2);
static const PolynomialFraction<double> f2(p3, p4);

TEST(TestPolynomialFractionMatrix, constructor_rows_cols_size)
{
    auto m1 = (PolynomialFractionMatrix(2, 2) << f1, f2, f2, f1).finished();
    ASSERT_EQ(m1.rows(), 2);
    ASSERT_EQ(m1.cols(), 2);
    ASSERT_EQ(m1.size(), 4);

    auto m2 = (PolynomialFractionMatrix(1, 4) << f1, f2, f2, f1).finished();
    ASSERT_EQ(m2.rows(), 1);
    ASSERT_EQ(m2.cols(), 4);
    ASSERT_EQ(m2.size(), 4);
}

TEST(TestPolynomialFractionMatrix, operator_equal)
{
    auto m1 = (PolynomialFractionMatrix(2, 2) << f1, f2, f2, f1).finished();
    auto m2 = (PolynomialFractionMatrix(2, 2) << f1, f2, f2, f1).finished();
    ASSERT_EQ(m1, m2);
    ASSERT_EQ(m2, m1);
}

TEST(TestPolynomialFractionMatrix, operator_not_equal)
{
    auto m1 = (PolynomialFractionMatrix(2, 2) << f1, f2, f2, f1).finished();
    auto m2 = (PolynomialFractionMatrix(2, 2) << f2, f2, f2, f1).finished();
    ASSERT_NE(m1, m2);
    ASSERT_NE(m2, m1);
}

// TODO fix operator*
#if 0
TEST(TestPolynomialFractionMatrix, operator_plus)
{
    auto m = (PolynomialFractionMatrix(2, 2) << f1, f2, f2, f1).finished();
    auto expected =
        (PolynomialFractionMatrix(2, 2) << 2 * f1, 2 * f2, 2 * f2, 2 * f1)
            .finished();
    ASSERT_EQ(m + m, expected);
}

TEST(TestPolynomialFractionMatrix, operator_compound_plus)
{
    auto m = (PolynomialFractionMatrix(2, 2) << f1, f2, f2, f1).finished();
    m += m;
    auto expected =
        (PolynomialFractionMatrix(2, 2) << 2 * f1, 2 * f2, 2 * f2, 2 * f1)
            .finished();
    ASSERT_EQ(m, expected);
}

TEST(TestPolynomialFractionMatrix, operator_minus)
{
    auto m1 = (PolynomialFractionMatrix(2, 2) << 2 * f1, 2 * f2, 2 * f2, 2 * f1)
                  .finished();
    auto m2 = (PolynomialFractionMatrix(2, 2) << f1, f2, f2, f1).finished();
    auto expected =
        (PolynomialFractionMatrix(2, 2) << f1, f2, f2, f1).finished();
    ASSERT_EQ(m1 - m2, expected);
}

TEST(TestPolynomialFractionMatrix, operator_compound_minus)
{
    auto m1 = (PolynomialFractionMatrix(2, 2) << 2 * f1, 2 * f2, 2 * f2, 2 * f1)
                  .finished();
    auto m2 = (PolynomialFractionMatrix(2, 2) << f1, f2, f2, f1).finished();
    auto expected =
        (PolynomialFractionMatrix(2, 2) << f1, f2, f2, f1).finished();
    m1 -= m2;
    ASSERT_EQ(m1, expected);
}

TEST(TestPolynomialFractionMatrix, operator_multiply_constant)
{
    auto m = (PolynomialFractionMatrix(2, 2) << f1, f2, f2, f1).finished();
    auto expected =
        (PolynomialFractionMatrix(2, 2) << 2 * f1, 2 * f2, 2 * f2, 2 * f1)
            .finished();
    ASSERT_EQ(m * 2, expected);
}

TEST(TestPolynomialFractionMatrix, operator_compound_multiply_constant)
{
    auto m = (PolynomialFractionMatrix(2, 2) << f1, f2, f2, f1).finished();
    auto expected =
        (PolynomialFractionMatrix(2, 2) << 2 * f1, 2 * f2, 2 * f2, 2 * f1)
            .finished();
    m *= 2;
    ASSERT_EQ(m, expected);
}
#endif

TEST(TestPolynomialFractionMatrix, operator_get_index_by_current_row_and_col)
{
    auto m = (PolynomialFractionMatrix(2, 2) << f1, f2, f2, f1).finished();
    ASSERT_EQ(m(0, 0), f1);
    ASSERT_EQ(m(0, 1), f2);
    ASSERT_EQ(m(1, 0), f2);
    ASSERT_EQ(m(1, 1), f1);
}

TEST(TestPolynomialFractionMatrix, operator_multiply_2x2)
{
    auto m1 = (PolynomialFractionMatrix(2, 2) << f1, f2, f2, f1).finished();
    auto m2 = m1;
    auto expected = (PolynomialFractionMatrix(2, 2) << f1 * f1 + f2 * f2,
                     f1 * f2 + f2 * f1, f2 * f1 + f1 * f2, f2 * f2 + f1 * f1)
                        .finished();
    ASSERT_EQ(m1 * m2, expected);
    ASSERT_EQ(m2 * m1, expected);
}

TEST(TestPolynomialFractionMatrix, operator_multiply_1x2_2x1)
{
    auto m1 = (PolynomialFractionMatrix(1, 2) << f1, f2).finished();
    auto m2 = (PolynomialFractionMatrix(2, 1) << f2, f1).finished();
    auto expected =
        (PolynomialFractionMatrix(1, 1) << f1 * f2 + f2 * f1).finished();
    ASSERT_EQ(m1 * m2, expected);
}

TEST(TestPolynomialFractionMatrix, get_left)
{
    using P = Polynomial<double>;
    using F = PolynomialFraction<double>;
    auto mf = (PolynomialFractionMatrix(3, 2) << F{{1}, {2, 1}}, F{{0}, {1}},
               F{{1}, {3, 1}}, F{{1}, {-3, 1}}, F{{1}, {2, -3, 1}}, F{{0}, {1}})
                  .finished();
    auto [Nl, Dl] = get_left(mf);

    auto expected_Nl = (PolynomialMatrix<double>(3, 2) << P{1}, P{0}, P{-3, 1},
                        P{3, 1}, P{1}, P{0})
                           .finished();

    auto expected_Dl = (PolynomialMatrix<double>(3, 3) << P{2, 1}, P{0}, P{0},
                        P{0}, P{-9, 0, 1}, P{0}, P{0}, P{0}, P{2, -3, 1})
                           .finished();
    ASSERT_EQ(Nl, expected_Nl);
    ASSERT_EQ(Dl, expected_Dl);
}

// TEST(TestPolynomialFractionMatrix, get_F)
// {
//     auto mf = (PolynomialFractionMatrix(3, 2)
//                    << PolynomialFractionMatrix{{1}, {2, 1}},
//                PolynomialFractionMatrix{{0}, {1}},
//                PolynomialFractionMatrix{{1}, {3, 1}},
//                PolynomialFractionMatrix{{1}, {-3, 1}},
//                PolynomialFractionMatrix{{1}, {2, -3, 1}},
//                PolynomialFractionMatrix{{0}, {1}})
//                   .finished();

//     Matrix<double> expected{
//         /*matrix*/
//         {(Eigen::MatrixXd(5, 9) << 2, 0, 0, 1, 0, 0, 0, 0, 0, /* 0 row */
//           0, -9, 0, 0, 0, 0, 0, 1, 0,                         /* 1 row */
//           0, 0, 2, 0, 0, -3, 0, 0, 1,                         /* 2 row */
//           1, -3, 1, 0, 1, 0, 0, 0, 0,                         /* 3 row */
//           0, 3, 0, 0, 1, 0, 0, 0, 0                           /* 4 row */
//           )
//              .finished()},
//         /*deg*/ 3};
//     ASSERT_EQ(get_F(mf), expected);
// }
