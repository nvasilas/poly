add_executable(poly main.cpp)
target_link_libraries(poly PRIVATE
    project_options project_warnings Eigen3::Eigen)
