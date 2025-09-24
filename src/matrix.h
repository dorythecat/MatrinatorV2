#ifndef MATRIX_H
#define MATRIX_H

template<unsigned int Rows, unsigned int Cols, typename T>
class Matrix {
public:
    // Basic definitions
    explicit Matrix(T value = T()) { for (unsigned int i = 0; i < Rows * Cols; ++i) data[i] = value; }
    ~Matrix() { free(data); }

    Matrix(const Matrix& other) {
        memcpy(data, other.data, sizeof(T) * Rows * Cols);
    }
    Matrix& operator=(const Matrix& other) {
        if (this != &other) memcpy(data, other.data, sizeof(T) * Rows * Cols);
        return *this;
    }

    // Element access with bounds checking
    T& operator()(const unsigned int row, const unsigned int col) {
        if (row >= Rows || col >= Cols) throw std::out_of_range("Matrix - Index out of range");
        return data[row * Cols + col];
    }
    const T& operator()(const unsigned int row, const unsigned int col) const {
        if (row >= Rows || col >= Cols) throw std::out_of_range("Matrix - Index out of range");
        return data[row * Cols + col];
    }

    // Element assignment
    void set(const unsigned int row, const unsigned int col, const T& value) {
        if (row >= Rows || col >= Cols) throw std::out_of_range("Matrix - Index out of range");
        data[row * Cols + col] = value;
    }

    // Size information
    [[nodiscard]] static unsigned int rows() { return Rows; }
    [[nodiscard]] static unsigned int cols() { return Cols; }

    // Comparison
    bool operator==(const Matrix& other) const {
        if (this == &other) return true;
        return memcmp(data, other.data, sizeof(T) * Rows * Cols) == 0;
    }
    bool operator!=(const Matrix& other) const { return !(*this == other); }

    // Output
    friend std::ostream& operator<<(std::ostream& os, Matrix m) {
        for (unsigned int i = 0; i < Rows; ++i) {
            for (unsigned int j = 0; j < Cols; ++j) {
                os << m.data[i * Cols + j] << " ";
            } os << "\n";
        } return os;
    }

private:
    T data[Rows * Cols];
};

#endif // MATRIX_H