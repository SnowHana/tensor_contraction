#include "tensor_contraction.hpp"
#include <iostream>
#include <numeric>
#include <unordered_map>


TensorContraction::TensorContraction(const Tensor& m1, const Tensor& m2)
: A_(std::move(m1)), B_(std::move(m2)) {}

const Tensor& TensorContraction::A() const noexcept { return A_; }
const Tensor& TensorContraction::B() const noexcept { return B_; }
// const std::pair<size_t, size_t> TensorContraction::axis() const noexcept { return axis_; }

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
        // Check dA[i] exists in dim_map (ie. dB)
        if (auto it = dim_map.find(dA[i]); it != dim_map.end()) {
            for (auto j : it->second) {
                candids.emplace_back(i, j);
            }
        }
    }

    return candids;
}

Tensor TensorContraction::contract(std::optional<std::pair<size_t, size_t>> axes) const {
    // ...ik , ...kj => ...ij
    // pair = <n, m> (A.dim(n) == k, B.dim(m) == k)
    if (axes && validate_axis(axes.value())) {
        // Specified axis to contract on
        
    } else {
        // Without any axis of choice...
        std::vector<std::pair<size_t, size_t>> res = find_candid_axes();


        return;
    }
}

const std::vector<std::size_t>& TensorContraction::output_dims(std::pair<size_t, size_t> axis) const {
    // Calculate contracted Tensor's dimension - Assume axis are valid
    const auto& dA = A().dims();
    const auto& dB = B().dims();
    
    const size_t a = axis.first;
    const size_t b = axis.second;

    std::vector<size_t> out;
    out.reserve(dA.size() + dB.size() - 2);

    // A
    for (std::size_t i = 0 ; i < dA.size() ; ++i)
        if (i != a) out.push_back(dA[i]);
    // B
    for (std::size_t j = 0; j < dB.size(); ++j)
        if (j != b) out.push_back(dB[j]);

    return out;
}

Tensor TensorContraction::contract_on_axis(std::pair<size_t, size_t> axis) const {
    
    // Actual calculation
    auto& dims = output_dims(axis);



    const auto& dA = A().dims();
    const auto& dB = B().dims();
    for (size_t i = 0; i < dA.size(); ++i) {
        if (i != axis.first) {
            for (size_t j = 0; j < dB.size(); ++j) {
                if (j != axis.second) {
                    // Calculate single entry for result matrix
                }
            }
        }
    }
}