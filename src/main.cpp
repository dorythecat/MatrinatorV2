#include <iostream>

#include "matrix.h"

int main() {
    Matrix<float, 3> mat1(std::function<int(size_t, size_t)>([](size_t i, size_t j) {
        return static_cast<int>(i / (j + 1) - j * i + 1);
    }));
    std::cout << "Matrix 1:\n" << mat1 << std::endl;
    std::cout << "Inverse of Matrix 1:\n" << mat1.inverse() << std::endl;
    return 0;
}