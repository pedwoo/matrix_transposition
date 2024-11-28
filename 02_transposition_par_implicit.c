#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <xmmintrin.h>

void initializeMatrix(float **matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = (float)rand() / RAND_MAX * 100;
        }
    }
}

int checkSymImp(float **matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (matrix[i][j] != matrix[j][i]) {
                return 0;
            }
        }
    }
    // int blockSize = 32;

    // for (int i = 0; i < n; i += blockSize) {
    //     for (int j = i; j < n; j += blockSize) {
    //         int maxI = (i + blockSize > n) ? n : i + blockSize;
    //         int maxJ = (j + blockSize > n) ? n : j + blockSize;

    //         for (int ii = i; ii < maxI; ++ii) {
    //             for (int jj = (ii == i ? j : i); jj < maxJ; jj += 4) {
    //                 if (jj + 4 <= maxJ) {
    //                     __m128 rowElems = _mm_loadu_ps(&matrix[ii][jj]);
    //                     __m128 colElems = _mm_set_ps(matrix[jj + 3][ii], matrix[jj + 2][ii], matrix[jj + 1][ii], matrix[jj][ii]);
    //                     __m128 cmp = _mm_cmpeq_ps(rowElems, colElems);

    //                     if (_mm_movemask_ps(cmp) != 0xF) {
    //                         return 0;
    //                     }
    //                 }
    //             }
    //         }
    //     }
    // }

    return 1;
}

void matTransposeImp(float **matrix, float **transposed, int n) {
    int blockSize = 32;

    for (int i = 0; i < n; i += blockSize) {
        for (int j = 0; j < n; j += blockSize) {
            int maxI = i + blockSize > n ? n : i + blockSize;
            int maxJ = j + blockSize > n ? n : j + blockSize;

            for (int ii = i; ii < maxI; ii += 4) {
                for (int jj = j; jj < maxJ; ++jj) {
                    _mm_prefetch((float *)&matrix[ii][jj + 1], _MM_HINT_T0);
                    __m128 vec = _mm_loadu_ps(&matrix[ii][jj]);
                    _mm_storeu_ps(&transposed[jj][ii], vec);
                }
            }
        }
    }
}

int main() {
    int n;
    printf("Enter the size of the matrix: ");
    scanf("%d", &n);

    float **matrix = (float **)malloc(n * sizeof(float *));
    float **transpose = (float **)malloc(n * sizeof(float *));

    if (!matrix || !transpose) {
        fprintf(stderr, "Memory allocation failed!\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        matrix[i] = (float *)malloc(n * sizeof(float));
        transpose[i] = (float *)malloc(n * sizeof(float));

        if (!matrix[i] || !transpose[i]) {
            fprintf(stderr, "Memory allocation for rows failed!\n");
            return 1;
        }
    }

    srand(time(NULL));
    initializeMatrix(matrix, n);

    // Using QueryPerformanceCounter for it provides higher precision
    LARGE_INTEGER frequency, start_time, end_time;
    double time_diff;
    QueryPerformanceFrequency(&frequency);

    // Performance evaluation of symmetric check
    QueryPerformanceCounter(&start_time);
    int isSymmetric = checkSymImp(matrix, n);
    QueryPerformanceCounter(&end_time);
    time_diff = (double)(end_time.QuadPart - start_time.QuadPart) * 1000.0 / frequency.QuadPart;
    printf("Time to check symmetry: %.4f ms ==> ", time_diff);
    printf("This matrix is %s\n", isSymmetric ? "symmetric" : "asymmetric");

    // Performance evaluation of matrix transpose
    QueryPerformanceCounter(&start_time);
    matTransposeImp(matrix, transpose, n);
    QueryPerformanceCounter(&end_time);
    time_diff = (double)(end_time.QuadPart - start_time.QuadPart) * 1000.0 / frequency.QuadPart;
    printf("Time to transpose: %.4f ms\n", time_diff);

    for (int i = 0; i < n; i++) {
        free(matrix[i]);
        free(transpose[i]);
    }
    free(matrix);
    free(transpose);

    return 0;
}

// Code for average performance evaluation
// int main() {
//     int n;
//     printf("Enter the size of the matrix: ");
//     scanf("%d", &n);
//     double total_s_time = 0.0;
//     double total_t_time = 0.0;

//     for (int z = 0; z < 100; z++) {
//         float **matrix = (float **)malloc(n * sizeof(float *));
//         float **transpose = (float **)malloc(n * sizeof(float *));
//         for (int i = 0; i < n; i++) {
//             matrix[i] = (float *)malloc(n * sizeof(float));
//             transpose[i] = (float *)malloc(n * sizeof(float));
//         }

//         srand(time(NULL));
//         initializeMatrix(matrix, n);

//         // Using QueryPerformanceCounter for it provides higher precision
//         LARGE_INTEGER frequency, start_time, end_time;
//         double time_diff;
//         QueryPerformanceFrequency(&frequency);

//         // Performance evaluation of symmetric check
//         QueryPerformanceCounter(&start_time);
//         int isSymmetric = checkSymImp(matrix, n);
//         QueryPerformanceCounter(&end_time);
//         time_diff = (double)(end_time.QuadPart - start_time.QuadPart) * 1000.0 / frequency.QuadPart;
//         // printf("Time to check symmetry: %.4f ms ==> ", time_diff);
//         // printf("This matrix is %s\n", isSymmetric ? "symmetric" : "asymmetric");
//         total_s_time += time_diff;

//         // Performance evaluation of matrix transpose
//         QueryPerformanceCounter(&start_time);
//         matTransposeImp(matrix, transpose, n);
//         QueryPerformanceCounter(&end_time);
//         time_diff = (double)(end_time.QuadPart - start_time.QuadPart) * 1000.0 / frequency.QuadPart;
//         // printf("Time to transpose: %.4f ms\n", time_diff);
//         total_t_time += time_diff;
//         for (int i = 0; i < n; i++) {
//             free(matrix[i]);
//             free(transpose[i]);
//         }
//         free(matrix);
//         free(transpose);
//     }
//     printf("Average s time: %.6f ms\n", total_s_time / 100);
//     printf("Average t time: %.6f ms\n", total_t_time / 100);

//     return 0;
// }