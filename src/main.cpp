#include <iostream>

#include "matrix.h"

int main() {
    Matrix<2, 3, int> mat1(5);

    std::cout << "Matrix 1:\n" << mat1 << std::endl;

    mat1.set(1, 2, 3);

    std::cout << "Matrix 1 after setting element (1,2) to 3:\n" << mat1 << std::endl;
    return 0;
}