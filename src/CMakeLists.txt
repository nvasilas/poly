add_executable(poly
    polynomial_fraction_matrix.cpp
    coprime_factorization.cpp
    main.cpp)
target_link_libraries(poly PRIVATE
    project_options project_warnings Eigen3::Eigen)
