@echo off

REM Check GCC version (assuming GCC is installed and in PATH)
gcc --version
IF ERRORLEVEL 1 (
    ECHO GCC is not installed or not in PATH.
    EXIT /B 1
)

REM Compile and run the sequential approach
ECHO Compiling and running the Sequential Approach
ECHO ================================================
gcc 01_transposition_sequential.c -o .\exec\01_transposition_sequential.exe
IF ERRORLEVEL 1 (
    ECHO Compilation failed for Sequential Approach.
    EXIT /B 1
)
.\exec\01_transposition_sequential
ECHO.

REM Compile and run the implicit parallelism approach
ECHO Compiling and running the Implicit Parallelism Approach
ECHO ========================================================
gcc -O2 -march=native 02_transposition_par_implicit.c -o .\exec\02_transposition_par_implicit.exe
IF ERRORLEVEL 1 (
    ECHO Compilation failed for Implicit Approach.
    EXIT /B 1
)
.\exec\02_transposition_par_implicit
ECHO.

REM Compile and run the OpenMP approach
ECHO Compiling and running the OpenMP Approach
ECHO ==========================================
gcc -O3 -fopenmp 03_transposition_par_openmp.c -o .\exec\03_transposition_par_openmp.exe
IF ERRORLEVEL 1 (
    ECHO Compilation failed for OpenMP Approach.
    EXIT /B 1
)
ECHO Running OpenMP Approach with 1 thread
.\exec\03_transposition_par_openmp 1 1
ECHO.
ECHO Running OpenMP Approach with 2 threads
.\exec\03_transposition_par_openmp 2 0
ECHO.
ECHO Running OpenMP Approach with 4 threads
.\exec\03_transposition_par_openmp 4 0
ECHO.
ECHO Running OpenMP Approach with 8 threads
.\exec\03_transposition_par_openmp 8 0
ECHO.
ECHO Running OpenMP Approach with 16 threads
.\exec\03_transposition_par_openmp 16 0
ECHO.
ECHO Running OpenMP Approach with 32 threads
.\exec\03_transposition_par_openmp 32 0
ECHO.

REM Job completed
ECHO All tasks completed successfully.
PAUSE
