#include "tensor.hpp"
#include <numeric>
#include <iostream>




Tensor::Tensor(std::vector<std::size_t> dimensions, std::vector<int> flat)
: dims_(std::move(dimensions)), data_(std::move(flat))  {
    // Simple dimension check
    if (std::accumulate(dims_.begin(), dims_.end(), 1, std::multiplies<int>()) != data_.size()) {
        throw std::runtime_error("Size mismatch");
    }   
}

const int& Tensor::at(std::vector<std::size_t>) const {
    // Getter
}


