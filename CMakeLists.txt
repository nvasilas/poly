cmake_minimum_required(VERSION 3.16)
project(poly LANGUAGES CXX)

include(cmake/StandardProjectSettings.cmake)
include(cmake/PreventInSourceBuilds.cmake)

add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_17)

add_library(project_warnings INTERFACE)

# enable cache system
include(cmake/Cache.cmake)

include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

# sanitizer options if supported by compiler
include(cmake/Sanitizers.cmake)
enable_sanitizers(project_options)

# allow for static analysis options
include(cmake/StaticAnalyzers.cmake)

add_subdirectory(src)

option(ENABLE_TESTING "Enable Test Builds" ON)
if(ENABLE_TESTING)
    enable_testing()
    add_subdirectory(test)
endif()
