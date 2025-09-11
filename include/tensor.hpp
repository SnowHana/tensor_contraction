// Tensor (3D matrix based on Matrix)
//

#include <cstddef>
#include <vector>
#include "rowmajor.hpp"

class Tensor {
    public:
        // Constructor
        Tensor(std::vector<std::size_t> dimensions, std::vector<int> flat);
        Tensor(std::vector<std::size_t> dimensions);

        // Setter
        int& at(const std::vector<std::size_t>& coord);
        

        // Getter
        const int& at(const std::vector<std::size_t>& coord) const;
        const std::vector<std::size_t>& dims() const;
        const RowMajorLayout& rowMajor() const;

    private:
        // dims_ = vector of dimension
        // e.g) 3 x 3 => {3, 3}, 2 x 3 x 4 => {2, 3, 4}
        std::vector<std::size_t> dims_{};
        
        // flat data
        std::vector<int> data_{};

        // RowMajorLayout : Handles (x1, x2, x3 ... ) => i (1D index)
        RowMajorLayout rowMajor_;
};
