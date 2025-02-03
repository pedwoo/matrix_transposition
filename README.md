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
Alternatively (or on a Windows system), the different files can be compiled and run separately, as follows:

-   **Sequential approach**\
    This is the simplest approach, used as baseline performance for all the ones coming after it, it uses no optimization at all.
    -   _Compilation_: `gcc 01_transposition_sequential.c -o ./exec/01_transposition_sequential`
    -   _Execution_: `.\exec\01_transposition_sequential`
-   **Implicit parallelism approach**\
    This approach implements a simple level of optimization, mainly given by the compiler flags used, and a transposition by blocks instead of single cells.
    -   _Compilation_: `gcc -O2 -march=native 02_transposition_par_implicit.c -o ./exec/02_transposition_par_implicit`
    -   _Execution_: `.\exec\02_transposition_par_implicit`
-   **OpenMP approach**\
    This approach implements the most optimized version of the code, making use of compiler flags, transposition by blocks, and loop collapsing using OpenMP directives. It can be run with a different number of threads.
    -   _Compilation_: `gcc -O3 -fopenmp 03_transposition_par_openmp.c -o ./exec/03_transposition_par_openmp`
    -   _Execution_: `.\exec\03_transposition_par_openmp <n_threads>`

All of the files above will run both the symmetry check and transposition for a given matrix size (specified at runtime), providing the performance for both.
