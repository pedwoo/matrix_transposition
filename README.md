# Parallel matrix transposition using OpenMP and MPI

## Introduction and project objective

This repository contains the project for the "Introduction to Parallel Computing" @Unitrento, year 2024/25, author: Daniele Pedrolli.\
The project is composed of two parts, the README.md that was handed in with the first deliverable can be found in `del1/README_old.md`.\
The first part of the project aims at improving the performance of matrix transposition by using more computational power through OpenMP, while the second part is about optimizing it using more memory resources through MPI. For the first part of the project there is also a version of the code that was developed on a Windows-based system (`del1/windows code`), alongside the code that was run on the Unitn cluster.

## Repository structure

<!-- ```
.
├── del1
│   ├── exec
│   │   ├── * -> Compiled linux code
│   ├── windows code -> code compatible with windows machine
│   │   ├── exec
│   │   │   ├── * -> Compiled windows source files
│   │   ├── performance
│   │   │   ├── * -> Modified windows code for performance evaluation
│   │   ├── utils
│   │   │   ├── graphs -> Graphs obtained from performance evaluation
│   │   │   ├── mingw-components.txt -> required minGW libraries
│   │   ├── 01_transposition_sequential.c
│   │   ├── 02_transposition_par_implicit.c
│   │   ├── 03_transposition_par_openmp.c
│   │   ├── util.c -> utility function for bandwidth computation
│   ├── openMP.pbs
│   ├── README_old.md -> README submitted with first deliverable
├── del2
│   ├── exec
│   │   ├── * -> Compiled linux code
│   ├── MPI.pbs
``` -->

```
.
├── del1
│   ├── exec
│   │   ├── * -> Compiled linux source code
│   ├── windows code
│   │   ├── exec
│   │   │   ├── * -> Compiled windows source code
│   │   ├── performance
│   │   │   ├── * -> Modified windows code for performance evaluation
│   │   ├── utils
│   │   │   ├── graphs
│   │   │   ├── mingw-components.txt
│   │   ├── 01_transposition_sequential.c
│   │   ├── 02_transposition_par_implicit.c
│   │   ├── 03_transposition_par_openmp.c
│   │   ├── util.c
│   ├── openMP.pbs
│   ├── README_old.md
├── del2
│   ├── exec
│   │   ├── * -> Compiled linux code
│   ├── MPI.pbs
```

## Reproducibility instructions

All the files that aren't in the `windows code` folder are intended to be compiled and run on a Linux based system. If that's the case, it is possible to run everything at once using the `openMP.pbs` and `MPI.pbs` files found respectively in `del1/openMP.pbs` and `del2/MPI.pbs`.\
Alternatively (or on a Windows system, by compiling a `.exe` file instead of `.out` and in the appropriate directory), the different files can be compiled and run separately, as follows:

-   **Sequential approach**\
    This is the simplest approach, used as baseline performance for all the ones coming after it, it uses no optimization at all.\
    File: [01_transposition_sequential.c](./del1/01_transposition_sequential.c)

    -   _Compilation_: `gcc 01_transposition_sequential.c -o ./exec/01_transposition_sequential.out`
    -   _Execution_: `./exec/01_transposition_sequential` or `.\exec\01_transposition_sequential`

-   **Implicit parallelism approach**\
    This approach implements a simple level of optimization, mainly given by the compiler flags used, and a transposition by blocks instead of single cells.\
    File: [02_transposition_par_implicit.c](./del1/02_transposition_par_implicit.c)

    -   _Compilation_: `gcc -O2 -march=native 02_transposition_par_implicit.c -o ./exec/02_transposition_par_implicit.out`
    -   _Execution_: `./exec/02_transposition_par_implicit` or `.\exec\02_transposition_par_implicit`

-   **OpenMP approach**\
    This approach implements the most optimized version of the code, making use of compiler flags, transposition by blocks, and loop collapsing using OpenMP directives. It can be run with a different number of threads.\
    File: [03_transposition_per_openmp.c](./del1/03_transposition_par_openmp.c)

    -   _Compilation_: `gcc -O3 -fopenmp 03_transposition_par_openmp.c -o ./exec/03_transposition_par_openmp.out`
    -   _Execution_: `./exec/03_transposition_par_openmp <n_threads>` or `.\exec\03_transposition_par_openmp <n_threads>`

All of the files above will run both the symmetry check and transposition for a given matrix size (specified at runtime), providing the performance for both.\
The following MPI approach is only intended to be compiled and executed on a Linux based system (like the Unitn cluster).

-   **MPI approach**\
    This approach used a distributed memory model, rather than a distributed computation model. It also includes a rivisited version of the sequential transposition and symmetry check.\
    File: [01b_transposition_sequential.c]()

    -   _Compilation_: `gcc 01b_transposition_sequential.c -o ./exec/01b_transposition_sequential.out`
    -   _Execution_: `./exec/01b_transposition_sequential <iterations> <n> (matrix size is 2^n)`

    File: [04_transposition_mpi_one.c]()\
    This solution's approach is to use an all-to-all distribution so that every processor has the entire matrix at it's disposal, but then only transposes/symmetry checks a part of it (a single line to a single column).

    -   _Compilation_: `mpicc 04_transposition_mpi_one.c -o ./exec/04_transposition_mpi_one.out`
    -   _Execution_: `mpirun -np <n_processors> ./exec/04_transposition_mpi_one <n> (matrix size is 2^n)`

    File: [05_transposition_mpi_two.c]()\
    This solution's approach optimizes the previous one, so rather than sending the entire matrix to all processors, to then only work on a single line like before, only the single line to operate on is distributed to the designed processor.

    -   _Compilation_: `mpicc 05_transposition_mpi_two.c -o ./exec/05_transposition_mpi_two.out`
    -   _Execution_: `mpirun -np <n_processors> ./exec/05_transposition_mpi_two <n> (matrix size is 2^n)`

    File: [06_transposition_mpi_symmetry.c]()

    -   _Compilation_: `gg`
    -   _Execution_: `gg`
