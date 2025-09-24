#ifndef MATRIX_H
#define MATRIX_H

template<unsigned int Rows, unsigned int Cols, typename T>
class Matrix {
public:
    T data[Rows * Cols];

    // Basic definitions
    explicit Matrix(T value = T()) { for (unsigned int i = 0; i < Rows * Cols; i++) data[i] = value; }

    // Size information
    [[nodiscard]] static unsigned int rows() { return Rows; }
    [[nodiscard]] static unsigned int cols() { return Cols; }

    // Miscellaneous utils
    [[nodiscard]] static bool isSquare() { return Rows == Cols; }
    [[nodiscard]] bool zero() const {
        for (unsigned int i = 0; i < Rows * Cols; i++) if (data[i] != T()) return false;
        return true;
    }
    [[nodiscard]] bool identity() const {
        if (!isSquare()) return false;
        for (unsigned int i = 0; i < Rows; i++) {
            for (unsigned int j = 0; j < Cols; j++) {
                if (i == j && data[i * Cols + j] != T(1)) return false;
                if (i != j && data[i * Cols + j] != T()) return false;
            }
        } return true;
    }

    // Element access with bounds checking
    T& operator()(const unsigned int row, const unsigned int col) {
        assertRange(row, col);
        return data[row * Cols + col];
    }
    const T& operator()(const unsigned int row, const unsigned int col) const {
        assertRange(row, col);
        return data[row * Cols + col];
    }

    // Set element
    void set(const unsigned int row, const unsigned int col, const T& value) {
        assertRange(row, col);
        data[row * Cols + col] = value;
    }

    // Comparison
    template<unsigned int OtherRows, unsigned int OtherCols, typename U>
    bool operator==(const Matrix<OtherRows, OtherCols, U>& other) const {
        if (Rows != OtherRows || Cols != OtherCols) return false;
        if (this == reinterpret_cast<const void*>(&other)) return true;
        for (unsigned int i = 0; i < Rows * Cols; i++) if (data[i] != other.data[i]) return false;
        return true;
    }
    template<unsigned int OtherRows, unsigned int OtherCols, typename U>
    bool operator!=(const Matrix<OtherRows, OtherCols, U>& other) const { return !(*this == other); }

    // Output
    friend std::ostream& operator<<(std::ostream& os, Matrix m) {
        for (unsigned int i = 0; i < Rows; i++) {
            for (unsigned int j = 0; j < Cols; j++) {
                os << m.data[i * Cols + j] << " ";
            } os << "\n";
        } return os;
    }

private:
    // Range assertion
    static void assertRange(const unsigned int row, const unsigned int col) {
        if (row >= Rows || col >= Cols) throw std::out_of_range("Matrix - Index out of range");
    }
};

#endif // MATRIX_H