#include "../src/matrix_fraction_description.h"
#include "gtest/gtest.h"

using namespace poly;

// const Polynomial<int> p1({1, 4, -3, 2});
// const Polynomial<int> p2({-1, 2});
// const auto p3 = p1 + p2;
// const Polynomial<int> p4({3, -5, 0, 1, 6});

TEST(TestPolynomialFractionMatrix, get_left)
{
    // clang-format off
    PolynomialFractionMatrix<int> mf(3, 2, {
                                     {{1}, {2, 1}}, {{0}, {1}},
                                     {{1}, {3, 1}}, {{1}, {-3, 1}},
                                     {{1}, {2, -3, 1}}, {{0}, {1}},
                                     });
    auto [Nl, Dl] = mf.get_left();

    PolynomialMatrix<int> expected_Nl(3, 2, {
                                      {1}, {0},
                                      {-3, 1}, {3, 1},
                                      {1}, {0},
                                      });
    PolynomialMatrix<int> expected_Dl(3, 3, {
                                      {2, 1}, {0}, {0},
                                      {0}, {-9, 0, 1}, {0},
                                      {0}, {0}, {2, -3, 1},
                                      });
    // clang-format on

    std::cout << Nl << std::endl;
    std::cout << std::endl;
    std::cout << Dl << std::endl;
    std::cout << std::endl;
    std::cout << expected_Nl << std::endl;
    std::cout << expected_Dl << std::endl;

    ASSERT_EQ(Nl, expected_Nl);
    ASSERT_EQ(Dl, expected_Dl);
}

#if 0
TEST(TestPolynomialFractionMatrix, constructor_rows_cols_size)
{
    PolynomialMatrix<int> m1(2, { p1, p3, p2, p1 });
    ASSERT_EQ(m1.rows(), 2);
    ASSERT_EQ(m1.cols(), 2);
    ASSERT_EQ(m1.size(), 4);

    PolynomialMatrix<int> m2(4, { p1, p3, p2, p1 });
    ASSERT_EQ(m2.rows(), 4);
    ASSERT_EQ(m2.cols(), 1);
    ASSERT_EQ(m2.size(), 4);
}

TEST(TestPolynomialFractionMatrix, constructor_throw)
{
    EXPECT_THROW(
        PolynomialMatrix<int>(3, { p1, p3, p2, p1 }), std::invalid_argument);
    EXPECT_THROW(
        PolynomialMatrix<int>(5, { p1, p3, p2, p1 }), std::invalid_argument);
}

TEST(TestPolynomialFractionMatrix, operator_equal)
{
    PolynomialMatrix<int> m1(2, { p1, p3, p2, p1 });
    PolynomialMatrix<int> m2(2, { p1, p3, p2, p1 });
    ASSERT_EQ(m1, m2);
    ASSERT_EQ(m2, m1);
}

TEST(TestPolynomialFractionMatrix, operator_not_equal)
{
    PolynomialMatrix<int> m1(2, { p1, p3, p2, p1 });
    PolynomialMatrix<int> m2(2, { p2, p3, p2, p1 });
    ASSERT_NE(m1, m2);
    ASSERT_NE(m2, m1);

    PolynomialMatrix<int> m3(2, { p1, p3, p2, p1 });
    PolynomialMatrix<int> m4(4, { p1, p3, p2, p1 });
    ASSERT_NE(m3, m4);
}

TEST(TestPolynomialFractionMatrix, operator_plus)
{
    PolynomialMatrix<int> m(2, { p1, p3, p2, p1 });
    PolynomialMatrix<int> expected(2, { 2 * p1, 2 * p3, 2 * p2, 2 * p1 });
    ASSERT_EQ(m + m, expected);
}

TEST(TestPolynomialFractionMatrix, operator_compound_plus)
{
    PolynomialMatrix<int> m(2, { p1, p3, p2, p1 });
    m += m;
    PolynomialMatrix<int> expected(2, { 2 * p1, 2 * p3, 2 * p2, 2 * p1 });
    ASSERT_EQ(m, expected);
}

TEST(TestPolynomialFractionMatrix, operator_minus)
{
    PolynomialMatrix<int> m1(2, { 2 * p1, 2 * p3, 2 * p2, 2 * p1 });
    PolynomialMatrix<int> m2(2, { p1, p3, p2, p1 });
    PolynomialMatrix<int> expected(2, { p1, p3, p2, p1 });
    ASSERT_EQ(m1 - m2, expected);
}

