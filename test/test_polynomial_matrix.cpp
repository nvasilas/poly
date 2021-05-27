#include "../src/polynomial_matrix.h"
#include "gtest/gtest.h"

using namespace poly;

static const Polynomial<int> p1({1, 4, -3, 2});
static const Polynomial<int> p2({-1, 2});
static const auto p3 = p1 + p2;
static const Polynomial<int> p4({3, -5, 0, 1, 6});

TEST(TestPolynomialMatrix, constructor_rows_cols_size)
{
    auto m1 = (PolynomialMatrix<int>(2, 2) << p1, p3, p2, p1).finished();
    ASSERT_EQ(m1.rows(), 2);
    ASSERT_EQ(m1.cols(), 2);
    ASSERT_EQ(m1.size(), 4);

    auto m2 = (PolynomialMatrix<int>(1, 4) << p1, p3, p2, p1).finished();
    ASSERT_EQ(m2.rows(), 1);
    ASSERT_EQ(m2.cols(), 4);
    ASSERT_EQ(m2.size(), 4);
}

TEST(TestPolynomialMatrix, operator_equal)
{
    auto m1 = (PolynomialMatrix<int>(2, 2) << p1, p3, p2, p1).finished();
    auto m2 = (PolynomialMatrix<int>(2, 2) << p1, p3, p2, p1).finished();
    ASSERT_EQ(m1, m2);
    ASSERT_EQ(m2, m1);
}

TEST(TestPolynomialMatrix, operator_not_equal)
{
    auto m1 = (PolynomialMatrix<int>(2, 2) << p1, p3, p2, p1).finished();
    auto m2 = (PolynomialMatrix<int>(2, 2) << p2, p3, p2, p1).finished();
    ASSERT_NE(m1, m2);
    ASSERT_NE(m2, m1);
}

TEST(TestPolynomialMatrix, operator_plus)
{
    auto m = (PolynomialMatrix<int>(2, 2) << p1, p3, p2, p1).finished();
    auto expected =
        (PolynomialMatrix<int>(2, 2) << 2 * p1, 2 * p3, 2 * p2, 2 * p1)
            .finished();
    ASSERT_EQ(m + m, expected);
}

TEST(TestPolynomialMatrix, operator_compound_plus)
{
    auto m = (PolynomialMatrix<int>(2, 2) << p1, p3, p2, p1).finished();
    m += m;
    auto expected =
        (PolynomialMatrix<int>(2, 2) << 2 * p1, 2 * p3, 2 * p2, 2 * p1)
            .finished();
    ASSERT_EQ(m, expected);
}

TEST(TestPolynomialMatrix, operator_minus)
{
    auto m1 = (PolynomialMatrix<int>(2, 2) << 2 * p1, 2 * p3, 2 * p2, 2 * p1)
                  .finished();
    auto m2 = (PolynomialMatrix<int>(2, 2) << p1, p3, p2, p1).finished();
    auto expected = (PolynomialMatrix<int>(2, 2) << p1, p3, p2, p1).finished();
    ASSERT_EQ(m1 - m2, expected);
}

TEST(TestPolynomialMatrix, operator_compound_minus)
{
    auto m1 = (PolynomialMatrix<int>(2, 2) << 2 * p1, 2 * p3, 2 * p2, 2 * p1)
                  .finished();
    auto m2 = (PolynomialMatrix<int>(2, 2) << p1, p3, p2, p1).finished();
    auto expected = (PolynomialMatrix<int>(2, 2) << p1, p3, p2, p1).finished();
    m1 -= m2;
    ASSERT_EQ(m1, expected);
}

TEST(TestPolynomialMatrix, operator_multiply_constant)
{
    auto m = (PolynomialMatrix<int>(2, 2) << p1, p3, p2, p1).finished();
    auto expected =
        (PolynomialMatrix<int>(2, 2) << 2 * p1, 2 * p3, 2 * p2, 2 * p1)
            .finished();
    ASSERT_EQ(m * 2, expected);
}

