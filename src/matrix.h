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
        for (size_t i = 0; i < Rows; i++) {
            for (size_t j = 0; j < Cols; j++) result.data[j * Rows + i] = data[i * Cols + j];
        } return result;
    }

    // Multiplication of row by scalar
    template<typename U>
    Matrix rowMul(const size_t row, U mul) {
        static_assert(std::is_convertible_v<U, T>, "Scalar type must be convertible to matrix type.");
        assertRange(row, 0);
        Matrix result;
        for (size_t i = row * Cols; i < (row + 1) * Cols; i++) result.data[i] = data[i] * static_cast<T>(mul);
        return result;
    }

    // Multiplication of row by scalar and addition to another row
    template<typename U>
    Matrix rowMulAdd(const size_t srcRow, const size_t destRow, U mul) {
        static_assert(std::is_convertible_v<U, T>, "Scalar type must be convertible to matrix type.");
        assertRange(srcRow, 0);
        assertRange(destRow, 0);
        Matrix result = *this;
        for (size_t i = 0; i < Cols; i++)
            result.data[destRow * Cols + i] += data[srcRow * Cols + i] * static_cast<T>(mul);
        return result;
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