#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <stdexcept>
#include <type_traits>
#include <limits>
#include <functional>

template<typename T, size_t Rows, size_t Cols = Rows>
class Matrix {
    static_assert(Rows > 0 && Cols > 0, "Matrix dimensions must be greater than zero.");
    static_assert(std::is_arithmetic_v<T>, "Matrix type must be arithmetic.");
    static_assert(Rows * Cols <= std::numeric_limits<size_t>::max(), "Matrix size exceeds maximum limit.");
public:
    T data[Rows * Cols];

    // Basic definitions
    explicit Matrix(bool zero = false) { // Identity if square, zero otherwise
        if (zero || Rows != Cols) for (size_t i = 0; i < Rows * Cols; i++) data[i] = T();
        else for (size_t i = 0; i < Rows; i++) data[i * Cols + i] = T(1);
    }
    explicit Matrix(T value = T()) { for (size_t i = 0; i < Rows * Cols; i++) data[i] = value; }
    template<typename U>
    explicit Matrix(std::function<U()> func) {
        static_assert(std::is_convertible_v<U, T>, "Function return type must be convertible to matrix type.");
        for (size_t i = 0; i < Rows * Cols; i++) data[i] = static_cast<T>(func());
    }
    template<typename U>
    explicit Matrix(std::function<U(size_t)> func) {
        static_assert(std::is_convertible_v<U, T>, "Function return type must be convertible to matrix type.");
        for (size_t i = 0; i < Rows * Cols; i++) data[i] = static_cast<T>(func(i));
    }
    template<typename U>
    explicit Matrix(std::function<U(size_t, size_t)> func) {
        static_assert(std::is_convertible_v<U, T>, "Function return type must be convertible to matrix type.");
        for (size_t i = 0; i < Rows * Cols; i++) data[i] = static_cast<T>(func(i, static_cast<size_t>(i / Cols)));
    }

    // Size information
    [[nodiscard]] static size_t rows() { return Rows; }
    [[nodiscard]] static size_t cols() { return Cols; }

    // Miscellaneous utils
    [[nodiscard]] static bool isSquare() { return Rows == Cols; }
    [[nodiscard]] bool zero() const {
        for (size_t i = 0; i < Rows * Cols; i++) if (data[i] != T()) return false;
        return true;
    }
    [[nodiscard]] bool identity() const {
        if (!isSquare()) return false;
        for (size_t i = 0; i < Rows; i++) {
            for (size_t j = 0; j < Cols; j++) {
                if (i == j && data[i * Cols + j] != T(1)) return false;
                if (i != j && data[i * Cols + j] != T()) return false;
            }
        } return true;
    }

    // Element access with bounds checking
    T& operator()(const size_t row, const size_t col) {
        assertRange(row, col);
        return data[row * Cols + col];
    }
    const T& operator()(const size_t row, const size_t col) const {
        assertRange(row, col);
        return data[row * Cols + col];
    }

    // Set element
    template<typename U>
    Matrix set(const size_t row, const size_t col, const U& value) {
        static_assert(std::is_convertible_v<U, T>, "Value type must be convertible to matrix type.");
        assertRange(row, col);
        data[row * Cols + col] = static_cast<T>(value);
        return *this;
    }

    // Comparison
    template<typename U, size_t AltRows, size_t AltCols = AltRows>
    bool operator==(const Matrix<U, AltRows, AltCols>& other) const {
        static_assert(std::is_convertible_v<U, T>, "Matrix types must be convertible for comparison.");
        if (Rows != AltRows || Cols != AltCols) return false;
        if (this == reinterpret_cast<const void*>(&other)) return true;
        for (size_t i = 0; i < Rows * Cols; i++) if (data[i] != static_cast<T>(other.data[i])) return false;
        return true;
    }
    template<typename U, size_t AltRows, size_t AltCols = AltRows>
    bool operator!=(const Matrix<U, AltRows, AltCols>& other) const { return !(*this == other); }

    // Addition
    Matrix operator+(const Matrix& other) const {
        Matrix result;
        for (size_t i = 0; i < Rows * Cols; i++) result.data[i] = data[i] + other.data[i];
        return result;
    }
    Matrix& operator+=(const Matrix& other) {
        *this = *this + other;
        return *this;
    }

    // Subtraction
    Matrix operator-(const Matrix& other) const {
        Matrix result;
        for (size_t i = 0; i < Rows * Cols; i++) result.data[i] = data[i] - other.data[i];
        return result;
    }
    Matrix& operator-=(const Matrix& other) {
        *this = *this - other;
        return *this;
    }

