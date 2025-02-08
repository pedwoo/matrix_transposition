#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define EPSILON 1e-6

void initializeMatrix(float *matrix, int n) {
    for (int i = 0; i < n * n; i++) {
        matrix[i] = ((float)rand() / RAND_MAX) * 10.0f;
    }
}

int checkTranspose(float *matrix, float *transposed, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (fabs(matrix[i * n + j] - transposed[j * n + i]) > EPSILON) {
                return 0;
            }
        }
    }
    return 1;
}

// Symmetry check using MPI Broadcast to distribute the entire matrix to all processors
int checkSymMPI(float *matrix, int n, int rank, int num_processor) {
    // Allocate the entire matrix on all processes except rank 0
    if (rank != 0) {
        matrix = (float *)malloc(n * n * sizeof(float));
    }
    // Every processor will handle n/num_processors rows
    int local_rows_number = n / num_processor;
    int local_start_row = rank * (local_rows_number + 1);
    int local_end_row = local_start_row + local_rows_number + 1;

    int local_sym = 1;
    int global_sym = 1;

    // Broadcast the entire matrix to all processes
    MPI_Bcast(matrix, n * n, MPI_FLOAT, 0, MPI_COMM_WORLD);
    // Check the symmetry of the local block of rows
    for (int i = local_start_row; i < local_end_row; i++) {
        for (int j = i + 1; j < n; j++) {
            if (fabs(matrix[i * n + j] - matrix[j * n + i]) > EPSILON) {
                local_sym = 0;
            }
        }
    }
    // Reduce the local symmetry checks to a global symmetry check
    MPI_Allreduce(&local_sym, &global_sym, 1, MPI_INT, MPI_LAND, MPI_COMM_WORLD);

    if (rank != 0) {
        free(matrix);  // Free the allocated matrix on all processes except rank 0
    }
    return global_sym;
}

// Transposition using MPI Scatter/Gather in a row-to-column fashion
void matTransposeMPI(float *matrix, float *transposed, int n, int rank, int num_processors) {
    // Every processor will handle n/num_processors rows
    int local_rows_number = n / num_processors;
    float *local_block = (float *)malloc(local_rows_number * n * sizeof(float));

    // Create a buffer for sending columns and receiving rows
    float *send_row_buffer = (float *)malloc(local_rows_number * sizeof(float));
    float *recv_row_buffer = NULL;
    if (rank == 0) {
        recv_row_buffer = (float *)malloc(n * sizeof(float));
    }

    // Scatter the matrix in blocks to all processes
    MPI_Scatter(matrix, local_rows_number * n, MPI_FLOAT, local_block, local_rows_number * n, MPI_FLOAT, 0, MPI_COMM_WORLD);
    for (int col = 0; col < n; col++) {
        for (int row = 0; row < local_rows_number; row++) {
            send_row_buffer[row] = local_block[row * n + col];
        }

        // Gather the transposed rows from all processes
        MPI_Gather(send_row_buffer, local_rows_number, MPI_FLOAT, recv_row_buffer, local_rows_number, MPI_FLOAT, 0, MPI_COMM_WORLD);
        // Combine the transposed rows back into the matrix (only on the main thread)
        if (rank == 0) {
            for (int row_main = 0; row_main < n; row_main++) {
                transposed[col * n + row_main] = recv_row_buffer[row_main];
            }
        }
    }

    free(send_row_buffer);
    free(local_block);

    if (rank == 0) {
        free(recv_row_buffer);
    }
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, num_processors;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_processors);

    // Input validation
    if (rank == 0 && argc != 3) {
        printf("Usage: mpirun -np <n_processors> %s <n> <iterations>\n", argv[0]);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int e = atoi(argv[1]);
    int n = 1 << e;
    int iterations = atoi(argv[2]);
    if (e < 4 || e > 12) {
        if (rank == 0) printf("Matrix n must be between 16 and 4096 (4 <= exponent <= 12)\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    if (iterations < 1 || iterations > 50) {
        if (rank == 0) printf("Number of iterations must be 1 <= iterations <= 50\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    if (n < num_processors) {
        if (rank == 0) printf("Matrix n must be greater than or equal to the number of processors\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    // Instantiation of the matrix and its transpose
    float *matrix = NULL;
    float *transposed = NULL;

    if (rank == 0) {
        matrix = (float *)malloc(n * n * sizeof(float));
        transposed = (float *)malloc(n * n * sizeof(float));
    }

    double start_time, end_time;
    double total_s, total_t = 0.0;
    MPI_Barrier(MPI_COMM_WORLD);

    for (int iter = 0; iter < iterations; iter++) {
        if (rank == 0) {
            initializeMatrix(matrix, n);
        }

        // Symmetry check performance evaluation
        MPI_Barrier(MPI_COMM_WORLD);
        start_time = MPI_Wtime();
        checkSymMPI(matrix, n, rank, num_processors);
        MPI_Barrier(MPI_COMM_WORLD);
        end_time = MPI_Wtime();
        if (rank == 0) total_s += (end_time - start_time);

        // Transposition performance evaluation
        MPI_Barrier(MPI_COMM_WORLD);
        start_time = MPI_Wtime();
        matTransposeMPI(matrix, transposed, n, rank, num_processors);
        MPI_Barrier(MPI_COMM_WORLD);
        end_time = MPI_Wtime();

        if (rank == 0) {
            total_t += (end_time - start_time);
            int success = checkTranspose(matrix, transposed, n);
            printf("%s", success ? "" : "Matrix transposition failed\n");
        }
    }

    if (rank == 0) {
        printf("Average symmetry chck time (size: %d, np: %d, iterations: %d): %f ms\n", n, num_processors, iterations, (total_s / iterations) * 1000);
        printf("Average transposition time (size: %d, np: %d, iterations: %d): %f ms\n", n, num_processors, iterations, (total_t / iterations) * 1000);
        free(matrix);
        free(transposed);
    }

    MPI_Finalize();
    return 0;
}
