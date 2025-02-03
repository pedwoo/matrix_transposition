#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define FLOAT_COMPARE_TOLERANCE 1e-6

void initializeMatrix(float **matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = (float)rand() / RAND_MAX * 10.0f;
        }
    }
}

// Symmetry check by blocks of 16 for consistent comparison
// Even when asymmetric (most of the time), it will still cover the entire matrix
int checkSymOMP(float **matrix, int n, int num_threads) {
    omp_set_num_threads(num_threads);

    int block_size = 16;
    int sym = 1;

#pragma omp parallel for default(none) shared(matrix, n, block_size) reduction(&& : sym)
    for (int i = 0; i < n; i += block_size) {
        for (int j = 0; j < n; j += block_size) {
            for (int ii = i; ii < i + block_size && ii < n; ii++) {
                for (int jj = j; jj < j + block_size && jj < n; jj++) {
                    if (fabs(matrix[ii][jj] - matrix[jj][ii]) > FLOAT_COMPARE_TOLERANCE) {
                        sym = 0;
                    }
                }
            }
        }
    }
    return sym;
}

// Transposition function by blocks of 16 for consistent comparison
int matTransposeOMP(float **matrix, float **transpose, int n, int num_threads) {
    omp_set_num_threads(num_threads);

    int block_size = 16;

#pragma omp parallel for default(none) shared(matrix, transpose, n, block_size)
    for (int i = 0; i < n; i += block_size) {
        for (int j = 0; j < n; j += block_size) {
            for (int ii = i; ii < i + block_size && ii < n; ii++) {
                for (int jj = j; jj < j + block_size && jj < n; jj++) {
                    transpose[jj][ii] = matrix[ii][jj];
                }
            }
        }
    }
    return 1;
}

int checkTranspose(float **matrix, float **transpose, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] != transpose[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <n> <n_threads> <iterations>\n", argv[0]);
        return 1;
    }

    int e = atoi(argv[1]);
    int num_threads = atoi(argv[2]);
    int iterations = atoi(argv[3]);
    double total_t, total_s = 0.0;
    if (iterations < 1 || iterations > 50) {
        printf("Number of iterations must be 1 <= iterations <= 50\n");
        return 1;
    }
    if (e < 4 || e > 12) {
        printf("Matrix size must be between 16 and 4096 (4 <= exponent <= 12)\n");
        return 1;
    }
    if (num_threads < 1) {
        printf("Number of threads must be greater than 0\n");
        return 1;
    }

    // Allocate memory for the matrix and its transpose
    int n = 1 << e;
    float **matrix = (float **)malloc(n * sizeof(float *));
    float **transpose = (float **)malloc(n * sizeof(float *));
    for (int i = 0; i < n; i++) {
        matrix[i] = (float *)malloc(n * sizeof(float));
        transpose[i] = (float *)malloc(n * sizeof(float));
    }

    // Transposition and symmetry check performance
    for (int iter = 0; iter < iterations; iter++) {
        struct timeval start, end;
        long seconds, microseconds;
        double elapsed;

        initializeMatrix(matrix, n);

        // Symmetry check performance evaluation
        gettimeofday(&start, NULL);
        int isSym = checkSymOMP(matrix, n, num_threads);
        gettimeofday(&end, NULL);

        seconds = end.tv_sec - start.tv_sec;
        microseconds = end.tv_usec - start.tv_usec;
        elapsed = seconds + microseconds * 1e-6;
        total_s += elapsed;

        // Transposition performance evaluation
        gettimeofday(&start, NULL);
        matTransposeOMP(matrix, transpose, n, num_threads);
        gettimeofday(&end, NULL);

        // Making sure that the transposition happened correctly
        int isTransposed = checkTranspose(matrix, transpose, n);
        printf("%s", isTransposed ? "" : "The matrix is not transposed correctly\n");

        seconds = end.tv_sec - start.tv_sec;
        microseconds = end.tv_usec - start.tv_usec;
        elapsed = seconds + microseconds * 1e-6;
        total_t += elapsed;
    }

    printf("Average symmetry chck time (size: %d, iter: %d): %f\n", n, iterations, (total_s / iterations) * 1000);
    printf("Average transposition time (size: %d, iter: %d): %f\n", n, iterations, (total_t / iterations) * 1000);

    // Free memory
    for (int i = 0; i < n; i++) {
        free(matrix[i]);
        free(transpose[i]);
    }
}