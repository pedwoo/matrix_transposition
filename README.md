# Matrix transposition in parallel

## 0. Table of contents

1. [Introduction](#introduction)  
   &nbsp;&nbsp;&nbsp;&nbsp;1.1. [What's the importance of matrix transposition](#11-whats-the-importance-of-matrix-transposition)  
   &nbsp;&nbsp;&nbsp;&nbsp;1.2. [Common applications](#12-common-applications)
2. [State of the art](#2-state-of-the-art)  
   &nbsp;&nbsp;&nbsp;&nbsp;2.1. [Current research and development](#21-current-research-and-development)  
   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;2.1.1. [Parallelization with OpenMP](#211-parallelization-with-openmp)  
   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;2.1.2. [Parallelization with MPI](#212-parallelization-with-mpi)  
   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;2.1.3. [Parallelization with CUDA](#213-gpu-based-parallelization-with-cuda)  
   &nbsp;&nbsp;&nbsp;&nbsp;2.2. [Limitations in existing solutions](#22-limitations-in-existing-solutions)  
   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;2.2.1. [Limitations of OpenMP](#221-limitations-of-openmp)  
   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;2.2.2. [Limitations of MPI](#222-limitations-of-mpi)  
   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;2.2.3. [Limitations of CUDA](#223-limitations-of-cuda)
3. [Contribution and methodology](#3-contribution-and-methodology)  
   &nbsp;&nbsp;&nbsp;&nbsp;3.1. [Sequential approach](#31-sequential-approach)  
   &nbsp;&nbsp;&nbsp;&nbsp;3.2. [Implicit parallelism](#32-implicit-parallelism)  
   &nbsp;&nbsp;&nbsp;&nbsp;3.3. [OpenMP](#33-parallelism-through-openmp)
4. [Experiments and system description](#4-experiments-and-system-description)  
   &nbsp;&nbsp;&nbsp;&nbsp;4.1. [System configuration](#41-system-configuration)  
   &nbsp;&nbsp;&nbsp;&nbsp;4.2. [Peak performance evaluation](#42-peak-performance-evaluation)
5. [Results](#5-results)  
   &nbsp;&nbsp;&nbsp;&nbsp;5.1. [Experimental times per approach and matrix size](#51-experimental-times-per-approach-and-matrix-size)  
   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;5.1.1. [Transposition performance](#511-transposition-performance)  
   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;5.1.2. [Symmetry check performance](#512-symmetry-check-performance)  
   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;5.1.3. [OpenMP transposition performance](#513-openmp-transposition-performance)  
   &nbsp;&nbsp;&nbsp;&nbsp;5.2. [Experimental times on the Unitn cluster](#52-experimental-times-on-the-unitn-cluster)  
   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;5.2.1. []()  
   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;5.2.2. []()  
   &nbsp;&nbsp;&nbsp;&nbsp;5.3. [Experimental memory bandwidth utilization](#53-experimental-memory-bandwidth-utilization)  
   &nbsp;&nbsp;&nbsp;&nbsp;5.4. [Speedup compared to baseline approach](#54-speedup-compared-to-baseline-approach)
6. [Conclusions](#6conclusion)
7. [Installation and reproducibility of the results](#7-installation-and-reproducibility-of-the-results)
8. [References](#8-references)

## 1. Introduction

The goal of this project is to explore and evaluate various parallelization techniques in order to identify the most efficient solution for the given problem. Through this analysis, we aim to either pinpoint the optimal approach or, at the very least, provide comprehensive results and insights to support informed decision-making within the context of the problem.  
First, we shall take a step back and understand why we are undertaking this task in the first place, and what its importance is.

### 1.1 What's the importance of matrix transposition

Matrices are fundamental in algebra, as they can encode a great amount of information that has diverse applications. Particularly, matrices that represent linear transformation are of great interest due to their wide-ranging utility since, by multiplying a specific matrix by any input vector, predictable transformations can be achieved, allowing consistent manipulations of vectors across various contexts.

### 1.2 Common applications

Matrices are widely used throughout various scientific fields, as they can encode a very large amount of information in a compact manner. Their importance lies in the ability to represent and apply both linear and non-linear transformations, which is one of their primary applications. The fields in which matrices and transposition play a crucial role include:

-   **Deep learning**: Particularly withing neural networks and convolutional layers, where data often requires reshaping or alignment for specific layer operations. Large input batches may need to be transposed to fit model expectations.
-   **Graphics and image processing**: Operations like rotations, reflections, and texture mapping are used extensively, where matrix transposition facilitates precise image representation and manipulations.
-   **Scientific and numerical simulations**: In fields like physics and chemistry, matrices represent large datasets and enable complex transformations, crucial for realistic simulations.
-   **Signal processing**: Matrices represent signal data, which may be transposed to align with computational steps such as those in Fourier Transform applications.

## 2. State of the art

As parallel computation became more popular and necessary in recent years, various strategies have been proposed to optimize operations such as matrix transposition, especially when working with large-scale computations.  
This section will present these strategies along with their limitations, to finally define the gap our project aims to address.

### 2.1 Current research and development

#### 2.1.1 Parallelization with OpenMP

OpenMP is an extensively used framework for parallelizing programs on multi-core CPUs. It uses compuler directives, which enable developers to easily parallelize different sections of a program, like loops and functions, without significant changes to the code. OpenMP also offers solid scalability features, which make it suitable for matrix operations on a multi-core system.

#### 2.1.2 Parallelization with MPI

MPI (Message Passing Interface) is an established method for parallelizing computation on distributed memory systems. It allows different processes to communicate, making it suitable for large scale, distributed computing systems. When working on matrix transposition, MPI can split the matrix in different blocks and distribute them over multiple different nodes, allowing for parallel execution and at the same time minimizing the impact of memory limitations on individual processors.

#### 2.1.3 GPU-based parallelization with CUDA

CUDA is a platform developed by NVIDIA that allows for GPU-accelerated matrix computation. Since GPUs offer a high degree of parallelism, CUDA can offer significant performance improvements in tasks such as matrix transposition. It is also worth mentioning that GPU memory normally has a much higher bandwidth compared to CPU memory, therefore making CUDA particularly effective for memory-bound operations.

#### 2.1.4 Hybrid approaches

A combination of the above mentioned methods can also be a solution, which is viable due to the increasing availability of heterogeneous computing systems. Using this methodology, one can for example use OpenMP for CPU-based parallelism and CUDA for gpu-based acceleration, allowing developers to exploit both types of hardware.

### 2.2 Limitations in existing solutions

In spite of the advances that are being made in parallel computing for matrix operations, some limitations remain across existing solutions:

#### 2.2.1 Limitations of OpenMP

OpenMP has limited scalability with very large matrices, due to a high thread synchronization overhead and issues with memory contention. The performance gain is very often limited for larger matrices since the problem becomes more memory-bound than intensive in terms of computations.

#### 2.2.2 Limitations of MPI

MPI is highly efficient for distributed systems, but also faces a high communication overhead, which hinders its performance for smaller-sized matrices. This is due to a high cost of internal process commmunication that ends up outweighing the benefits of such an approach.

#### 2.2.3 Limitations of CUDA

CUDA offers excellent performance on devices that are equipped with a GPU, but is often limited by its strict hardware dependency, since it requires specific optimizations based on the GPU.

#### 2.2.4 Limitations of hybrid approaches

Hybrid approaches are able to deliver superior performance but require complex tuning and resource management, which ends up having the same per-hardware specificity issue that CUDA presents.

## 3. Contribution and methodology

### 3.1 Sequential approach

Approaching matrix transposition from a sequential point of view is an almost trivial problem. The one thing that can be noted, which persists throughout all the solutions proposed, is that due to the size of the matrix being decided at runtime, we cannot statically declare the matrices but must instead allocate memory dynamically. This is achieved using `malloc`, which allows memory to be allocated on the heap during program execution.

The compilation command, which is in line with the structure of the repository (found [Here](https://github.com/pedwoo/matrix_transposition.git)) follows:

```
gcc 01_transposition_sequential.c -o ./exec/01_transposition_sequential
```

Also to be noted is the method of registering times, for which we use `QueryPerformanceCounter`. This is a way of measuring time based on the clock frequency of the CPU, rather than a more commong wall clock, thus achieving higher precision, especially when working with very short times.

### 3.2 Implicit parallelism

A significant performance improvement can be done on the sequential approach by making use of implicit parallelism techniques like vectorization, prefetching, and overall optimizing the memory access patterns. An overview of how the code procedes to transpose the matrix once initiated follows:

-   A `blockSize` is defined in order to split the matrix into blocks (which are matrices themselves) and transpose them one block at a time. In the experiments 32 is always the value used, although using a smaller value would yield a greater performance improvement also for smaller matrices.  
    The reason to split the matrix is to improve cache locality by ensuring that the data is accessed in contiguous chunks, and reduce the amount of cache misses as the block is small enough that it can be fully cached, only causing cache misses when changing from one block to the next.
-   The boundaries of the current block are defined in order to be used.
-   The elements inside the boundaries just defined are processed. By using the type `__m128` we are able to store 4 float values (4 x 32-bit) in a single 128-bit register, thus improving the locality of the different cache accesses.  
    &nbsp;&nbsp;&nbsp;&nbsp;**1**. Values to be used in the next iteration are loaded to L1 cache  
    &nbsp;&nbsp;&nbsp;&nbsp;**2**. 4 floats are from the current row (ii) are contiguously saved in a single register  
    &nbsp;&nbsp;&nbsp;&nbsp;**3**. The 4 floats previously saved are loaded into the transposed matrix along column jj

A note has to be made about the symmetry check function (`checkSymImp`): this function uses the same principles mentioned above for the transposition, except the comparison of the "row elements" with the "column elements" which is done differently from the transposition, of course:

-   The elements of the row are loaded in `rowElems` (contiguously).
-   The elements of the column are loaded in `colElems` (contiguously).
-   They are compared using `_mm_cmpeq_ps` in order to compare the 128-bit registers directly.
-   The comparison, which was saved in `cmp` is converted into a bitmask. Each bit in the resulting integer corresponds to one of the comparison results. If the values are the same, the individual comparison returns 1, hence by checking that the final value is 0xF (all 1s), we want that all comparisons were successful.  
    Regardless of this, considering the symmetry check is a simple operation, adding all of this overhead only slows down the execution massively, even compared to the baseline sequential approach. For this reason, the parallel code is commented in the repository, in favour of a standard procedure.

The compilation command, which is in line with the structure of the repository (found [Here](https://github.com/pedwoo/matrix_transposition.git)) follows:

```
gcc -O2 -march=native 02_transposition_par_implicit.c -o ./exec/02_transposition_par_implicit
```

This is the combination of compiler flags/code optimization that returned the best performance increase ([Results here](#51-experimental-times-per-approach-and-matrix-size)). A brief explanation of the flags used follows:

-   `-O2`: Used to employ general compiler optimizations like loop unrolling, inlining functions and reducing unnecessary instructions.
-   `-march=native`: Used to enable the use of vectorization (SIMD instructions).

### 3.3 Parallelism through OpenMP

The compilation command, which is in line with the structure of the repository (found [Here](https://github.com/pedwoo/matrix_transposition.git)) follows:

```

```

## 4. Experiments and system description

### 4.1 System configuration

Let's begin our examination of the experiments by first taking a step back, and outlining the technical specifications of the computing environment. The system used to run the scripts locally comprises the following specifications:

-   **CPU**: AMD Ryzen 7840HS (8 cores, 16 threads) with a base clock speed of 3.8GHz and maximum boost clock of 5.1GHz.
-   **Memory**: 32GB LPDDR5x operating at 6400MT/s with dual channel configuration.
-   **Storage**: 1TB NVMe SSD.
-   **Operating system**: Windows 11 Home version 23H2.
-   **Compiler**: GCC 6.3.0 with OpenMP enabled.
-   **Additional libraries**: Standard C libraries for numerical computation and OpenMP for parallel processing.

Note that due to the system running windows, gcc compiler has to be installed separately, in this case through MinGW, and the gcc version is 6.3.0. Additional information to ensure an accurate reproduction of the instance used for testing is provided in section **6**.

### 4.2 Peak performance evaluation

We can now take a brief look at what the system we perform the experiments on could theoretically allow in terms of performance. Not all of these informations provided above about the system are useful to our evaluation, the relevant specifics follow:

-   **Memory clock speed**: 6400MT/s
-   **Memory bus width**: 32bits (4 bytes) per channel
-   **Memory channels**: 4 (supported by the RAM)
-   **Memory channels**: 2 (supported by the CPU)

For DDR type memory, bandwidth can be simply computed as:

<img style="display:block;margin:0 auto 10px;" src="https://latex.codecogs.com/png.image?\dpi{110}\fg{white}\text{Memory&bandwdith}=\text{Memory&clock&speed}\times\text{Memory&bus&width}\times\text{Number&of&channels}"/>

<img style="display:block;margin:0 auto 10px" src="https://latex.codecogs.com/png.image?\dpi{110}\fg{white}\text{Memory bandwidth}=6400\times10^6\times32\times4=819.2\text{Gbps}=102.4\text{GBps}"/>

Sadly, even though the RAM works in a 4-channel configuration, the CPU only allows for dual-channel memory access, effectively cutting in half the usable bandwidth. The value used in calculations will therefore be $51.2\text{GBps}$, even through a more powerful CPU would allow the use of the entire memory bandwidth.  
With this number in mind, we can go on to compute the theoretical times it would take to transpose each size of matrix supposing full memory bandwidth utilization. The formula follows:

<img style="display:block;margin:0 auto 10px" src="https://latex.codecogs.com/png.image?\dpi{110}\fg{white}\text{t}=\frac{\text{Total data transfer}}{\text{Memory bandwidth}}"/>

Computing this allows us two ways to approach the calculation of the effective memory bandwidth utilization (U) for each of the solution proposed: one way is to compute the ratio between the theoretical and experimental times (a), while the other is the ratio of experimental data transfer and what peak bandwidth would allow in that same amount of time (b)

<img style="display:block;margin:0 auto 10px" src="https://latex.codecogs.com/png.image?\dpi{110}\fg{white}\text{(a)}\text{U}=\frac{\text{Theoretical time}}{\text{Experimental time}}\times100"/>
<img style="display:block;margin:0 auto 10px" src="https://latex.codecogs.com/png.image?\dpi{110}\fg{white}\text{(b)}\text{U}=\frac{\text{Total data transfer}}{\text{Experimental time}\times\text{Peak bandwidth}}\times100"/>

## 5. Results

### 5.1 Experimental times per approach and matrix size

The results for each matrix size and approach follows. These results were obtained when testing locally.

#### 5.1.1 Transposition performance

<table border="1" style="margin:auto">
    <thead>
        <th><strong>Matrix Size</strong></th>
        <th><strong>Sequential (ms)</strong></th>
        <th><strong>Implicit (ms)</strong></th>
        <th><strong>OpenMP 1th (ms)</strong></th>
        <th><strong>OpenMP 16th (ms)</strong></th>
    </thead>
    <tbody>
        <tr>
            <td>16 × 16</td>
            <td>0.000431</td>
            <td>0.000136</td>
            <td>0.011179</td>
            <td>0.080900</td>
        </tr>
        <tr>
            <td>32 × 32</td>
            <td>0.001348</td>
            <td>0.000307</td>
            <td>0.011193</td>
            <td>0.071948</td>
        </tr>
        <tr>
            <td>64 × 64</td>
            <td>0.005646</td>
            <td>0.001130</td>
            <td>0.014490</td>
            <td>0.073297</td>
        </tr>
        <tr>
            <td>128 × 128</td>
            <td>0.024963</td>
            <td>0.005141</td>
            <td>0.020680</td>
            <td>0.078257</td>
        </tr>
        <tr>
            <td>256 × 256</td>
            <td>0.099544</td>
            <td>0.025121</td>
            <td>0.064853</td>
            <td>0.098842</td>
        </tr>
        <tr>
            <td>512 × 512</td>
            <td>0.406555</td>
            <td>0.134602</td>
            <td>0.251878</td>
            <td>0.150927</td>
        </tr>
        <tr>
            <td>1024 × 1024</td>
            <td>2.662161</td>
            <td>0.684287</td>
            <td>1.420957</td>
            <td>0.369677</td>
        </tr>
        <tr>
            <td>2048 × 2048</td>
            <td>15.576879</td>
            <td>5.287665</td>
            <td>9.913844</td>
            <td>3.156352</td>
        </tr>
        <tr>
            <td>4096 × 4096</td>
            <td>76.933626</td>
            <td>26.285314</td>
            <td>46.543102</td>
            <td>12.394242</td>
        </tr>
    </tbody>
</table>
<br/>

#### 5.1.2 Symmetry check performance

<table border="1" style="margin:auto">
    <thead>
        <th><strong>Matrix Size</strong></th>
        <th><strong>Sequential (ms)</strong></th>
        <th><strong>Implicit (ms)</strong></th>
        <th><strong>OpenMP 1th (ms)</strong></th>
        <th><strong>OpenMP 16th (ms)</strong></th>
    </thead>
    <tbody>
        <tr>
            <td>16 × 16</td>
            <td>0.000285</td>
            <td>0.000038</td>
            <td>0.000155</td>
            <td>0.101527</td>
        </tr>
        <tr>
            <td>32 × 32</td>
            <td>0.000701</td>
            <td>0.000021</td>
            <td>0.000351</td>
            <td>0.084870</td>
        </tr>
        <tr>
            <td>64 × 64</td>
            <td>0.001509</td>
            <td>0.000023</td>
            <td>0.001341</td>
            <td>0.087259</td>
        </tr>
        <tr>
            <td>128 × 128</td>
            <td>0.006815</td>
            <td>0.000023</td>
            <td>0.005797</td>
            <td>0.104125</td>
        </tr>
        <tr>
            <td>256 × 256</td>
            <td>0.021933</td>
            <td>0.000025</td>
            <td>0.022852</td>
            <td>0.123531</td>
        </tr>
        <tr>
            <td>512 × 512</td>
            <td>0.074801</td>
            <td>0.000029</td>
            <td>0.102316</td>
            <td>0.156315</td>
        </tr>
        <tr>
            <td>1024 × 1024</td>
            <td>0.268553</td>
            <td>0.000044</td>
            <td>0.532228</td>
            <td>0.501619</td>
        </tr>
        <tr>
            <td>2048 × 2048</td>
            <td>1.027298</td>
            <td>0.000209</td>
            <td>4.769207</td>
            <td>1.723141</td>
        </tr>
        <tr>
            <td>4096 × 4096</td>
            <td>4.051358</td>
            <td>0.000225</td>
            <td>26.055655</td>
            <td>8.155287</td>
        </tr>
    </tbody>
</table>
<br/>

Surprisingly, even though the sequential approach was much slower in transposing the matrices compared to OpenMP, when checking symmetry is proves substantially more efficient, especially when taking into consideration that the only direct comparison with it is the single-threaded OpenMP result, since the sequential approach operates on a single thread itself.  
The reason for this difference in performance is that when simply checking the symmetry of the matrix the overhead required to operate under OpenMP is completely overkill and causes the performance to drop.

Also to be taken into consideration is the fact that when compiling the OpenMP script we use the flags `-O3 -fopenmp` which, even though the two operate on different aspects of the optimization, cause somewhat of a conflict as some of the aggressive optimizations implied by `-O3` are prevented from coming into play by `-fopenmp` (this includes for example full loop unrolling, which for such a simple operation can be very beneficial).

Due to this exact same fact the implicit approach is much faster than both sequential and OpenMP approaches, as there isn't any overhead caused by multi-threading, and all the optimizations (this case contained in `-O2`) are actually implemented.

#### 5.1.3 OpenMP transposition performance

<table border="1" style="margin:auto">
    <thead>
            <th><strong>Matrix Size</strong></th>
            <th><strong>1 thread (ms)</strong></th>
            <th><strong>2 threads (ms)</strong></th>
            <th><strong>4 threads (ms)</strong></th>
            <th><strong>8 threads (ms)</strong></th>
            <th><strong>16 threads (ms)</strong></th>
    </thead>
    <tbody>
        <tr>
            <td>16 × 16</td>
            <td>0.011179</td>
            <td>0.030534</td>
            <td>0.041921</td>
            <td>0.054663</td>
            <td>0.080900</td>
        </tr>
        <tr>
            <td>32 × 32</td>
            <td>0.011193</td>
            <td>0.023435</td>
            <td>0.036481</td>
            <td>0.042075</td>
            <td>0.071948</td>
        </tr>
        <tr>
            <td>64 × 64</td>
            <td>0.014490</td>
            <td>0.030449</td>
            <td>0.034197</td>
            <td>0.048950</td>
            <td>0.073297</td>
        </tr>
        <tr>
            <td>128 × 128</td>
            <td>0.020680</td>
            <td>0.037482</td>
            <td>0.040542</td>
            <td>0.059454</td>
            <td>0.078257</td>
        </tr>
        <tr>
            <td>256 × 256</td>
            <td>0.064853</td>
            <td>0.070317</td>
            <td>0.067049</td>
            <td>0.071892</td>
            <td>0.098842</td>
        </tr>
        <tr>
            <td>512 × 512</td>
            <td>0.251878</td>
            <td>0.229961</td>
            <td>0.155771</td>
            <td>0.135454</td>
            <td>0.150927</td>
        </tr>
        <tr>
            <td>1024 × 1024</td>
            <td>1.420957</td>
            <td>0.988193</td>
            <td>0.646876</td>
            <td>0.451882</td>
            <td>0.369677</td>
        </tr>
        <tr>
            <td>2048 × 2048</td>
            <td>9.913844</td>
            <td>6.574907</td>
            <td>4.940285</td>
            <td>3.576542</td>
            <td>3.156352</td>
        </tr>
        <tr>
            <td>4096 × 4096</td>
            <td>46.543102</td>
            <td>29.410319</td>
            <td>21.338571</td>
            <td>14.922946</td>
            <td>12.394242</td>
        </tr>
    </tbody>
</table>
<br/>

From this data we can make pretty much the same observation we did before, which is that, due to the overhead introduced when using OpenMP, for small matrices, the performance is "capped" at the minimum time it takes to setup such overhead. This ultimately causes the times to be nearly constant across matrix sizes and thread counts, and often much slower than both sequential and implicit approaches.

### 5.2 Experimental times on the Unitn cluster

The results obtained when testing the code within the Unitn cluster follow. In this case for the OpenMP approaches, up to 96 threads were used (the maximum amount of threads in a single node).

#### 5.2.1 Transpositon performance on Unitn cluster

Units are omitted in order to make the table more readable. The first column doesn't have a unit as it is just the matrix size, all other columns are in `ms`

<table border="1" style="margin:auto">
    <thead>
        <th><strong>Matrix Size</strong></th>
        <th><strong>Sequential</strong></th>
        <th><strong>Implicit Time</strong></th>
        <th><strong>OpenMP 1t</strong></th>
        <th><strong>OpenMP 2t</strong></th>
        <th><strong>OpenMP 4t</strong></th>
        <th><strong>OpenMP 8t</strong></th>
        <th><strong>OpenMP 16t</strong></th>
        <th><strong>OpenMP 32t</strong></th>
        <th><strong>OpenMP 64t</strong></th>
        <th><strong>OpenMP 96t</strong></th>
    </thead>
    <tbody>
        <tr>
            <td>16x16</td>
            <td>0.000667</td>
            <td>0.000000</td>
            <td>0.005000</td>
            <td>0.039667</td>
            <td>0.047000</td>
            <td>0.080333</td>
            <td>0.144333</td>
            <td>0.264333</td>
            <td>0.503000</td>
            <td>9.582333</td>
        </tr>
        <tr>
            <td>32x32</td>
            <td>0.002333</td>
            <td>0.000667</td>
            <td>0.001333</td>
            <td>0.003667</td>
            <td>0.005333</td>
            <td>0.009000</td>
            <td>0.008000</td>
            <td>0.017000</td>
            <td>0.026000</td>
            <td>9.457000</td>
        </tr>
        <tr>
            <td>64x64</td>
            <td>0.009333</td>
            <td>0.001000</td>
            <td>0.004000</td>
            <td>0.036667</td>
            <td>0.031667</td>
            <td>0.024333</td>
            <td>0.027333</td>
            <td>0.029000</td>
            <td>0.040000</td>
            <td>4.695000</td>
        </tr>
        <tr>
            <td>128x128</td>
            <td>0.037000</td>
            <td>0.004000</td>
            <td>0.015667</td>
            <td>0.057000</td>
            <td>0.068333</td>
            <td>0.050000</td>
            <td>0.033667</td>
            <td>0.037000</td>
            <td>0.054667</td>
            <td>0.054000</td>
        </tr>
        <tr>
            <td>256x256</td>
            <td>0.153667</td>
            <td>0.016333</td>
            <td>0.054667</td>
            <td>0.162000</td>
            <td>0.117333</td>
            <td>0.093000</td>
            <td>0.073333</td>
            <td>0.047333</td>
            <td>0.050000</td>
            <td>0.070000</td>
        </tr>
        <tr>
            <td>512x512</td>
            <td>0.646333</td>
            <td>0.095333</td>
            <td>0.278333</td>
            <td>0.841333</td>
            <td>0.630333</td>
            <td>0.351667</td>
            <td>0.282667</td>
            <td>0.234333</td>
            <td>0.264667</td>
            <td>0.328333</td>
        </tr>
        <tr>
            <td>1024x1024</td>
            <td>2.638000</td>
            <td>0.419333</td>
            <td>1.146667</td>
            <td>2.649667</td>
            <td>1.669000</td>
            <td>0.954333</td>
            <td>0.585667</td>
            <td>0.500000</td>
            <td>0.460333</td>
            <td>0.463000</td>
        </tr>
        <tr>
            <td>2048x2048</td>
            <td>23.948333</td>
            <td>6.010333</td>
            <td>10.158333</td>
            <td>10.602333</td>
            <td>7.787000</td>
            <td>5.410667</td>
            <td>3.373667</td>
            <td>2.682000</td>
            <td>2.235000</td>
            <td>2.302667</td>
        </tr>
        <tr>
            <td>4096x4096</td>
            <td>123.396667</td>
            <td>41.008667</td>
            <td>66.783333</td>
            <td>52.660667</td>
            <td>43.164000</td>
            <td>29.565667</td>
            <td>26.597333</td>
            <td>36.311333</td>
            <td>25.712333</td>
            <td>20.174667</td>
        </tr>
    </tbody>
</table>
<br/>

#### 5.2.2 Symmetry check performance on Unitn cluster

Units are omitted in order to make the table more readable. The first column doesn't have a unit as it is just the matrix size, all other columns are in `ms`

<table border="1" style="margin:auto">
    <thead>
        <th><strong>Matrix Size</strong></th>
        <th><strong>Sequential</strong></th>
        <th><strong>Implicit Time</strong></th>
        <th><strong>OpenMP 1t</strong></th>
        <th><strong>OpenMP 2t</strong></th>
        <th><strong>OpenMP 4t</strong></th>
        <th><strong>OpenMP 8t</strong></th>
        <th><strong>OpenMP 16t</strong></th>
        <th><strong>OpenMP 32t (ms)</strong></th>
        <th><strong>OpenMP 64t</strong></th>
        <th><strong>OpenMP 96t</strong></th>
    </thead>
    <tbody>
        <tr>
            <td>16x16</td>
            <td>0.000333</td>
            <td>0.000333</td>
            <td>0.002333</td>
            <td>0.002000</td>
            <td>0.002000</td>
            <td>0.002000</td>
            <td>0.002333</td>
            <td>0.002000</td>
            <td>0.002000</td>
            <td>0.003000</td>
        </tr>
        <tr>
            <td>32x32</td>
            <td>0.001000</td>
            <td>0.000333</td>
            <td>0.001667</td>
            <td>0.000333</td>
            <td>0.001333</td>
            <td>0.001000</td>
            <td>0.001667</td>
            <td>0.001333</td>
            <td>0.001667</td>
            <td>0.001667</td>
        </tr>
        <tr>
            <td>64x64</td>
            <td>0.003333</td>
            <td>0.000333</td>
            <td>0.002000</td>
            <td>0.000333</td>
            <td>0.002000</td>
            <td>0.002000</td>
            <td>0.002000</td>
            <td>0.002000</td>
            <td>0.002000</td>
            <td>0.003000</td>
        </tr>
        <tr>
            <td>128x128</td>
            <td>0.012667</td>
            <td>0.002333</td>
            <td>0.006667</td>
            <td>0.002333</td>
            <td>0.006667</td>
            <td>0.006667</td>
            <td>0.006667</td>
            <td>0.006667</td>
            <td>0.006667</td>
            <td>0.006667</td>
        </tr>
        <tr>
            <td>256x256</td>
            <td>0.048667</td>
            <td>0.009000</td>
            <td>0.018000</td>
            <td>0.018000</td>
            <td>0.018000</td>
            <td>0.018000</td>
            <td>0.018000</td>
            <td>0.018000</td>
            <td>0.018667</td>
            <td>0.018667</td>
        </tr>
        <tr>
            <td>512x512</td>
            <td>0.189333</td>
            <td>0.035000</td>
            <td>0.067667</td>
            <td>0.035000</td>
            <td>0.035667</td>
            <td>0.035000</td>
            <td>0.035000</td>
            <td>0.035000</td>
            <td>0.035667</td>
            <td>0.035667</td>
        </tr>
        <tr>
            <td>1024x1024</td>
            <td>0.743000</td>
            <td>0.139000</td>
            <td>0.250667</td>
            <td>0.139000</td>
            <td>0.250667</td>
            <td>0.250667</td>
            <td>0.250667</td>
            <td>0.250667</td>
            <td>0.250667</td>
            <td>0.250667</td>
        </tr>
        <tr>
            <td>2048x2048</td>
            <td>2.948333</td>
            <td>0.544667</td>
            <td>0.889667</td>
            <td>0.544667</td>
            <td>0.544667</td>
            <td>0.544667</td>
            <td>0.544667</td>
            <td>0.544667</td>
            <td>0.544667</td>
            <td>0.544667</td>
        </tr>
        <tr>
            <td>4096x4096</td>
            <td>11.742000</td>
            <td>2.193667</td>
            <td>3.385000</td>
            <td>2.193667</td>
            <td>3.385000</td>
            <td>3.385000</td>
            <td>3.385000</td>
            <td>3.385000</td>
            <td>3.385000</td>
            <td>3.385000</td>
        </tr>
    </tbody>
</table>
<br/>

Some observations can be done after looking at the data provided.

-   The matrix sizes we have used for testing aren't large enough to the point where using a full 96 cores (1 node) or more is necessary at all, so after a certain matrix size or assigned thread count, providing more resources doesn't return any relevant improvement in the results.
-   As noted multiple times earlier, parallelization through OpenMP for the smaller matrix sizes obtains marginal improvements, if any, compared to the others. This is due to the overhead that the use of OpenMP introduces in the program. The results are in fact "capped" to how fast this overhead can execute, and therefore present very similar results, which isn't the case for any other approach.

### 5.3 Experimental memory bandwidth utilization

The following table contains the memory bandwidth utilization for each approach and matrix size, computed using the formulae above.

<table border="1" style="margin:auto">
    <thead>
        <th><strong>Matrix Size</strong></th>
        <th><strong>Data (MB)</strong></th>
        <th><strong>Ideal time (ms)</strong></th>
        <th><strong>Sequential (%)</strong></th>
        <th><strong>Implicit (%)</strong></th>
        <th><strong>OpenMP (%)</strong></th>
  </thead>
  <tbody>
    <tr>
        <td>16x16</td>
        <td>0.002</td>
        <td>0.000037</td>
        <td>8.063</td>
        <td>37.253</td>
        <td>0.042</td>
    </tr>
    <tr>
        <td>32x32</td>
        <td>0.008</td>
        <td>0.000149</td>
        <td>8.050</td>
        <td>63.141</td>
        <td>0.153</td>
    </tr>
    <tr>
        <td>64x64</td>
        <td>0.032</td>
        <td>0.000596</td>
        <td>7.657</td>
        <td>79.899</td>
        <td>0.628</td>
    </tr>
    <tr>
        <td>128x128</td>
        <td>0.128</td>
        <td>0.002384</td>
        <td>7.423</td>
        <td>49.691</td>
        <td>2.428</td>
    </tr>
    <tr>
        <td>256x256</td>
        <td>0.512</td>
        <td>0.009537</td>
        <td>7.407</td>
        <td>66.556</td>
        <td>8.085</td>
    </tr>
    <tr>
        <td>512x512</td>
        <td>2</td>
        <td>0.038147</td>
        <td>8.488</td>
        <td>39.095</td>
        <td>23.528</td>
    </tr>
    <tr>
        <td>1024x1024</td>
        <td>8</td>
        <td>0.152588</td>
        <td>5.470</td>
        <td>25.588</td>
        <td>34.612</td>
    </tr>
    <tr>
        <td>2048x2048</td>
        <td>32</td>
        <td>0.610352</td>
        <td>4.130</td>
        <td>13.642</td>
        <td>19.470</td>
    </tr>
    <tr>
        <td>4096x4096</td>
        <td>128</td>
        <td>2.441406</td>
        <td>3.380</td>
        <td>9.853</td>
        <td>18.326</td>
    </tr>
  </tbody>
</table>

### 5.4 Speedup compared to baseline approach

We can now also compare the three approaches in terms of speedup. By using the sequential approach as a baseline, we can express in percentage how much faster each of the other approaches are, per matrix size.  
We can compute the speedup using the formula that follows. Note that `Test time` represents the time for which we want to calculate the speedup:

<img style="display:block;margin:auto" src="https://latex.codecogs.com/png.image?\dpi{110}\fg{white}\text{Speedup}=\frac{\text{Baseline&space;time-Test&space;time}}{\text{Baseline&space;time}}\times100"/>

The results per approach and matrix size follow.

<table border="1" style="margin:auto">
    <thead>
        <th>Matrix Size</th>
        <th>Sequential Time (ms)</th>
        <th>Implicit Speedup (%)</th>
        <th>OpenMP Speedup (%)</th>
    </thead>
    <tbody>
        <tr>
            <td>16x16</td>
            <td>0.000462</td>
            <td>462</td>
            <td>0.52</td>
        </tr>
        <tr>
            <td>32x32</td>
            <td>0.001851</td>
            <td>784</td>
            <td>1.91</td>
        </tr>
        <tr>
            <td>64x64</td>
            <td>0.007784</td>
            <td>1046</td>
            <td>8.20</td>
        </tr>
        <tr>
            <td>128x128</td>
            <td>0.032118</td>
            <td>670</td>
            <td>32.70</td>
        </tr>
        <tr>
            <td>256x256</td>
            <td>0.128760</td>
            <td>899</td>
            <td>109.23</td>
        </tr>
        <tr>
            <td>512x512</td>
            <td>0.449411</td>
            <td>460</td>
            <td>277.41</td>
        </tr>
        <tr>
            <td>1024x1024</td>
            <td>2.789407</td>
            <td>468</td>
            <td>632.54</td>
        </tr>
        <tr>
            <td>2048x2048</td>
            <td>14.779019</td>
            <td>331</td>
            <td>471.50</td>
        </tr>
        <tr>
            <td>4096x4096</td>
            <td>72.229659</td>
            <td>293</td>
            <td>542.61</td>
        </tr>
    </tbody>
</table>

## 6.Conclusion

## 7. Installation and reproducibility of the results

The repository containing all the code for the project can be found here [Github link](https://github.com/pedwoo/matrix_transposition.git).  
After downloading the code (or cloning through the link), if not already installed, install gcc compiler. On a Linux machine this can simply be done from a command prompt, while on Windows third-party software is required. Since testing was also done on a Windows machine MinGW was used.  
The specific configurations for MinGW can be found in the `utils` folder of the repository. The file in there was obtained by running the following command: `mingw-get list mingw-components.txt`. To reproduce the instance of MinGW, complete the installation through the installer, then in a command prompt use `mingw-get install <package-name>` to install the different packages.

To reproduce the results on the Unitn cluster (supposing one has access to it, of course), the code in the folder `cluster code` can just be copied and pasted alongside the `matrix_transposition.pbs` file, which will compile and run automatically all the scripts. Additionally, the contents of `.bashrc` should be copied or merged into an existing equal file in the cluster directory.

## 8. References
