#include <Eigen/Dense>
#include <iostream>

int main() {
    Eigen::Matrix2d mat_a;
    mat_a << 1, 2,
            3, 4;

    Eigen::Matrix2d mat_b;
    mat_b << 5, 6,
            7, 8;

    Eigen::Matrix2d mat_sum = mat_a + mat_b;
    Eigen::Matrix2d mat_product = mat_a * mat_b;

    std::cout << "Matrix A:\n" << mat_a << "\n\n";
    std::cout << "Matrix B:\n" << mat_b << "\n\n";
    std::cout << "Sum (A + B):\n" << mat_sum << "\n\n";
    std::cout << "Product (A * B):\n" << mat_product << "\n";
    return 0;
}
