// Tensor (3D matrix based on Matrix)
//

#include <cstddef>
#include <vector>
#include "rowmajor.hpp"

class Tensor {
    public:
        // Constructor
        Tensor(std::vector<std::size_t> dimensions, std::vector<int> flat);

        // Setter
        // int& at(std::vector<std::size_t>);
        // void set_data(std::size_t, int);
        int& at(const std::vector<std::size_t>& coord);
        

        // Getter
        const int& at(const std::vector<std::size_t>& coord) const;
        const std::vector<std::size_t>& dims() const;
        const RowMajorLayout& rowMajor() const;
        // const std::vector<int>& data() const noexcept;
        // int data(std::size_t) const;

    private:
        // dims_ = vector of dimension
        // e.g) 3 x 3 => {3, 3}, 2 x 3 x 4 => {2, 3, 4}
        std::vector<std::size_t> dims_{};
        
        // flat data
        std::vector<int> data_{};

    
        RowMajorLayout rowMajor_;
};

class TensorContraction {
    public:
        TensorContraction(const Tensor& m1, const Tensor& m2);
        const Tensor& A() const;
        const Tensor& B() const;
        Tensor contraction() const;
    private:
        const Tensor* A_{nullptr};
        const Tensor* B_{nullptr};
};
