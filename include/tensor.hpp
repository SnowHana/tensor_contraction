// Tensor (3D matrix based on Matrix)
//

#include <cstddef>
#include <vector>

class Tensor {
    public:
        Tensor(std::vector<std::size_t> dimensions, std::vector<int> flat);
        const int& at(std::vector<std::size_t>) const;
        int& at(std::vector<std::size_t>);
        std::size_t dims;
        std::vector<std::size_t> dims() const;
    private:
        std::vector<std::size_t> dims_{};
        std::vector<int> data_{};


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
