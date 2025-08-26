#include "rowmajor.hpp"

RowMajorLayout::RowMajorLayout(std::vector<std::size_t> dimensions) 
: dims_(std::move(dimensions)) {
    // construct strides
    strides_.resize(dims_.size());
    
    size_t s = 1;
    for (size_t i = dims_.size() - 1; i >= 0; --i) {
        strides_[i] = s;
        s *= dims_[i];
    }
}

const std::vector<std::size_t> RowMajorLayout::dims() const {
    return dims_;
}

const std::vector<std::size_t> 


int toFlat(std::vector<std::size_t> coordinates) {
    if (coordinates.size() != dims().size())
}