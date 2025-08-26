
#include "matrix.hpp"
#include <iostream>

Matrix::Matrix(std::size_t r, std::size_t c, std::vector<int> flat)
: rows_(r), cols_(c), data_(std::move(flat)) {
    if (data_.size() != rows_ * cols_) {
        throw std::runtime_error("size mismatch");
    }
}

const int& Matrix::at(size_t i, size_t j) const{
    if (i < 0 || j < 0 || i >= rows() || j >= cols()) {
        throw std::out_of_range("Index out of range");
    }
    return data_[i * cols() + j];
}

int& Matrix::at(size_t   i, size_t     j) {
    if (i < 0 || j < 0 || i >= rows() || j >= cols()) {
        throw std::out_of_range("Index out of range");
    }
    return data_[i * cols() + j];
}

std::size_t Matrix::rows() const { return rows_; }
std::size_t Matrix::cols() const { return cols_; }


// ---- MatrixCalculator
MatrixCalculator::MatrixCalculator(const Matrix& m1, const Matrix& m2) {
    if (m1.cols() == m2.rows()) {
        A_ = &m1; B_ = &m2;
    } else if (m1.rows() == m2.cols()) {
        A_ = &m2; B_ = &m1;
    } else {
        throw std::runtime_error("Matrix size mismatch");
    }
}

// Getter
const Matrix& MatrixCalculator::A() const {return *A_;}
const Matrix& MatrixCalculator::B() const {return *B_;}

Matrix MatrixCalculator::multiply() const {
    const Matrix& Amat = A();
    const Matrix& Bmat = B();

    Matrix C(Amat.rows(), Bmat.cols(), std::vector<int>(Amat.rows() * Bmat.cols(), 0));
    for (size_t i = 0; i < Amat.rows(); ++i) {
        for (size_t j = 0; j < Bmat.cols(); ++j) {
            int sum = 0;
            for (size_t k = 0; k < Amat.cols(); ++k) {
                sum += Amat.at(i, k) * Bmat.at(k, j);
            }
            C.at(i, j) = sum;
        }
    }

    return C;

}


// int main() {
//     // Testing...
//     std::vector<int> entries;
//     int value;

//     std::cout << "Enter integers: \n";
//     while (std::cin >> value) {
//         entries.push_back(value);
//     }

//     Matrix(3, 3, entries);




// }
