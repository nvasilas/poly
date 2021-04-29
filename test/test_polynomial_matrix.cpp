#include "../src/polynomial_matrix.h"
#include "gtest/gtest.h"

using namespace poly;

const Polynomial<int> p1({1, 4, -3, 2});
const Polynomial<int> p2({-1, 2});
const auto p3 = p1 + p2;
const Polynomial<int> p4({3, -5, 0, 1, 6});

TEST(TestPolynomialMatrix, constructor_rows_cols_size)
{
    PolynomialMatrix<int> m1{{p1, p3}, {p2, p1}};
    ASSERT_EQ(m1.rows(), 2);
    ASSERT_EQ(m1.columns(), 2);
    ASSERT_EQ(size(m1), 4);

    PolynomialMatrix<int> m2{{p1, p3, p2, p1}};
    ASSERT_EQ(m2.rows(), 1);
    ASSERT_EQ(m2.columns(), 4);
    ASSERT_EQ(size(m2), 4);
}

TEST(TestPolynomialMatrix, operator_equal)
{
    PolynomialMatrix<int> m1{{p1, p3}, {p2, p1}};
    PolynomialMatrix<int> m2{{p1, p3}, {p2, p1}};
    ASSERT_EQ(m1, m2);
    ASSERT_EQ(m2, m1);
}

TEST(TestPolynomialMatrix, operator_not_equal)
{
    PolynomialMatrix<int> m1{{p1, p3}, {p2, p1}};
    PolynomialMatrix<int> m2{{p2, p3}, {p2, p1}};
    ASSERT_NE(m1, m2);
    ASSERT_NE(m2, m1);

    PolynomialMatrix<int> m3{{p1, p3}, {p2, p1}};
    PolynomialMatrix<int> m4{{p1, p3, p2, p1}};
    ASSERT_NE(m3, m4);
}

TEST(TestPolynomialMatrix, operator_plus)
{
    PolynomialMatrix<int> m{{p1, p3}, {p2, p1}};
    PolynomialMatrix<int> expected{{2 * p1, 2 * p3}, {2 * p2, 2 * p1}};
    ASSERT_EQ(m + m, expected);
}

TEST(TestPolynomialMatrix, operator_compound_plus)
{
    PolynomialMatrix<int> m{{p1, p3}, {p2, p1}};
    m += m;
    PolynomialMatrix<int> expected{{2 * p1, 2 * p3}, {2 * p2, 2 * p1}};
    ASSERT_EQ(m, expected);
}

TEST(TestPolynomialMatrix, operator_minus)
{

    PolynomialMatrix<int> m1{{2 * p1, 2 * p3}, {2 * p2, 2 * p1}};
    PolynomialMatrix<int> m2{{p1, p3}, {p2, p1}};
    PolynomialMatrix<int> expected{{p1, p3}, {p2, p1}};
    ASSERT_EQ(m1 - m2, expected);
}

TEST(TestPolynomialMatrix, operator_compound_minus)
{
    PolynomialMatrix<int> m1{{2 * p1, 2 * p3}, {2 * p2, 2 * p1}};
    PolynomialMatrix<int> m2{{p1, p3}, {p2, p1}};
    PolynomialMatrix<int> expected{{p1, p3}, {p2, p1}};
    m1 -= m2;
    ASSERT_EQ(m1, expected);
}

TEST(TestPolynomialMatrix, operator_multiply_constant)
{
    const PolynomialMatrix<int> m{{p1, p3}, {p2, p1}};
    PolynomialMatrix<int> expected{{2 * p1, 2 * p3}, {2 * p2, 2 * p1}};
    ASSERT_EQ(m * 2, expected);
}

TEST(TestPolynomialMatrix, operator_compound_multiply_constant)
{
    PolynomialMatrix<int> m{{p1, p3}, {p2, p1}};
    PolynomialMatrix<int> expected{{2 * p1, 2 * p3}, {2 * p2, 2 * p1}};
    m *= 2;
    ASSERT_EQ(m, expected);
}

