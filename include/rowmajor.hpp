#include <vector>

class RowMajorLayout {
    public:
        // Constructor
        RowMajorLayout(std::vector<std::size_t> dimensions);

        // Getter
        const std::vector<std::size_t>& dims() const noexcept;
        const std::vector<std::size_t>& strides() const noexcept;
        
        std::size_t rank() const noexcept;  // n for n-dimensional (ie. 2 x 3 x 4 => rank == 3)
        std::size_t size() const noexcept;  // sum(entries number) (ie. 2 x 3 x 4 => size == 24)
        
        const std::size_t dims(std::size_t k) const;
        const std::size_t strides(std::size_t k) const;

        // coordinates -> 1D index i
        size_t toFlat(const std::vector<std::size_t>&) const;
    private:
        // Attr.
        std::vector<std::size_t> dims_{};
        std::vector<std::size_t> strides_{};

};
