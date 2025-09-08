#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "tensor.hpp"
#include <vector>
#include <numeric>
#include <algorithm>


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


// ---------------- Additional tests for at() ----------------

TEST_CASE("at() boundary indices on 1D and 2D", "[tensor][at]") {
    SECTION("1D boundary access") {
        Tensor t({5}, {10, 20, 30, 40, 50});
        REQUIRE(t.at({0}) == 10);
        REQUIRE(t.at({4}) == 50);

        t.at({0}) = -1;
        t.at({4}) = -5;
        REQUIRE(t.at({0}) == -1);
        REQUIRE(t.at({4}) == -5);
    }

    SECTION("2D boundary corners") {
        // shape 2x3, row-major flat: [ (0,0)=0, (0,1)=1, (0,2)=2, (1,0)=3, (1,1)=4, (1,2)=5 ]
        Tensor t({2,3}, {0,1,2,3,4,5});
        REQUIRE(t.at({0,0}) == 0);
        REQUIRE(t.at({0,2}) == 2);
        REQUIRE(t.at({1,0}) == 3);
        REQUIRE(t.at({1,2}) == 5);

        // mutate edges
        t.at({0,2}) = 22;
        t.at({1,0}) = 30;
        REQUIRE(t.at({0,2}) == 22);
        REQUIRE(t.at({1,0}) == 30);
    }
}

TEST_CASE("at() row-major mapping on 3D", "[tensor][at]") {
    // shape 3 x 4 x 5 => total 60
    std::vector<int> flat(60);
    std::iota(flat.begin(), flat.end(), 0);
    Tensor t({3,4,5}, flat);

    // Strides for row-major: [4*5=20, 5, 1]
    // value at (i,j,k) must equal i*20 + j*5 + k (since flat was 0..59)
    for (std::size_t i = 0; i < 3; ++i) {
        for (std::size_t j = 0; j < 4; ++j) {
            for (std::size_t k = 0; k < 5; ++k) {
                int expected = static_cast<int>(i*20 + j*5 + k);
                REQUIRE(t.at({i,j,k}) == expected);
            }
        }
    }

    // Mutate a few random interior points
    t.at({1,2,3}) = 999;
    t.at({2,0,4}) = 777;
    REQUIRE(t.at({1,2,3}) == 999);
    REQUIRE(t.at({2,0,4}) == 777);
}

TEST_CASE("at() rank mismatch and index OOB", "[tensor][at]") {
    Tensor t({2,3}, {0,1,2,3,4,5});

    // rank mismatch (too many / too few)
    REQUIRE_THROWS_AS(t.at({0}), std::runtime_error);
    REQUIRE_THROWS_AS(t.at({0,0,0}), std::runtime_error);

    // index too large in each dimension
    REQUIRE_THROWS_AS(t.at({2,0}), std::out_of_range); // dim0 size=2 → valid {0,1}
    REQUIRE_THROWS_AS(t.at({0,3}), std::out_of_range); // dim1 size=3 → valid {0,1,2}
}

TEST_CASE("at() const overload returns const ref (read-only)", "[tensor][at][const]") {
    Tensor t({2,2}, {10,20,30,40});
    const Tensor& ct = t;

    // Read via const at()
    REQUIRE(ct.at({0,0}) == 10);
    REQUIRE(ct.at({1,1}) == 40);

    // Also ensure non-const and const see the same underlying storage
    t.at({0,1}) = 123;
    REQUIRE(ct.at({0,1}) == 123);
}

TEST_CASE("at() end-to-end: fill via at() then verify", "[tensor][at][fill]") {
    Tensor t({3,3}, std::vector<int>(9, 0));

    // Fill every element with f(i,j) = i*10 + j using at()
    for (std::size_t i = 0; i < 3; ++i) {
        for (std::size_t j = 0; j < 3; ++j) {
            t.at({i,j}) = static_cast<int>(i*10 + j);
        }
    }

    // Verify
    for (std::size_t i = 0; i < 3; ++i) {
        for (std::size_t j = 0; j < 3; ++j) {
            REQUIRE(t.at({i,j}) == static_cast<int>(i*10 + j));
        }
    }
}

TEST_CASE("at() sequential row-major traversal equals flat order", "[tensor][at][order]") {
    // shape 2 x 3 x 2, flat initialized as 0..11
    std::vector<int> flat(12);
    std::iota(flat.begin(), flat.end(), 0);
    Tensor t({2,3,2}, flat);

    // Traverse in row-major order and compare with flat
    std::size_t idx = 0;
    for (std::size_t i = 0; i < 2; ++i) {
        for (std::size_t j = 0; j < 3; ++j) {
            for (std::size_t k = 0; k < 2; ++k) {
                REQUIRE(t.at({i,j,k}) == flat[idx]);
                ++idx;
            }
        }
    }
}

TEST_CASE("at() multiple writes are reflected correctly", "[tensor][at][write]") {
    Tensor t({2,2,2}, std::vector<int>(8, 0));

    // Write with different patterns
    t.at({0,0,0}) = 1;
    t.at({0,1,1}) = 2;
    t.at({1,0,1}) = 3;
    t.at({1,1,0}) = 4;

    REQUIRE(t.at({0,0,0}) == 1);
    REQUIRE(t.at({0,1,1}) == 2);
    REQUIRE(t.at({1,0,1}) == 3);
    REQUIRE(t.at({1,1,0}) == 4);

    // Overwrite one and check again
    t.at({1,1,0}) = 42;
    REQUIRE(t.at({1,1,0}) == 42);
}

TEST_CASE("at() stress small grid: all coordinates valid, neighbors distinct", "[tensor][at][grid]") {
    // Small 3D grid
    std::vector<int> flat(3*3*3);
    std::iota(flat.begin(), flat.end(), 100); // start at 100 to make values obvious
    Tensor t({3,3,3}, flat);

    // Pick a middle point and compare neighbors to ensure different positions map differently
    REQUIRE(t.at({1,1,1}) != t.at({1,1,2}));
    REQUIRE(t.at({1,1,1}) != t.at({1,2,1}));
    REQUIRE(t.at({1,1,1}) != t.at({2,1,1}));
}