    // Scalar addition
    template<typename U>
    Matrix operator+(const U& scalar) const {
        static_assert(std::is_convertible_v<U, T>, "Scalar type must be convertible to matrix type.");
        Matrix result;
        for (size_t i = 0; i < Rows * Cols; i++) result.data[i] = data[i] + static_cast<T>(scalar);
        return result;
    }
    template<typename U>
    Matrix& operator+=(const U& scalar) {
        static_assert(std::is_convertible_v<U, T>, "Scalar type must be convertible to matrix type.");
        *this = *this + scalar;
        return *this;
    }

    // Scalar subtraction
    template<typename U>
    Matrix operator-(const U& scalar) const {
        static_assert(std::is_convertible_v<U, T>, "Scalar type must be convertible to matrix type.");
        Matrix result;
        for (size_t i = 0; i < Rows * Cols; i++) result.data[i] = data[i] - static_cast<T>(scalar);
        return result;
    }
    template<typename U>
    Matrix& operator-=(const U& scalar) {
        static_assert(std::is_convertible_v<U, T>, "Scalar type must be convertible to matrix type.");
        *this = *this - scalar;
        return *this;
    }

    // Scalar multiplication
    template<typename U>
    Matrix operator*(const U& scalar) const {
        static_assert(std::is_convertible_v<U, T>, "Scalar type must be convertible to matrix type.");
        Matrix result;
        for (size_t i = 0; i < Rows * Cols; i++) result.data[i] = data[i] * static_cast<T>(scalar);
        return result;
    }
    template<typename U>
    Matrix& operator*=(const U& scalar) {
        static_assert(std::is_convertible_v<U, T>, "Scalar type must be convertible to matrix type.");
        *this = *this * scalar;
        return *this;
    }

    // Scalar division
    template<typename U>
    Matrix operator/(const U& scalar) const {
        static_assert(std::is_convertible_v<U, T>, "Scalar type must be convertible to matrix type.");
        Matrix result;
        for (size_t i = 0; i < Rows * Cols; i++) result.data[i] = data[i] / static_cast<T>(scalar);
        return result;
    }
    template<typename U>
    Matrix& operator/=(const U& scalar) {
        static_assert(std::is_convertible_v<U, T>, "Scalar type must be convertible to matrix type.");
        *this = *this / scalar;
        return *this;
    }

    // Matrix multiplication
    template<typename U, size_t AltCols>
    Matrix<T, Rows, AltCols> operator*(const Matrix<U, Cols, AltCols>& other) const {
        static_assert(std::is_convertible_v<U, T>, "Matrix types must be convertible for multiplication.");
        Matrix<T, Rows, AltCols> result;
        for (size_t i = 0; i < Rows; i++) {
            for (size_t j = 0; j < AltCols; j++) {
                for (size_t k = 0; k < Cols; k++) {
                    result.data[i * AltCols + j] += data[i * Cols + k] * static_cast<T>(other.data[k * AltCols + j]);
                }
            }
        } return result;
    }
    template<typename U, size_t AltCols>
    Matrix<T, Rows, AltCols>& operator*=(const Matrix<U, Cols, AltCols>& other) {
        static_assert(std::is_convertible_v<U, T>, "Matrix types must be convertible for multiplication.");
        *this = *this * other;
        return *this;
    }

    // Matrix division
    template<typename U, size_t AltCols>
    Matrix<T, Rows, AltCols> operator/(const Matrix<U, Cols, AltCols>& other) const {
        static_assert(std::is_convertible_v<U, T>, "Matrix types must be convertible for division.");
        Matrix<T, Rows, AltCols> result;
        for (size_t i = 0; i < Rows; i++) {
            for (size_t j = 0; j < AltCols; j++) {
                for (size_t k = 0; k < Cols; k++) {
                    if (other.data[k * AltCols + j] == U())
                        throw std::domain_error("Matrix - Division by zero in matrix division.");
                    result.data[i * AltCols + j] += data[i * Cols + k] / static_cast<T>(other.data[k * AltCols + j]);
                }
            }
        } return result;
    }
    template<typename U, size_t AltCols>
    Matrix<T, Rows, AltCols>& operator/=(const Matrix<U, Cols, AltCols>& other) {
        static_assert(std::is_convertible_v<U, T>, "Matrix types must be convertible for division.");
        *this = *this / other;
        return *this;
    }

    // Matrix per-element inversion (1/M)
    Matrix operator!() const {
        Matrix result;
        for (size_t i = 0; i < Rows * Cols; i++) {
            if (data[i] == T()) throw std::domain_error("Matrix - Division by zero in matrix inversion.");
            result.data[i] = T(1) / data[i];
        } return result;
    }

    // Matrix transposition
    Matrix<T, Cols, Rows> transpose() const {
        Matrix<T, Cols, Rows> result;
        for (size_t i = 0; i < Rows; i++)
            for (size_t j = 0; j < Cols; j++) result.data[j * Rows + i] = data[i * Cols + j];
        return result;
    }

