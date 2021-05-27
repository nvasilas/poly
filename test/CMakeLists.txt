find_package(GTest REQUIRED)

macro(configure_gtest test_name)
    target_link_libraries(${test_name} GTest::gtest GTest::gtest_main Eigen3::Eigen)
    target_compile_features(${test_name} PRIVATE cxx_std_17)
    gtest_discover_tests(${test_name})
endmacro()

add_executable(test_polynomial test_polynomial.cpp)
configure_gtest(test_polynomial)

add_executable(test_polynomial_fraction test_polynomial_fraction.cpp)
configure_gtest(test_polynomial_fraction)

add_executable(test_polynomial_matrix test_polynomial_matrix.cpp)
configure_gtest(test_polynomial_matrix)

add_executable(test_polynomial_fraction_matrix test_polynomial_fraction_matrix.cpp)
configure_gtest(test_polynomial_fraction_matrix)
