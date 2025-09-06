// tests/test_rowmajor.cpp
#include <catch2/catch_test_macros.hpp>
#include "rowmajor.hpp"
#include <vector>

using Indices = std::vector<std::size_t>;

TEST_CASE("RowMajorLayout: stride construction for 3D", "[rowmajor][strides]") {
    RowMajorLayout layout({2, 3, 4});
    auto dims = layout.dims();
    auto strides = layout.strides();

    REQUIRE(dims == std::vector<std::size_t>({2, 3, 4}));
    REQUIRE(strides == std::vector<std::size_t>({12, 4, 1}));

    // Spot-check some linear indices
    REQUIRE(layout.toFlat(Indices{0, 0, 0}) == 0);
    REQUIRE(layout.toFlat(Indices{0, 0, 1}) == 1);
    REQUIRE(layout.toFlat(Indices{0, 1, 0}) == 4);
    REQUIRE(layout.toFlat(Indices{1, 0, 0}) == 12);
    REQUIRE(layout.toFlat(Indices{1, 2, 3}) == 1*12 + 2*4 + 3*1); // 23
}

TEST_CASE("RowMajorLayout: 1D degenerate case", "[rowmajor]") {
    RowMajorLayout layout({5});
    REQUIRE(layout.strides() == std::vector<std::size_t>({1}));
    // toFlat should be identity for 1D
    REQUIRE(layout.toFlat(Indices{0}) == 0);
    REQUIRE(layout.toFlat(Indices{3}) == 3);
    REQUIRE(layout.toFlat(Indices{4}) == 4);
}

TEST_CASE("RowMajorLayout: 2D mapping", "[rowmajor]") {
    // dims = {rows, cols} in row-major
    RowMajorLayout layout({3, 7});
    // strides = {7, 1}
    REQUIRE(layout.strides() == std::vector<std::size_t>({7, 1}));

    // (r,c) -> r*7 + c
    REQUIRE(layout.toFlat(Indices{0, 0}) == 0);
    REQUIRE(layout.toFlat(Indices{0, 6}) == 6);
    REQUIRE(layout.toFlat(Indices{1, 0}) == 7);
    REQUIRE(layout.toFlat(Indices{2, 5}) == 2*7 + 5);
}

TEST_CASE("RowMajorLayout: rank mismatch throws", "[rowmajor][errors]") {
    RowMajorLayout layout({2, 3, 4});
    // too few indices
    REQUIRE_THROWS_AS(layout.toFlat(Indices{1, 2}), std::runtime_error);
    // too many indices
    REQUIRE_THROWS_AS(layout.toFlat(Indices{1, 2, 3, 4}), std::runtime_error);
}

TEST_CASE("RowMajorLayout: larger dims sanity check", "[rowmajor]") {
    RowMajorLayout layout({4, 5, 6}); // strides = {30, 6, 1}
    REQUIRE(layout.strides() == std::vector<std::size_t>({30, 6, 1}));
    REQUIRE(layout.toFlat(Indices{3, 4, 5}) == 3*30 + 4*6 + 5); // 119
}
