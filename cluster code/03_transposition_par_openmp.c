#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
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
        for (int j = i; j < n; j++) {
            matrix[i][j] = (float)rand() / RAND_MAX * 100;
            matrix[j][i] = matrix[i][j];
        }
    }
}

int checkSymOMP(float **matrix, int n, int n_threads) {
    const float epsilon = 1e-6;
    int isSymmetric = 1;

    omp_set_num_threads(n_threads);

#pragma omp parallel for shared(isSymmetric)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j += 16) {
            int end = (j + 16 < i) ? j + 16 : i;

            for (int jj = j; jj < end; jj++) {
                if (isSymmetric && fabsf(matrix[i][jj] - matrix[jj][i]) > epsilon) {
#pragma omp critical
                    isSymmetric = 0;
                }
            }
        }
    }

    return isSymmetric;
}

void matTransposeOMP(float **matrix, float **transposed, int n) {
    int blockSize = 32;

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

// Code for average performance evaluation
const int sizes[] = {16, 32, 64, 128, 256, 512, 1024, 2048, 4096};
int main(int argc, char *argv[]) {
    int n_threads = atoi(argv[1]);
    int symmetry_check = atoi(argv[2]);
    printf("TRANSPOSITION TIME EVALUATION --- THREADS: %d\n", n_threads);
    for (int s = 0; s < 9; s++) {
        int n = sizes[s];
        double total_t_time = 0.0;

        for (int z = 0; z < 3; z++) {
            float **matrix = (float **)malloc(n * sizeof(float *));
            float **transpose = (float **)malloc(n * sizeof(float *));
            for (int i = 0; i < n; i++) {
                matrix[i] = (float *)malloc(n * sizeof(float));
                transpose[i] = (float *)malloc(n * sizeof(float));
            }

            initializeMatrixAsym(matrix, n);

            struct timeval start_time, end_time;

            gettimeofday(&start_time, NULL);
            matTransposeOMP(matrix, transpose, n, n_threads);
            gettimeofday(&end_time, NULL);

            long seconds = end_time.tv_sec - start_time.tv_sec;
            long microseconds = end_time.tv_usec - start_time.tv_usec;
            double time_diff = (seconds * 1000.0) + (microseconds / 1000.0);

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
    if (symmetry_check == 1) {
        printf("\nSYMMETRY CHECK TIME EVALUATION\n");
        for (int s = 0; s < 9; s++) {
            int n = sizes[s];
            double total_s_time = 0.0;

            for (int z = 0; z < 3; z++) {
                float **matrix = (float **)malloc(n * sizeof(float *));
                float **transpose = (float **)malloc(n * sizeof(float *));
                for (int i = 0; i < n; i++) {
                    matrix[i] = (float *)malloc(n * sizeof(float));
                    transpose[i] = (float *)malloc(n * sizeof(float));
                }

                initializeMatrixAsym(matrix, n);

                struct timeval start_time, end_time;

                gettimeofday(&start_time, NULL);
                volatile int isSymmetric = checkSymOMP(matrix, n, n_threads);
                gettimeofday(&end_time, NULL);

                long seconds = end_time.tv_sec - start_time.tv_sec;
                long microseconds = end_time.tv_usec - start_time.tv_usec;
                double time_diff = (seconds * 1000.0) + (microseconds / 1000.0);

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
    }
    return 0;
}