#include <iostream>

#include "matrix.h"

int main() {
    Matrix<2, 3, int> mat1(5);

    std::cout << "Matrix 1:\n" << mat1 << std::endl;
    return 0;
}