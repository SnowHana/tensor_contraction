#include "tensor.hpp"
#include <optional>
class TensorContraction {
    public:
        TensorContraction(const Tensor& m1, const Tensor& m2);
        // Getter
        const Tensor& A() const noexcept;
        const Tensor& B() const noexcept;

        // Contract with optional axis
        Tensor contract(std::optional<std::pair<size_t, size_t>>) const;
    private:
        const Tensor A_;
        const Tensor B_;
        
        // Check given contract axis are valid
        bool validate_axis(std::pair<size_t, size_t> axes) const;
        // Find axis to contract on
        std::vector<std::pair<size_t, size_t>> find_candid_axes() const;




};