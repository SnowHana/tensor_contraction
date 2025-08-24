cmake_minimum_required(VERSION 3.15)
project(tensor_multiplier LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Coverage
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
# set(CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
option(ENABLE_COVERAGE "Enable coverage" OFF)
if (ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  add_compile_options(--coverage -O0)
  add_link_options(--coverage)
endif()


find_package(Catch2 3 REQUIRED)

# Library
add_library(matrix src/matrix.cpp)
target_include_directories(matrix PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Tests
add_executable(test_matrix tests/test_matrix.cpp)
# Make sure test sees headers and links the lib + Catch2
target_link_libraries(test_matrix PRIVATE matrix Catch2::Catch2WithMain)
# (Optional) also add include dir explicitly; harmless redundancy:
target_include_directories(test_matrix PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