TEST(TestPolynomialMatrix, operator_compound_multiply_constant)
{
    auto m = (PolynomialMatrix<int>(2, 2) << p1, p3, p2, p1).finished();
    auto expected =
        (PolynomialMatrix<int>(2, 2) << 2 * p1, 2 * p3, 2 * p2, 2 * p1)
            .finished();
    m *= 2;
    ASSERT_EQ(m, expected);
}

TEST(TestPolynomialMatrix, operator_get_index_by_current_row_and_col)
{
    auto m1 =
        (PolynomialMatrix<int>(2, 3) << p1, p3, p2, p1, p2, p3).finished();
    ASSERT_EQ(m1(0, 1), p3);
    ASSERT_EQ(m1(1, 1), p2);
    ASSERT_EQ(m1(0, 2), p2);
    ASSERT_EQ(m1(1, 2), p3);

    auto m2 =
        (PolynomialMatrix<int>(3, 2) << p1, p3, p2, p1, p2, p3).finished();
    ASSERT_EQ(m2(0, 1), p3);
    ASSERT_EQ(m2(1, 1), p1);
    ASSERT_EQ(m2(2, 0), p2);
    ASSERT_EQ(m2(2, 1), p3);
}

TEST(TestPolynomialMatrix, operator_multiply_2x2)
{
    auto m1 = (PolynomialMatrix<int>(2, 2) << p1, p3, p2, p1).finished();
    auto m2 = m1;
    auto expected = (PolynomialMatrix<int>(2, 2) << p1 * p1 + p3 * p2,
                     p1 * p3 + p3 * p1, p2 * p1 + p1 * p2, p2 * p3 + p1 * p1)
                        .finished();
    ASSERT_EQ(m1 * m2, expected);
    ASSERT_EQ(m2 * m1, expected);
}

TEST(TestPolynomialMatrix, operator_multiply_1x2_2x1)
{
    auto m1 = (PolynomialMatrix<int>(1, 2) << p1, p2).finished();
    auto m2 = (PolynomialMatrix<int>(2, 1) << p3, p4).finished();
    auto expected =
        (PolynomialMatrix<int>(1, 1) << p1 * p3 + p2 * p4).finished();
    ASSERT_EQ(m1 * m2, expected);
}

TEST(TestPolynomialMatrix, max_degree)
{
    auto m = (PolynomialMatrix<int>(2, 2) << p1, p4, p2, p1).finished();
    ASSERT_EQ(m.maxCoeff().degree(), 4);
}

// TEST(TestPolynomialMatrix, to_matrix)
// {
//     PolynomialMatrix<int> m1{
//         {{1, 0, -7, 6}, {0}},
//         {{1, -1, -4, 4}, {1, 0}},
//         {{1, 5, 6}, {0}}
//     };
//     Matrix<int> expected1{{1, 0, -7, 6, 0, 0, 0, 0},
//                           {1, -1, -4, 4, 1, 0, 0, 0},
//                           {1, 5, 6, 0, 0, 0, 0, 0}};
//     ASSERT_EQ(to_matrix(m1), expected1);

//     PolynomialMatrix<int> m2{
//         {{0}, {1, 0, -7, 6}}, {{1, 0}, {1, -1, -4, 4}}, {{0}, {1, 5, 6}}};
//     Matrix<int> expected2{
//         {0, 0, 0, 0, 1, 0, -7, 6},
//         {1, 0, 0, 0, 1, -1, -4, 4},
//         {0, 0, 0, 0, 1, 5, 6, 0},
//     };
//     ASSERT_EQ(to_matrix(m2), expected2);

//     PolynomialMatrix<int> m3{{{0}}, {{1, 0}}};
//     Matrix<int> expected3{{0, 0}, {1, 0}};
//     ASSERT_EQ(to_matrix(m3), expected3);

//     PolynomialMatrix<int> m4{{{0}}};
//     Matrix<int> expected4{{0}};
//     ASSERT_EQ(to_matrix(m4), expected4);

