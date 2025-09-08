#include "rowmajor.hpp"
#include <iostream>

RowMajorLayout::RowMajorLayout(std::vector<std::size_t> dimensions) 
: dims_(std::move(dimensions)) {
    // construct strides
    strides_.resize(dims_.size());
    
    std::size_t s = 1;
    for (std::size_t i = dims_.size(); i-- > 0;) {
        strides_[i] = s;
        s *= dims_[i];
    }
}

// Getters
const std::vector<std::size_t>& RowMajorLayout::dims() const noexcept { return dims_; }
const std::vector<std::size_t>& RowMajorLayout::strides() const noexcept { return strides_; }


size_t RowMajorLayout::toFlat(const std::vector<std::size_t>& coordinates) const {
    // coordinates (x1, x2, x3 , ... xn) => i (Index in flat vector)
    
    // Error check : Mismatch dimension length (i.e. : N dimensional vs N - 1 dimensional)
    if (coordinates.size() != dims().size() || coordinates.size() != strides().size()) {
        throw std::runtime_error("Dimension mismatch!");
    }

    size_t res = 0;

    for (size_t i = 0; i < strides().size(); ++i) {
        // Check...
        if (coordinates[i] >= dims(i)) {
            throw std::out_of_range("Invalid index!");
        }
        res += strides()[i] * coordinates[i];
    }

    return res;
}

std::size_t RowMajorLayout::rank() const noexcept { return dims_.size(); }
std::size_t RowMajorLayout::size() const noexcept { 
    std::size_t total = 1;
    for (auto d : dims_) {
        total *= d;
    }
    return total; 
}  
const std::size_t RowMajorLayout::dims(std::size_t k) const {
    if (k < 0 ||  k >= dims().size()) {
        throw std::out_of_range("Invalid index!");
    } 
    return dims_[k];
}
const std::size_t RowMajorLayout::strides(std::size_t k) const { 
    if (k < 0 ||  k >= dims().size()) {
        throw std::out_of_range("Invalid index!");
    } 
    return strides_[k];
}