#include "tensor.hpp"
#include <numeric>
#include <iostream>




Tensor::Tensor(std::vector<std::size_t> dimensions, std::vector<int> flat)
: dims_(std::move(dimensions)), data_(std::move(flat)), rowMajor_(dims_)  {
    // Simple dimension check

    std::size_t total = 1;
    for (auto d : dims_) {
        total *= d;
    }


    if (total != data_.size()) {
        throw std::runtime_error("Size mismatch");
    }
}

// Setter
int& Tensor::at(const std::vector<std::size_t>& coord) {
    size_t i = rowMajor().toFlat(coord);
    return data_[i];
}




// Getter
const int& Tensor::at(const std::vector<std::size_t>& coord) const {
    size_t i = rowMajor().toFlat(coord);
    return data_[i];
}

const std::vector<std::size_t>& Tensor::dims() const { return dims_; }
const RowMajorLayout& Tensor::rowMajor() const { return rowMajor_; }

