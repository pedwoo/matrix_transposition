#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <xmmintrin.h>

void initializeMatrix(float **matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            // matrix[i][j] = (float)rand() / RAND_MAX * 100;
            matrix[i][j] = 1.0;
        }
    }
}

int checkSymImp(float **matrix, int n) {
    float tolerance = 1e-6;  // Tolerance for floating-point comparison

    for (int i = 0; i < n; i++) {
        // Prefetch the row for the current index to optimize memory access
        _mm_prefetch((const char *)&matrix[i + 1][0], _MM_HINT_T0);

        for (int j = i + 1; j < n; j++) {
            _mm_prefetch((const char *)&matrix[j][i], _MM_HINT_T0);

            // Use SIMD to compare elements, ensuring we do not access out of bounds
            if (fabs(matrix[i][j] - matrix[j][i]) > tolerance) {
                return 0;
            }
        }
    }
    return 1;
}

void matTransposeImp(float **matrix, float **transposed, int n) {
    int blockSize = 32;

    for (int i = 0; i < n; i += blockSize) {
        for (int j = 0; j < n; j += blockSize) {
            int maxI = i + blockSize > n ? n : i + blockSize;
            int maxJ = j + blockSize > n ? n : j + blockSize;

            for (int ii = i; ii < maxI; ii += 4) {  // We can processo 4 floats at a time
                for (int jj = j; jj < maxJ; ++jj) {
                    _mm_prefetch((const char *)&matrix[ii][jj + 1], _MM_HINT_T0);  // Prefetch next element
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