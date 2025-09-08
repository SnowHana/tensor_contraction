#include "tensor_contraction.hpp"
#include <iostream>
#include <numeric>
#include <unordered_map>

TensorContraction::TensorContraction(const Tensor& m1, const Tensor& m2)
: A_(std::move(m1)), B_(std::move(m2)) {}

const Tensor& TensorContraction::A() const noexcept { return A_; }
const Tensor& TensorContraction::B() const noexcept { return B_; }

bool TensorContraction::validate_axis(std::pair<size_t, size_t> axes) const {
    // Check axis of given are fair
    const auto& dA = A().dims();
    const auto& dB = B().dims();

    if (axes.first >= dA.size() || axes.second >= dB.size()) {
        return false;
    }
    return dA[axes.first] == dB[axes.second];
}

std::vector<std::pair<size_t, size_t>> TensorContraction::find_candid_axes() const {
    // Find possible axis to contract on
    const auto& dA = A().dims();
    const auto& dB = B().dims();

    std::unordered_map<size_t, std::vector<std::size_t>> dim_map;
    dim_map.reserve(dB.size());
    for (size_t i = 0; i < dB.size(); ++i) {
        dim_map[dB[i]].push_back(i);
    }

    // Check it exists...
    std::vector<std::pair<size_t, size_t>> candids;

    for (size_t i = 0; i < dA.size(); ++i) {

        if (auto it = dim_map.find(dA[i]); it != dim_map.end()) {
            for (auto j : it->second) {
                candids.emplace_back(i, j);
            }
        }
    }

    return candids;
}

Tensor TensorContraction::contract(std::optional<std::pair<size_t, size_t>> axes = std::nullopt) const {
    // ...ik , ...kj => ...ij
    // pair = <n, m> (A.dim(n) == k, B.dim(m) == k)
    if (axes) {
        // Specified
        // Check given parameter is valid
        auto& dimsA = A().dims();
        auto& dimsB = B().dims();

    } else {
        // Without any axis of choice...
        
    }
}