TEST(TestPolynomialFractionMatrix, operator_compound_minus)
{
    PolynomialMatrix<int> m1(2, { 2 * p1, 2 * p3, 2 * p2, 2 * p1 });
    PolynomialMatrix<int> m2(2, { p1, p3, p2, p1 });
    PolynomialMatrix<int> expected(2, { p1, p3, p2, p1 });
    m1 -= m2;
    ASSERT_EQ(m1, expected);
}

TEST(TestPolynomialFractionMatrix, operator_plus_throw)
{
    PolynomialMatrix<int> m1(2, { p1, p3, p2, p1 });
    PolynomialMatrix<int> m2(4, { p1, p3, p2, p1 });
    EXPECT_THROW(m1 + m2, std::logic_error);

    PolynomialMatrix<int> m3(2, { p1, p3, p2, p1 });
    PolynomialMatrix<int> m4(1, { p1, p3, p2, p1 });
    EXPECT_THROW(m3 + m4, std::logic_error);
}

TEST(TestPolynomialFractionMatrix, operator_minus_throw)
{
    PolynomialMatrix<int> m1(2, { p1, p3, p2, p1 });
    PolynomialMatrix<int> m2(4, { p1, p3, p2, p1 });
    EXPECT_THROW(m1 - m2, std::logic_error);

    PolynomialMatrix<int> m3(2, { p1, p3, p2, p1 });
    PolynomialMatrix<int> m4(1, { p1, p3, p2, p1 });
    EXPECT_THROW(m3 - m4, std::logic_error);
}

TEST(TestPolynomialFractionMatrix, operator_multiply_constant)
{
    PolynomialMatrix<int> m(2, { p1, p3, p2, p1 });
    PolynomialMatrix<int> expected(2, { 2 * p1, 2 * p3, 2 * p2, 2 * p1 });
    ASSERT_EQ(m * 2, expected);
    ASSERT_EQ(2 * m, expected);
}

TEST(TestPolynomialFractionMatrix, operator_compound_multiply_constant)
{
    PolynomialMatrix<int> m(2, { p1, p3, p2, p1 });
    PolynomialMatrix<int> expected(2, { 2 * p1, 2 * p3, 2 * p2, 2 * p1 });
    m *= 2;
    ASSERT_EQ(m, expected);
}

TEST(TestPolynomialFractionMatrix, operator_get_index_by_current_row_and_col)
{
    PolynomialMatrix<int> m1(2, { p1, p3, p2, p1, p2, p3 });
    ASSERT_EQ(m1(0, 1), p3);
    ASSERT_EQ(m1(1, 1), p2);
    ASSERT_EQ(m1(0, 2), p2);
    ASSERT_EQ(m1(1, 2), p3);

    PolynomialMatrix<int> m2(3, { p1, p3, p2, p1, p2, p3 });
    ASSERT_EQ(m2(0, 1), p3);
    ASSERT_EQ(m2(1, 1), p1);
    ASSERT_EQ(m2(2, 0), p2);
    ASSERT_EQ(m2(2, 1), p3);
}

TEST(TestPolynomialFractionMatrix, operator_multiply_2x2)
{
    // clang-format off
    PolynomialMatrix<int> m1(2, {
            p1, p3,
            p2, p1
    });
    PolynomialMatrix<int> m2(2, {
            p1, p3,
            p2, p1
    });
    PolynomialMatrix<int> expected(2, 2, {
            p1 * p1 + p3 * p2,
            p1 * p3 + p3 * p1,
            p2 * p1 + p1 * p2,
            p2 * p3 + p1 * p1
    });
    // clang-format on
    ASSERT_EQ(m1 * m2, expected);
    ASSERT_EQ(m2 * m1, expected);
}

TEST(TestPolynomialFractionMatrix, operator_multiply_1x2_2x1)
{
    // clang-format off
    PolynomialMatrix<int> m1(1, 2, { p1, p2 });
    PolynomialMatrix<int> m2(2, 1, {
            p3,
            p4
    });
    // clang-format on
    PolynomialMatrix<int> expected(1, 1, { p1 * p3 + p2 * p4 });
    ASSERT_EQ(m1 * m2, expected);
}

TEST(TestPolynomialFractionMatrix, operator_multiply_throw)
{
    // clang-format off
    PolynomialMatrix<int> m1(2, {
            p1, p3,
            p2, p1
    });
    PolynomialMatrix<int> m2(4, {
            p1, p3,
            p2, p1
    });
    // clang-format on
    EXPECT_THROW(m1 + m2, std::logic_error);
}

