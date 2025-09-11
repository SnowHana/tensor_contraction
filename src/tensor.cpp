#include "tensor.hpp"
#include <numeric>
#include <iostream>
#include <algorithm>




Tensor::Tensor(std::vector<std::size_t> dimensions, std::vector<int> flat)
: dims_(std::move(dimensions)), data_(std::move(flat)), rowMajor_(dims_)  {
    // Simple dimension check

    // std::size_t total = 1;
    // for (auto d : dims_) {
    //     total *= d;
    // }


    if (rowMajor_.size() != data_.size()) {
        throw std::runtime_error("Size mismatch");
    }
}

// TODO: Haven't tested this one ngl
Tensor::Tensor(std::vector<std::size_t> dimensions)
: dims_(std::move(dimensions)), rowMajor_(dims_) {
    // Populate data with random numbers
    data_.resize(rowMajor_.size());
    generate(data_.begin(), data_.end(), []() {
        return rand() % 100;
    }); 
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

