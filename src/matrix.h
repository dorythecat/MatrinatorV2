#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <stdexcept>
#include <type_traits>
#include <limits>
#include <functional>

template<size_t Rows, size_t Cols, typename T>
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
        static_assert(std::is_arithmetic_v<U>, "Function return type must be arithmetic.");
        for (size_t i = 0; i < Rows * Cols; i++) data[i] = static_cast<T>(func());
    }
    template<typename U>
    explicit Matrix(std::function<U(size_t)> func) {
        static_assert(std::is_arithmetic_v<U>, "Function return type must be arithmetic.");
        for (size_t i = 0; i < Rows * Cols; i++) data[i] = static_cast<T>(func(i));
    }
    template<typename U>
    explicit Matrix(std::function<U(size_t, size_t)> func) {
        static_assert(std::is_arithmetic_v<U>, "Function return type must be arithmetic.");
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
    void set(const size_t row, const size_t col, const T& value) {
        assertRange(row, col);
        data[row * Cols + col] = value;
    }

    // Comparison
    template<size_t OtherRows, size_t OtherCols, typename U>
    bool operator==(const Matrix<OtherRows, OtherCols, U>& other) const {
        if (Rows != OtherRows || Cols != OtherCols) return false;
        if (this == reinterpret_cast<const void*>(&other)) return true;
        for (size_t i = 0; i < Rows * Cols; i++) if (data[i] != other.data[i]) return false;
        return true;
    }
    template<size_t OtherRows, size_t OtherCols, typename U>
    bool operator!=(const Matrix<OtherRows, OtherCols, U>& other) const { return !(*this == other); }

    // Addition
    Matrix operator+(const Matrix& other) const {
        Matrix result;
        for (size_t i = 0; i < Rows * Cols; i++) result.data[i] = data[i] + other.data[i];
        return result;
    }
    Matrix& operator+=(const Matrix& other) {
        for (size_t i = 0; i < Rows * Cols; i++) data[i] += other.data[i];
        return *this;
    }

    // Subtraction
    Matrix operator-(const Matrix& other) const {
        Matrix result;
        for (size_t i = 0; i < Rows * Cols; i++) result.data[i] = data[i] - other.data[i];
        return result;
    }
    Matrix& operator-=(const Matrix& other) {
        for (size_t i = 0; i < Rows * Cols; i++) data[i] -= other.data[i];
        return *this;
    }

    // Scalar multiplication
    template<typename U>
    Matrix operator*(const U& scalar) const {
        static_assert(std::is_arithmetic_v<U>, "Scalar type must be arithmetic.");
        Matrix result;
        for (size_t i = 0; i < Rows * Cols; i++) result.data[i] = data[i] * static_cast<T>(scalar);
        return result;
    }
    template<typename U>
    Matrix& operator*=(const U& scalar) {
        static_assert(std::is_arithmetic_v<U>, "Scalar type must be arithmetic.");
        for (size_t i = 0; i < Rows * Cols; i++) data[i] *= static_cast<T>(scalar);
        return *this;
    }

    // Scalar division
    template<typename U>
    Matrix operator/(const U& scalar) const {
        static_assert(std::is_arithmetic_v<U>, "Scalar type must be arithmetic.");
        Matrix result;
        for (size_t i = 0; i < Rows * Cols; i++) result.data[i] = data[i] / static_cast<T>(scalar);
        return result;
    }
    template<typename U>
    Matrix& operator/=(const U& scalar) {
        static_assert(std::is_arithmetic_v<U>, "Scalar type must be arithmetic.");
        for (size_t i = 0; i < Rows * Cols; i++) data[i] /= static_cast<T>(scalar);
        return *this;
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