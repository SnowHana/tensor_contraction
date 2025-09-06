cmake_minimum_required(VERSION 3.15)
project(tensor_multiplier LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# -------------------------------------------------------------------
# Coverage support (optional, controlled with -DENABLE_COVERAGE=ON)
# -------------------------------------------------------------------
option(ENABLE_COVERAGE "Enable coverage" OFF)
if (ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  add_compile_options(--coverage -O0)
  add_link_options(--coverage)
endif()

# -------------------------------------------------------------------
# Dependencies
# -------------------------------------------------------------------
find_package(Catch2 3 REQUIRED)

# -------------------------------------------------------------------
# Library target
# -------------------------------------------------------------------
add_library(matrix
    src/matrix.cpp
    src/rowmajor.cpp
)
target_include_directories(matrix PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# -------------------------------------------------------------------
# Testing
# -------------------------------------------------------------------
include(CTest)   # enable CTest framework
include(Catch)  # Catch2 CMake helpers (Catch2 v3)

# Test: matrix
add_executable(test_matrix tests/test_matrix.cpp)
target_link_libraries(test_matrix PRIVATE matrix Catch2::Catch2WithMain)
target_include_directories(test_matrix PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
catch_discover_tests(test_matrix)

# Test: rowmajor
add_executable(test_rowmajor tests/test_rowmajor.cpp)
target_link_libraries(test_rowmajor PRIVATE matrix Catch2::Catch2WithMain)
target_include_directories(test_rowmajor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
catch_discover_tests(test_rowmajor)