    // Switch two rows
    Matrix switchRows(const size_t r1, const size_t r2) {
        assertRange(r1, 0);
        assertRange(r2, 0);
        for (size_t i = 0; i < Cols; i++) std::swap(data[r1 * Cols + i], data[r2 * Cols + i]);
        return *this;
    }

    // Multiplication of row by scalar
    template<typename U>
    Matrix multiplyRow(const size_t row, const U mul) {
        static_assert(std::is_convertible_v<U, T>, "Scalar type must be convertible to matrix type.");
        assertRange(row, 0);
        for (size_t i = row * Cols; i < row * Cols + Cols; i++) data[i] = data[i] * static_cast<T>(mul);
        return *this;
    }

    // Multiplication of row by scalar and addition to another row
    template<typename U>
    Matrix linearAddRows(const size_t r1, const size_t r2, const U mul) {
        static_assert(std::is_convertible_v<U, T>, "Scalar type must be convertible to matrix type.");
        assertRange(r1, 0);
        assertRange(r2, 0);
        for (size_t i = 0; i < Cols; i++) {
            data[r2 * Cols + i] += data[r1 * Cols + i] * static_cast<T>(mul);
            if (data[r2 * Cols + i] == T()) data[r2 * Cols + i] = T(); // Avoid -0
        }
        return *this;
    }

    // Row Echelon form
    Matrix rowEchelon() {
        for (size_t i = 0; i < Rows; i++) {
            // Find pivot
            size_t pivotRow = i;
            while (pivotRow < Rows && data[pivotRow * Cols + i] == T()) pivotRow++;
            if (pivotRow == Rows) continue; // No pivot in this column

            // Swap to current row
            if (pivotRow != i) switchRows(i, pivotRow);

            // Eliminate below
            for (size_t j = i + 1; j < Rows; j++)
                if (data[j * Cols + i] != T()) linearAddRows(i, j, -data[j * Cols + i]);
        } return *this;
    }

    // Reduced Row Echelon form
    Matrix reducedRowEchelon() {
        rowEchelon();
        for (int i = static_cast<int>(Rows) - 1; i >= 0; i--) {
            // Find pivot
            size_t pivotCol = 0;
            while (pivotCol < Cols && data[i * Cols + pivotCol] == T()) pivotCol++;
            if (pivotCol == Cols) continue; // No pivot in this row
            // Normalize pivot
            multiplyRow(i, T(1) / data[i * Cols + pivotCol]);
            // Eliminate above
            for (int j = i - 1; j >= 0; j--)
                if (data[j * Cols + pivotCol] != T()) linearAddRows(i, j, -data[j * Cols + pivotCol]);
        } return *this;
    }

    // Inverse
    Matrix inverse() const {
        static_assert(Rows == Cols, "Inverse is only defined for square matrices.");
        Matrix<T, Rows, 2 * Cols> augmented(true);
        for (size_t i = 0; i < Rows; i++) {
            for (size_t j = 0; j < Cols; j++) augmented.data[i * (2 * Cols) + j] = data[i * Cols + j];
            augmented.data[i * (2 * Cols) + (Cols + i)] = T(1);
        }
        augmented.reducedRowEchelon();
        Matrix result(true);
        for (size_t i = 0; i < Rows; i++)
            for (size_t j = 0; j < Cols; j++) result.data[i * Cols + j] = augmented.data[i * (2 * Cols) + (Cols + j)];
        return result;
    }

    // Determinant
    T determinant() const {
        static_assert(Rows == Cols, "Determinant is only defined for square matrices.");
        Matrix temp = *this;
        temp.rowEchelon();
        T det = T(1);
        for (size_t i = 0; i < Rows; i++) det *= temp.data[i * Cols + i];
        return det;
    }

    // Rank
    T rank() const {
        Matrix temp = *this;
        temp.rowEchelon();
        size_t rank = 0;
        for (size_t i = 0; i < Rows; i++) {
            for (size_t j = 0; j < Cols; j++) {
                if (temp.data[i * Cols + j] == T()) continue;
                rank++;
                break;
            }
        } return rank;
    }

    // Output
    friend std::ostream& operator<<(std::ostream& os, Matrix m) {
        for (size_t i = 0; i < Rows; i++) {
            for (size_t j = 0; j < Cols; j++) os << m.data[i * Cols + j] << " ";
            os << "\n";
        } return os;
    }

private:
    // Range assertion
    static void assertRange(const size_t row, const size_t col) {
        if (row >= Rows || col >= Cols) throw std::out_of_range("Matrix - Index out of range");
    }
};

#endif // MATRIX_H