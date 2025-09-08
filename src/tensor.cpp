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
// void Tensor::set_data(std::size_t i, int val) {
//     // Set data[i] to val
//     if (i >= data().size()) {
//         throw std::out_of_range("Index " + std::to_string(i) + " is out of bound.");
//     }
    
//     data_[i] = val;
// }

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
// const std::vector<int>& Tensor::data() const noexcept { return data_; }
// int Tensor::data(size_t i) const {
//     if (i >= data().size()) {
//         throw std::out_of_range("Index " + std::to_string(i) + " is out of bound.");
//     }

//     return data_[i];
// }


