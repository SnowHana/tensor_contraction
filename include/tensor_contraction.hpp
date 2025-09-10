#include "tensor.hpp"
#include <optional>
class TensorContraction {
    public:
        TensorContraction(const Tensor& m1, const Tensor& m2);
        // Getter
        const Tensor& A() const noexcept;
        const Tensor& B() const noexcept;
        // const std::pair<size_t, size_t> axis() const noexcept;

        // Contract with optional axis
        Tensor contract(std::optional<std::pair<size_t, size_t>>) const;
    private:
        const Tensor A_;
        const Tensor B_;
        // const std::pair<size_t, size_t> axis_;
        
        // ------- Finding Axis --------
        // Check given contract axis are valid
        bool validate_axis(std::pair<size_t, size_t> axes) const;
        // Find axis to contract on
        std::vector<std::pair<size_t, size_t>> find_candid_axes() const;
        
        // Calculate result dims
        const std::vector<std::size_t>& output_dims(std::pair<size_t, size_t> axis) const;
        // Contraction calculation
        Tensor contract_on_axis(std::pair<size_t, size_t>) const;



};