TEST(TestPolynomialMatrix, operator_get_index_by_current_row_and_col)
{
    PolynomialMatrix<int> m1{{p1, p3, p2}, {p1, p2, p3}};
    ASSERT_EQ(m1(0, 1), p3);
    ASSERT_EQ(m1(1, 1), p2);
    ASSERT_EQ(m1(0, 2), p2);
    ASSERT_EQ(m1(1, 2), p3);

    PolynomialMatrix<int> m2{{p1, p3}, {p2, p1}, {p2, p3}};
    ASSERT_EQ(m2(0, 1), p3);
    ASSERT_EQ(m2(1, 1), p1);
    ASSERT_EQ(m2(2, 0), p2);
    ASSERT_EQ(m2(2, 1), p3);
}

TEST(TestPolynomialMatrix, operator_multiply_2x2)
{
    PolynomialMatrix<int> m1{ { p1, p3 },
                              { p2, p1 } };
    auto m2 = m1;
    PolynomialMatrix<int> expected{ { p1 * p1 + p3 * p2, p1 * p3 + p3 * p1 },
                                    { p2 * p1 + p1 * p2, p2 * p3 + p1 * p1 } };
    ASSERT_EQ(m1 * m2, expected);
    ASSERT_EQ(m2 * m1, expected);
}

TEST(TestPolynomialMatrix, operator_multiply_1x2_2x1)
{
    PolynomialMatrix<int> m1{ { p1, p2 } };
    PolynomialMatrix<int> m2{ { p3 },
                              { p4 } };
    PolynomialMatrix<int> expected{{p1 * p3 + p2 * p4}};
    ASSERT_EQ(m1 * m2, expected);
}

TEST(TestPolynomialMatrix, max_degree)
{
    PolynomialMatrix<int> m{{p1, p4}, {p2, p1}};
    ASSERT_EQ(max(m).degree(), 4);
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
    PolynomialMatrix<int> m1{
        {{1, 0, -7, 6}, {0}},
        {{1, -1, -4, 4}, {1, 0}},
        {{1, 5, 6}, {0}}
    };
    Matrix<int> expected1{
        {1, 0, 0, 0, -7, 0, 6, 0},
        {1, 1, -1, 0, -4, 0, 4, 0},
        {1, 0, 5, 0, 6, 0, 0, 0}
    };
    auto mat_coeff = to_coeff_matrix(m1);
    std::cout << mat_coeff << std::endl;
    ASSERT_EQ(to_coeff_matrix(m1), expected1);

    PolynomialMatrix<int> m2{
        {{0}, {1, 0, -7, 6}},
        {{1, 0}, {1, -1, -4, 4}},
        {{0}, {1, 5, 6}},
    };
    Matrix<int> expected2{
        {0, 1, 0, 0, 0, -7, 0, 6}, 
        {1, 1, 0, -1, 0, -4, 0, 4},
        {0 ,1, 0, 5, 0,  6, 0, 0},
    };
    ASSERT_EQ(to_coeff_matrix(m2), expected2);

    PolynomialMatrix<int> m3{
        {{0}},
        {{1, 0}}
    };
    Matrix<int> expected3{{0, 0}, {1, 0}};
    ASSERT_EQ(to_coeff_matrix(m3), expected3);

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

    PolynomialMatrix<int> m7{
        {{1, 0, -7, 6}, {0}, {1, 2}},
        {{1, -1, -4, 4}, {1, 0}, {1}},
        {{1, 5, 6}, {0}, {0}}
    };
    Matrix<int> expected7{
        {1, 0, 1, 0, 0, 2, -7, 0, 0, 6, 0, 0},
        {1, 1, 1, -1, 0, 0, -4, 0, 0, 4, 0, 0},
        {1, 0, 0, 5, 0, 0, 6, 0, 0, 0, 0, 0}
    };
    ASSERT_EQ(to_coeff_matrix(m7), expected7);
}
