#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <xmmintrin.h>

void initializeMatrix(float **matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = (float)rand() / RAND_MAX * 100;
            matrix[j][i] = matrix[i][j];
        }
    }
}

int checkSymOMP(float **matrix, int n, int n_threads) {
    const float epsilon = 1e-6;
    int isSymmetric = 1;

    omp_set_num_threads(n_threads);
#pragma omp parallel for schedule(static) collapse(1) shared(isSymmetric)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (isSymmetric && fabsf(matrix[i][j] - matrix[j][i]) > epsilon) {
#pragma omp atomic write
                isSymmetric = 0;
            }
        }
    }

    return isSymmetric;
}

void matTransposeOMP(float **matrix, float **transposed, int n, int n_threads) {
    int blockSize = 32;

    omp_set_num_threads(n_threads);
#pragma omp parallel for collapse(2)
    for (int i = 0; i < n; i += blockSize) {
        for (int j = 0; j < n; j += blockSize) {
            int maxI = i + blockSize > n ? n : i + blockSize;
            int maxJ = j + blockSize > n ? n : j + blockSize;

            for (int ii = i; ii < maxI; ++ii) {
                for (int jj = j; jj < maxJ; ++jj) {
                    if (jj + 1 < maxJ) {
                        _mm_prefetch((float *)&matrix[ii][jj + 1], _MM_HINT_T0);
                    }
                    if (ii + 1 < maxI) {
                        _mm_prefetch((float *)&matrix[ii + 1][j], _MM_HINT_T0);
                    }
                    transposed[jj][ii] = matrix[ii][jj];
                }
            }
        }
    }
}

int main(int argc, char *argv[]) {
    int n_threads = atoi(argv[1]);
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
    int isSymmetric = checkSymOMP(matrix, n, n_threads);
    QueryPerformanceCounter(&end_time);
    time_diff = (double)(end_time.QuadPart - start_time.QuadPart) * 1000.0 / frequency.QuadPart;
    printf("Time to check symmetry: %.4f ms ==> ", time_diff);
    printf("This matrix is %s\n", isSymmetric ? "symmetric" : "asymmetric");

    // Performance evaluation of matrix transpose
    QueryPerformanceCounter(&start_time);
    matTransposeOMP(matrix, transpose, n, n_threads);
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
//         int isSymmetric = checkSymOMP(matrix, n);
//         QueryPerformanceCounter(&end_time);
//         time_diff = (double)(end_time.QuadPart - start_time.QuadPart) * 1000.0 / frequency.QuadPart;
//         // printf("Time to check symmetry: %.4f ms ==> ", time_diff);
//         // printf("This matrix is %s\n", isSymmetric ? "symmetric" : "asymmetric");
//         total_s_time += time_diff;

//         // Performance evaluation of matrix transpose
//         QueryPerformanceCounter(&start_time);
//         matTransposeOMP(matrix, transpose, n);
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