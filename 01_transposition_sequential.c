#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

void initializeMatrix(float **matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = (float)rand() / RAND_MAX * 100;
        }
    }
}

int checkSym(float **matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] != matrix[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

void matTranspose(float **matrix, float **transpose, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            transpose[j][i] = matrix[i][j];
        }
    }
}

int main() {
    int n = 10;
    printf("Enter the size of the matrix: ");
    scanf("%d", &n);

    float **matrix = (float **)malloc(n * sizeof(float *));
    float **transpose = (float **)malloc(n * sizeof(float *));
    for (int i = 0; i < n; i++) {
        matrix[i] = (float *)malloc(n * sizeof(float));
        transpose[i] = (float *)malloc(n * sizeof(float));
    }

    srand(time(NULL));
    initializeMatrix(matrix, n);

    // Using QueryPerformanceCounter for it provides higher precision
    LARGE_INTEGER frequency, start_time, end_time;
    double time_diff;
    QueryPerformanceFrequency(&frequency);

    // Performance evaluation of symmetric check
    QueryPerformanceCounter(&start_time);
    int isSymmetric = checkSym(matrix, n);
    QueryPerformanceCounter(&end_time);
    time_diff = (double)(end_time.QuadPart - start_time.QuadPart) * 1000.0 /
                frequency.QuadPart;
    printf("Time to check symmetry: %.4f ms ==> ", time_diff);
    printf("This matrix is %s\n", isSymmetric ? "symmetric" : "asymmetric");

    // Performance evaluation of matrix transpose
    QueryPerformanceCounter(&start_time);  // Start timing
    matTranspose(matrix, transpose, n);
    QueryPerformanceCounter(&end_time);  // End timing
    time_diff = (double)(end_time.QuadPart - start_time.QuadPart) * 1000.0 /
                frequency.QuadPart;
    printf("Time to transpose: %.4f ms\n", time_diff);

    return 0;
}