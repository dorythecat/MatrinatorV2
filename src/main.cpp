#include <iostream>

#include "matrix.h"

int main() {
    const Matrix<float, 3> mat1(std::function([](const size_t i, const size_t j) {
        return static_cast<int>(i / (j + 1) - j * i + 1);
    }));
    std::cout << "Matrix 1:\n" << mat1 << std::endl;
    std::cout << "Inverse of Matrix 1:\n" << mat1.inverse() << std::endl;
    return 0;
}