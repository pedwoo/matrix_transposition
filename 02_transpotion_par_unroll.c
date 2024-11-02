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
        int j;
        for (j = 0; j <= n - 16; j += 16) {
            transpose[j][i] = matrix[i][j];
            transpose[j + 1][i] = matrix[i][j + 1];
            transpose[j + 2][i] = matrix[i][j + 2];
            transpose[j + 3][i] = matrix[i][j + 3];
            transpose[j + 4][i] = matrix[i][j + 4];
            transpose[j + 5][i] = matrix[i][j + 5];
            transpose[j + 6][i] = matrix[i][j + 6];
            transpose[j + 7][i] = matrix[i][j + 7];
            transpose[j + 8][i] = matrix[i][j + 8];
            transpose[j + 9][i] = matrix[i][j + 9];
            transpose[j + 10][i] = matrix[i][j + 10];
            transpose[j + 11][i] = matrix[i][j + 11];
            transpose[j + 12][i] = matrix[i][j + 12];
            transpose[j + 13][i] = matrix[i][j + 13];
            transpose[j + 14][i] = matrix[i][j + 14];
            transpose[j + 15][i] = matrix[i][j + 15];
        }
    }
}

int main() {
    int n;
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
    time_diff = (double)(end_time.QuadPart - start_time.QuadPart) * 1000.0 / frequency.QuadPart;
    printf("Time to check symmetry: %.4f ms ==> ", time_diff);
    printf("This matrix is %s\n", isSymmetric ? "symmetric" : "asymmetric");

    // Performance evaluation of matrix transpose
    QueryPerformanceCounter(&start_time);  // Start timing
    matTranspose(matrix, transpose, n);
    QueryPerformanceCounter(&end_time);  // End timing
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
//     int n = 10;
//     printf("Enter the size of the matrix: ");
//     scanf("%d", &n);
//     double total_time = 0.0;

//     for (int z = 0; z < 10; z++) {
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
//         int isSymmetric = checkSym(matrix, n);
//         QueryPerformanceCounter(&end_time);
//         time_diff = (double)(end_time.QuadPart - start_time.QuadPart) *
//         1000.0 /
//                     frequency.QuadPart;
//         printf("Time to check symmetry: %.4f ms ==> ", time_diff);
//         printf("This matrix is %s\n", isSymmetric ? "symmetric" :
//         "asymmetric");

//         // Performance evaluation of matrix transpose
//         QueryPerformanceCounter(&start_time);  // Start timing
//         matTranspose(matrix, transpose, n);
//         QueryPerformanceCounter(&end_time);  // End timing
//         time_diff = (double)(end_time.QuadPart - start_time.QuadPart) *
//         1000.0 /
//                     frequency.QuadPart;
//         printf("Time to transpose: %.4f ms\n", time_diff);
//         total_time += time_diff;

//         for (int i = 0; i < n; i++) {
//             free(matrix[i]);
//             free(transpose[i]);
//         }
//         free(matrix);
//         free(transpose);
//     }
//     printf("Average time: %.4f ms\n", total_time / 10);

//     return 0;
// }