#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "tensor.hpp"
#include "tensor_contraction.hpp"   // your class header

#include <vector>
#include <utility>

// Helper to make a dummy Tensor with just shape + flat data.
template<typename T=int>
Tensor makeTensor(const std::vector<std::size_t>& shape, T start=0) {
    std::size_t total = 1;
    for (auto d : shape) total *= d;
    std::vector<T> flat(total);
    for (std::size_t i=0; i<total; ++i) flat[i] = static_cast<T>(start + i);
    return Tensor(shape, flat);
}

TEST_CASE("candidate axes: unique match", "[helpers]") {
    Tensor A = makeTensor<int>({2,3,4});
    Tensor B = makeTensor<int>({2,8});
    TensorContraction tc(A,B);

    auto cand = tc.find_candid_axes();
    REQUIRE(cand.size() == 1);
    REQUIRE(cand[0] == std::pair<std::size_t,std::size_t>{0,0});

    // validate_axis true for the unique match
    REQUIRE(tc.validate_axis({0,0}) == true);

    // and false for a non-match
    REQUIRE(tc.validate_axis({1,0}) == false);
}

TEST_CASE("candidate axes: multiple matches", "[helpers]") {
    Tensor A = makeTensor<int>({2,3,4});
    Tensor B = makeTensor<int>({3,4,5});
    TensorContraction tc(A,B);

    auto cand = tc.find_candid_axes();
    // Expected matches: (A[1]=3 ↔ B[0]=3), (A[2]=4 ↔ B[1]=4)
    REQUIRE(cand.size() == 2);
    REQUIRE((cand[0] == std::pair<std::size_t,std::size_t>{1,0}
          || cand[1] == std::pair<std::size_t,std::size_t>{1,0}));
    REQUIRE((cand[0] == std::pair<std::size_t,std::size_t>{2,1}
          || cand[1] == std::pair<std::size_t,std::size_t>{2,1}));

    REQUIRE(tc.validate_axis({1,0}) == true);
    REQUIRE(tc.validate_axis({2,1}) == true);
}

TEST_CASE("candidate axes: no matches", "[helpers]") {
    Tensor A = makeTensor<int>({2,3});
    Tensor B = makeTensor<int>({5,7});
    TensorContraction tc(A,B);

    auto cand = tc.find_candid_axes();
    REQUIRE(cand.empty());

    // validate_axis should be false for any in-range pair with mismatched dims
    REQUIRE(tc.validate_axis({0,0}) == false);
    REQUIRE(tc.validate_axis({1,1}) == false);
}

TEST_CASE("validate_axis: bounds checking", "[helpers]") {
    Tensor A = makeTensor<int>({2,3});
    Tensor B = makeTensor<int>({2});
    TensorContraction tc(A,B);

    // In-range and equal → true
    REQUIRE(tc.validate_axis({0,0}) == true);

    // Out-of-range indices must return false (not crash)
    REQUIRE(tc.validate_axis({2,0}) == false); // A index out of range
    REQUIRE(tc.validate_axis({0,1}) == false); // B index out of range
}

TEST_CASE("candidate axes: duplicates on both sides", "[helpers]") {
    Tensor A = makeTensor<int>({2,2});
    Tensor B = makeTensor<int>({2,2});
    TensorContraction tc(A,B);

    auto cand = tc.find_candid_axes();
    // All combinations where dims equal = 2 → 4 pairs expected
    // (0,0), (0,1), (1,0), (1,1)
    REQUIRE(cand.size() == 4);
    REQUIRE(std::find(cand.begin(), cand.end(), std::pair<std::size_t,std::size_t>{0,0}) != cand.end());
    REQUIRE(std::find(cand.begin(), cand.end(), std::pair<std::size_t,std::size_t>{0,1}) != cand.end());
    REQUIRE(std::find(cand.begin(), cand.end(), std::pair<std::size_t,std::size_t>{1,0}) != cand.end());
    REQUIRE(std::find(cand.begin(), cand.end(), std::pair<std::size_t,std::size_t>{1,1}) != cand.end());
}
