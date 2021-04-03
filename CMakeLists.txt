cmake_minimum_required(VERSION 3.16)
project(poly LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

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

find_package(blaze REQUIRED)
set(BLAZE_FLAGS -mavx)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR
	CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
    target_compile_options(project_options INTERFACE ${BLAZE_FLAGS})
else()
    message(AUTHOR_WARNING
	"No compiler warnings set for '${CMAKE_CXX_COMPILER_ID}' compiler.")
endif()

add_subdirectory(src)

option(ENABLE_TESTING "Enable Test Builds" ON)
if(ENABLE_TESTING)
    enable_testing()
    add_subdirectory(test)
endif()