TEST(TestPolynomialFractionMatrix, operator_eval)
{
    // clang-format off
    PolynomialMatrix<int> m(2, {
            p1, p3,
            p2, p1
    });
    std::vector<int> expected_0 = {
            1, 0,
            -1, 1
    };
    std::vector<int> expected_2 = {
            13, 16,
            3, 13
    };
    // clang-format on
    ASSERT_EQ(m(0), expected_0);
    ASSERT_EQ(m(2), expected_2);
}

TEST(TestPolynomialFractionMatrix, max_degree)
{
    PolynomialMatrix<int> m(2, { p1, p4, p2, p1 });
    ASSERT_EQ(m.max_degree(), 4);
}

TEST(TestPolynomialFractionMatrix, to_matrix)
{
    // clang-format off
    PolynomialMatrix<int> m1(3, 2, {
            { 1, 0, -7, 6 },  { 0 },
            { 1, -1, -4, 4 }, { 1, 0 },
            { 1, 5, 6 },      { 0 },
    });
    std::vector<int> expected1({
            1,  0, -7, 6, 0, 0, 0, 0,
            1, -1, -4, 4, 1, 0, 0, 0,
            1,  5,  6, 0, 0, 0, 0, 0,
    });
    // clang-format on
    ASSERT_EQ(m1.to_matrix(), expected1);

    // clang-format off
    PolynomialMatrix<int> m2(3, 2, {
            { 0 },    { 1, 0, -7, 6 },
            { 1, 0 }, { 1, -1, -4, 4 },
            { 0 },    { 1, 5, 6 },
    });
    std::vector<int> expected2({
            0, 0, 0, 0, 1, 0, -7, 6,
            1, 0, 0, 0, 1,-1, -4, 4,
            0, 0, 0, 0, 1, 5,  6, 0,
    });
    // clang-format on
    ASSERT_EQ(m2.to_matrix(), expected2);

    // clang-format off
    PolynomialMatrix<int> m3(2, 1, {
            { 0 },
            { 1, 0 },
    });
    std::vector<int> expected3({
            0, 0,
            1, 0,
    });
    // clang-format on
    ASSERT_EQ(m3.to_matrix(), expected3);

    PolynomialMatrix<int> m4(1, 1, { { 0 } });
    std::vector<int> expected4({ 0 });
    ASSERT_EQ(m4.to_matrix(), expected4);
}

TEST(TestPolynomialFractionMatrix, to_coeff_matrix)
{
    // clang-format off
    PolynomialMatrix<int> m1(3, 2, {
            { 1, 0, -7, 6 },  { 0 },
            { 1, -1, -4, 4 }, { 1, 0 },
            { 1, 5, 6 },      { 0 }
    });
    std::vector<int> expected1({
            1, 0,  0, 0, -7, 0, 6, 0,
            1, 1, -1, 0, -4, 0, 4, 0,
            1, 0,  5, 0,  6, 0, 0, 0,
    });
    // clang-format on
    ASSERT_EQ(m1.to_coeff_matrix(), expected1);

    // clang-format off
    PolynomialMatrix<int> m2(3, 2, {
            { 0 },    { 1, 0, -7, 6 },
            { 1, 0 }, { 1, -1, -4, 4 },
            { 0 },    { 1, 5, 6 },
    });
    std::vector<int> expected2({
            0, 1, 0, 0, 0, -7, 0, 6,
            1, 1, 0,-1, 0, -4, 0, 4,
            0 ,1, 0, 5, 0,  6, 0, 0,
    });
    // clang-format on
    ASSERT_EQ(m2.to_coeff_matrix(), expected2);

    // clang-format off
    PolynomialMatrix<int> m3(2, 1, {
            { 0 },
            { 1, 0 },
    });
    std::vector<int> expected3({
            0, 0,
            1, 0,
    });
    // clang-format on
    ASSERT_EQ(m3.to_coeff_matrix(), expected3);

    // clang-format off
    PolynomialMatrix<int> m4(1, 2, {
            { 0 }, { 1, 0 },
    });
    std::vector<int> expected4({
            0, 0, 1, 0,
    });
    // clang-format on
    ASSERT_EQ(m4.to_coeff_matrix(), expected4);

    PolynomialMatrix<int> m5(1, 1, { { 0 } });
    std::vector<int> expected5({ 0 });
    ASSERT_EQ(m5.to_coeff_matrix(), expected5);

    PolynomialMatrix<int> m6(1, 2, { { 0 }, { 1 } });
    std::vector<int> expected6({ 0, 1 });
    ASSERT_EQ(m6.to_coeff_matrix(), expected6);
}
#endif
