#include <iostream>
#include <thread>
#include <time.h>
#include <chrono>
#include <vector>

#define N 3

#define R1 N // number of rows in Matrix-1
#define C1 N // number of columns in Matrix-1
#define R2 N // number of rows in Matrix-2
#define C2 N // number of columns in Matrix-2

int res[R1][C2];

void mulMat(int mat1[][C1], int mat2[][C2])
{
    std::cout << "Multiplication of given two matrices is:\n";

    for (int i = 0; i < R1; i++) {
        for (int j = 0; j < C2; j++) {
            res[i][j] = 0;

            for (int k = 0; k < R2; k++) {
                res[i][j] += mat1[i][k] * mat2[k][j];
            }

            std::cout << res[i][j] << "\t";
        }

        std::cout << std::endl;
    }
}

void mulMatTh(int mat1[][C1], int mat2[][C2], int row)
{
    std::cout << "Multiplication of given two matrices is:\n";

    for (int i = 0; i < R1; i++) {
        res[row][i] = 0;
        for (int j = 0; j < C2; j++) {
            res[row][i] += mat1[row][j] * mat2[j][i];
        }
    }
}

int main()
{
    int mat1[R1][C1] = { { 1, 1, 1 }, { 2, 2, 2 }, { 3, 3, 3 } };

    int mat2[R2][C2] = { { 1, 1, 1 }, { 2, 2, 2 }, { 3, 3, 3 } };

    std::vector<std::thread> threads;

    auto start1 = std::chrono::system_clock::now();
    for (int i = 0; i < N; ++i) {
        threads.emplace_back( [i, &mat1, &mat2]() { mulMatTh(mat1, mat2, i); });
    }

    for (auto& t : threads) {
        t.join();
    }

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            std::cout << res[i][j] << " ";
        }
        std::cout << std::endl;
    }
    auto end1 = std::chrono::system_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1).count() << "ms with multi" << std::endl;

    auto start2 = std::chrono::system_clock::now();
    mulMat(mat1, mat2);
    auto end2 = std::chrono::system_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end2 - start2).count() << "ms without multi" << std::endl;

    return 0;
}