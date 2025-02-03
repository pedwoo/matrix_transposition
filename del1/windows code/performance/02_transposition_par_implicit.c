#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <xmmintrin.h>

void initializeMatrixAsym(float **matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = (float)rand() / RAND_MAX * 100;
        }
    }
}

void initializeMatrixSym(float **matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            matrix[i][j] = (float)rand() / RAND_MAX * 100;
            matrix[j][i] = matrix[i][j];
        }
    }
}

int checkSymImp(float **matrix, int n) {
    const float epsilon = 1e-6;
    int isSymmetric = 1;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (isSymmetric && fabsf(matrix[i][j] - matrix[j][i]) > epsilon) {
                isSymmetric = 0;
            }
        }
    }

    return isSymmetric;
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

// Code for average performance evaluation
const int sizes[] = {16, 32, 64, 128, 256, 512, 1024, 2048, 4096};
int main() {
    printf("TRANSPOSITION TIME EVALUATION\n");
    for (int s = 0; s < 9; s++) {
        int n = sizes[s];
        double total_t_time = 0.0;

        for (int z = 0; z < 100; z++) {
            float **matrix = (float **)malloc(n * sizeof(float *));
            float **transpose = (float **)malloc(n * sizeof(float *));
            for (int i = 0; i < n; i++) {
                matrix[i] = (float *)malloc(n * sizeof(float));
                transpose[i] = (float *)malloc(n * sizeof(float));
            }

            initializeMatrixAsym(matrix, n);
            LARGE_INTEGER frequency, start_time, end_time;
            double time_diff;

            QueryPerformanceFrequency(&frequency);

            QueryPerformanceCounter(&start_time);
            matTransposeImp(matrix, transpose, n);
            QueryPerformanceCounter(&end_time);
            time_diff = (double)(end_time.QuadPart - start_time.QuadPart) * 1000.0 / frequency.QuadPart;

            total_t_time += time_diff;

            for (int i = 0; i < n; i++) {
                free(matrix[i]);
                free(transpose[i]);
            }
            free(matrix);
            free(transpose);
        }
        printf("Matrix size: %d, time: %.6f ms\n", sizes[s], total_t_time / 100);
    }
    printf("\nSYMMETRY CHECK TIME EVALUATION\n");
    for (int s = 0; s < 9; s++) {
        int n = sizes[s];
        double total_s_time = 0.0;

        for (int z = 0; z < 100; z++) {
            float **matrix = (float **)malloc(n * sizeof(float *));
            float **transpose = (float **)malloc(n * sizeof(float *));
            for (int i = 0; i < n; i++) {
                matrix[i] = (float *)malloc(n * sizeof(float));
                transpose[i] = (float *)malloc(n * sizeof(float));
            }

            initializeMatrixAsym(matrix, n);

            LARGE_INTEGER frequency, start_time, end_time;
            double time_diff;

            QueryPerformanceFrequency(&frequency);

            QueryPerformanceCounter(&start_time);
            volatile int isSymmetric = checkSymImp(matrix, n);
            QueryPerformanceCounter(&end_time);
            time_diff = (double)(end_time.QuadPart - start_time.QuadPart) * 1000.0 / frequency.QuadPart;

            total_s_time += time_diff;

            for (int i = 0; i < n; i++) {
                free(matrix[i]);
                free(transpose[i]);
            }
            free(matrix);
            free(transpose);
        }
        printf("Matrix size: %d, time: %.6f ms\n", sizes[s], total_s_time / 100);
    }

    return 0;
}