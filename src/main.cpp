#include <iostream>

#include "matrix.h"

int main() {
    Matrix<int, 7> mat1(std::function<int(size_t, size_t)>([](size_t i, size_t j) {
        return static_cast<int>(i * 3 + j * i);
    }));
    std::cout << "Matrix 1:\n" << mat1 << std::endl;
    return 0;
}