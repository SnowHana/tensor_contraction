#include <vector>

class RowMajorLayout {
    public:
        RowMajorLayout(std::vector<std::size_t> dimensions);
        int toFlat(std::vector<std::size_t>);
        const std::vector<std::size_t> dims() const;
        const std::vector<std::size_t> strides() const;
    private:
        std::vector<std::size_t> dims_{};
        std::vector<std::size_t> strides_{};

};