//     PolynomialMatrix<int> m5{
//         {{1, 0, -7, 6}, {0}, {1, 2}},
//         {{1, -1, -4, 4}, {1, 0}, {1}},
//         {{1, 5, 6}, {0}, {0}}
//     };
//     Matrix<int> expected5{{1, 0, -7, 6, 0, 0, 0, 0, 1, 2, 0, 0},
//                           {1, -1, -4, 4, 1, 0, 0, 0, 1, 0, 0, 0},
//                           {1, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0 ,0}};
//     ASSERT_EQ(to_matrix(m5), expected5);
// }

TEST(TestPolynomialMatrix, to_coeff_matrix)
{
    using P = Polynomial<int>;
    auto m1 = (PolynomialMatrix<int>(3, 2) << P{1, 0, -7, 6}, P{0},
               P{1, -1, -4, 4}, P{1, 0}, P{1, 5, 6}, P{0})
                  .finished();
    Matrix<int> expected1{
        /*matrix*/
        {(Eigen::MatrixXi(3, 8) << 1, 0, 0, 0, -7, 0, 6, 0, /* 0 row */
          1, 1, -1, 0, -4, 0, 4, 0,                         /* 1 row */
          1, 0, 5, 0, 6, 0, 0, 0 /* 2 row */)
             .finished()},
        /*deg*/ 4};
    ASSERT_EQ(to_coeff_matrix(m1), expected1);

    auto m2 = (PolynomialMatrix<int>(3, 2) << P{0}, P{1, 0, -7, 6}, P{1, 0},
               P{1, -1, -4, 4}, P{0}, P{1, 5, 6})
                  .finished();
    Matrix<int> expected2{
        /*matrix*/
        {(Eigen::MatrixXi(3, 8) << 0, 1, 0, 0, 0, -7, 0, 6, /* 0 row */
          1, 1, 0, -1, 0, -4, 0, 4,                         /* 1 row */
          0, 1, 0, 5, 0, 6, 0, 0 /* 2 row */)
             .finished()},
        /*deg*/ 4};
    ASSERT_EQ(to_coeff_matrix(m2), expected2);

    auto m3 = (PolynomialMatrix<int>(2, 1) << P{0}, P{1, 0}).finished();
    Matrix<int> expected3{/*matrix*/
                          {(Eigen::MatrixXi(2, 2) << 0, 0, 1, 0).finished()},
                          /*deg*/ 2};
    ASSERT_EQ(to_coeff_matrix(m3), expected3);
#if 0

    PolynomialMatrix<int> m4{
        {{0}, {1, 0}}
    };
    Matrix<int> expected4{
        {0, 0, 1, 0}
    };
    ASSERT_EQ(to_coeff_matrix(m4), expected4);

    PolynomialMatrix<int> m5{{0}};
    Matrix<int> expected5{{0}};
    ASSERT_EQ(to_coeff_matrix(m5), expected5);

    PolynomialMatrix<int> m6{
        {{0}, {1}}
    };
    Matrix<int> expected6{{ 0, 1 }};
    ASSERT_EQ(to_coeff_matrix(m6), expected6);
#endif

    auto m7 = (PolynomialMatrix<int>(3, 3) << P{1, 0, -7, 6}, P{0}, P{1, 2},
               P{1, -1, -4, 4}, P{1, 0}, P{1}, P{1, 5, 6}, P{0}, P{0})
                  .finished();
    Matrix<int> expected7{/*matrix*/
                          {(Eigen::MatrixXi(3, 12) << 1, 0, 1, 0, 0, 2, -7, 0,
                            0, 6, 0, 0,                           /* 0 row */
                            1, 1, 1, -1, 0, 0, -4, 0, 0, 4, 0, 0, /* 1 row */
                            1, 0, 0, 5, 0, 0, 6, 0, 0, 0, 0, 0 /* 2 row */)
                               .finished()},
                          /*deg*/ 4};
    ASSERT_EQ(to_coeff_matrix(m7), expected7);
}
