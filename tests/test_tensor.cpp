#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "tensor.hpp"
#include <vector>
#include <numeric>

TEST_CASE("Tensor construction and shape check", "[tensor]") {
    std::vector<std::size_t> shape = {2, 3};   // 2x3 tensor
    std::vector<int> flat = {1, 2, 3, 4, 5, 6};

    Tensor t(shape, flat);

    SECTION("dims reports correct shape") {
        auto& dims = t.dims();
        REQUIRE(dims.size() == 2);
        REQUIRE(dims[0] == 2);
        REQUIRE(dims[1] == 3);
    }

    SECTION("rowMajor reports correct size") {
        REQUIRE(t.rowMajor().size() == 6);
        REQUIRE(t.rowMajor().rank() == 2);
    }
}


TEST_CASE("Tensor construction and shape check - bigger dimensions", "[tensor]") {
    std::vector<std::size_t> shape = {2, 3, 4, 5};   // 2x3 tensor
    std::vector<int> flat(120);
    std::iota(std::begin(flat), std::end(flat), 0);

    Tensor t(shape, flat);

    SECTION("dims reports correct shape") {
        auto& dims = t.dims();
        REQUIRE(dims.size() == 4);
        REQUIRE(dims[0] == 2);
        REQUIRE(dims[1] == 3);
        REQUIRE(dims[2] == 4);
        REQUIRE(dims[3] == 5);
    }

    SECTION("rowMajor reports correct size") {
        REQUIRE(t.rowMajor().rank() == 4);
        REQUIRE(t.rowMajor().size() == 120);
    }
}

TEST_CASE("Tensor throws on mismatched size", "[tensor]") {
    std::vector<std::size_t> shape = {2, 2};
    std::vector<int> flat = {1, 2, 3}; // only 3 elements, should be 4

    REQUIRE_THROWS_AS(Tensor(shape, flat), std::runtime_error);
}

TEST_CASE("Tensor element access with at()", "[tensor]") {
    Tensor t({2, 2}, {10, 20, 30, 40});

    SECTION("Getter returns correct values") {
        REQUIRE(t.at({0,0}) == 10);
        REQUIRE(t.at({0,1}) == 20);
        REQUIRE(t.at({1,0}) == 30);
        REQUIRE(t.at({1,1}) == 40);
    }

    SECTION("Setter modifies underlying data") {
        t.at({0,1}) = 99;
        REQUIRE(t.at({0,1}) == 99);

        // Ensure it actually updated the flat data
        REQUIRE(t.at({1,0}) == 30); // unaffected
    }
}

TEST_CASE("Tensor out-of-range coordinate throws", "[tensor]") {
    Tensor t({2, 2}, {1,2,3,4});

    // rank mismatch
    REQUIRE_THROWS_AS(t.at({0,0,0}), std::runtime_error);

    // index too large
    REQUIRE_THROWS_AS(t.at({2,0}), std::out_of_range);
    REQUIRE_THROWS_AS(t.at({0,2}), std::out_of_range);
}
