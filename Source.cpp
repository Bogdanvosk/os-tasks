#include <iostream>
#include <thread>
#include <time.h>
#include <chrono>
#include <vector>


void fillMatrix(int** matrix, int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        matrix[i] = new int[cols];
        for (int j = 0; j < cols; ++j) {
            std::cout << "Enter value for matrix[" << i << "][" << j << "]: ";
            std::cin >> matrix[i][j];
        }
    }
}

void mulMat(int** mat1, int rows1, int cols1, int** mat2, int rows2, int cols2, int** result) {

    for (int i = 0; i < rows1; ++i) {
        for (int j = 0; j < cols2; ++j) {
            result[i][j] = 0;
            for (int k = 0; k < cols1; ++k) {
                result[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }
    
}

void mulMatTh(int** mat1, int rows1, int cols1, int** mat2, int rows2, int cols2, int row, int** result)
{

    for (int i = 0; i < rows1; i++) {
        result[row][i] = 0;
        for (int j = 0; j < cols2; j++) {
            result[row][i] += mat1[row][j] * mat2[j][i];
        }
    }
}

int main()
{
    int rows1, cols1, rows2, cols2;

    std::cout << "Enter the number of rows and columns for matrix 1: ";
    std::cin >> rows1 >> cols1;

    std::cout << "Enter the number of rows and columns for matrix 2: ";
    std::cin >> rows2 >> cols2;

    int** mat1 = new int* [rows1];
    int** mat2 = new int* [rows2];

    std::cout << "Enter values for matrix 1:" << std::endl;
    fillMatrix(mat1, rows1, cols1);

    std::cout << "Enter values for matrix 2:" << std::endl;
    fillMatrix(mat2, rows2, cols2);


    int** result1 = new int* [rows1];
    for (int i = 0; i < rows1; ++i) {
        result1[i] = new int[cols2];
    }

    int** result2 = new int* [rows1];
    for (int i = 0; i < rows1; ++i) {
        result2[i] = new int[cols2];
    }

    
    std::vector<std::thread> threads;

    auto start1 = std::chrono::system_clock::now();

    for (int i = 0; i < rows1; ++i) {
        threads.emplace_back([i, mat1, rows1, cols1, mat2, rows2, cols2, &result1]() {
            mulMatTh(mat1, rows1, cols1, mat2, rows2, cols2, i, result1);
            });
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "Result of matrix multiplication:" << std::endl;
    for (int i = 0; i < rows1; ++i) {
        for (int j = 0; j < cols2; ++j) {
            std::cout << result1[i][j] << " ";
        }
        std::cout << std::endl;
    }

    // Free memory for result matrix
    for (int i = 0; i < rows1; ++i) {
        delete[] result1[i];
    }
    delete[] result1;

    auto end1 = std::chrono::system_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1).count() << "ms with multi" << std::endl;



    auto start2 = std::chrono::system_clock::now();
    mulMat(mat1, rows1, cols1, mat2, rows2, cols2, result2);
    auto end2 = std::chrono::system_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end2 - start2).count() << "ms without multi" << std::endl;

    std::cout << "Result of matrix multiplication:" << std::endl;
    for (int i = 0; i < rows1; ++i) {
        for (int j = 0; j < cols2; ++j) {
            std::cout << result2[i][j] << " ";
        }
        std::cout << std::endl;
    }

    // Free memory for result matrix
    for (int i = 0; i < rows1; ++i) {
        delete[] result2[i];
    }
    delete[] result2;

    return 0;
}
