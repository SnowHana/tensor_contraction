#include <cstddef>
#include <vector>

class Matrix {
    public:
        Matrix(std::size_t r, std::size_t c, std::vector<int> flat);
        const int& at(size_t i, size_t j) const;
        int& at(size_t i, size_t j);
        std::size_t rows() const;
        std::size_t cols() const;
    private:
        std::size_t rows_{0};
        std::size_t cols_{0};
        std::vector<int> data_{};
};

class MatrixCalculator {
    public:
        MatrixCalculator(const Matrix& m1, const Matrix& m2);
        const Matrix& A() const;
        const Matrix& B() const;
        Matrix multiply() const;
    private:
        const Matrix* A_{nullptr};
        const Matrix* B_{nullptr};

};