#ifndef MATRIX_H
#define MATRIX_H

template<unsigned int Rows, unsigned int Cols, typename T>
class Matrix {
public:
    explicit Matrix(T value = T()) { for (unsigned int i = 0; i < Rows * Cols; ++i) data[i] = value; }
    ~Matrix() { free(data); }

    Matrix(const Matrix& other) {
        memcpy(data, other.data, sizeof(T) * Rows * Cols);
    }
    Matrix& operator=(const Matrix& other) {
        if (this != &other) memcpy(data, other.data, sizeof(T) * Rows * Cols);
        return *this;
    }

    T& operator()(const unsigned int row, const unsigned int col) { return data[row * Cols + col]; }
    const T& operator()(const unsigned int row, const unsigned int col) const { return data[row * Cols + col]; }


    [[nodiscard]] static unsigned int rows() { return Rows; }
    [[nodiscard]] static unsigned int cols() { return Cols; }

    bool operator==(const Matrix& other) const {
        if (this == &other) return true;
        return memcmp(data, other.data, sizeof(T) * Rows * Cols) == 0;
    }
    bool operator!=(const Matrix& other) const { return !(*this == other); }

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