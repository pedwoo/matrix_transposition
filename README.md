# matrix_transposition

Exploring different parallelization technologies to optimize matrix transposition

## 0. Introduction

The goal of this project is to explore and evaluate various parallelization techniques in order to identify the most efficient solution for the given problem. Through this analysis, we aim to either pinpoint the optimal approach or, at the very least, provide comprehensive results and insights to support informed decision-making within the context of the problem.  
First, we shall take a step back and understand why we are undertaking this task in the first place, and what its importance is.

### 0.1 What's the importance of matrix transposition

Matrices are fundamental in algebra, as they can encode a great amount of information that has diverse applications. Particularly, matrices that represent linear transformation are of great interest due to their wide-ranging utility since, by multiplying a specific matrix by any input vector, predictable transformations can be achieved, allowing consistent manipulations of vectors across various contexts.

### 0.2 Common applications

Matrices and transposition play crucial roles in various fields, including:

-   **Deep learning**: Particularly withing neural networks and convolutional layers, where data often requires reshaping or alignment for specific layer operations. Large input batches may need to be transposed to fit model expectations.
-   **Graphics and image processing**: Operations like rotations, reflections, and texture mapping are used extensively, where matrix transposition facilitates precise image representation and manipulations.
-   **Scientific and numerical simulations**: In fields like physics and chemistry, matrices represent large datasets and enable complex transformations, crucial for realistic simulations.
-   **Signal processing**: Matrices represent signal data, which may be transposed to align with computational steps such as those in Fourier Transform applications.

## 1. State of the art

As parallel computation became more popular and necessary in recent years, various strategies have been proposed to optimize operations such as matrix transposition, especially when working with large-scale computations.  
This section will present these strategies along with their limitations, to finally define the gap our project aims to address.

### 1.1 Current research and development

#### 1.1.1 Parallelization with OpenMP

OpenMP is an extensively used framework for parallelizing programs on multi-core CPUs. It uses compuler directives, which enable developers to easily parallelize different sections of a program, like loops and functions, without significant changes to the code. OpenMP also offers solid scalability features, which make it suitable for matrix operations on a multi-core system.

#### 1.1.2 Parallelization with MPI

MPI (Message Passing Interface) is an established method for parallelizing computation on distributed memory systems. It allows different processes to communicate, making it suitable for large scale, distributed computing systems. When working on matrix transposition, MPI can split the matrix in different blocks and distribute them over multiple different nodes, allowing for parallel execution and at the same time minimizing the impact of memory limitations on individual processors.

#### 1.1.3 GPU-based parallelization with CUDA

CUDA is a platform developed by NVIDIA that allows for GPU-accelerated matrix computation. Since GPUs offer a high degree of parallelism, CUDA can offer significant performance improvements in tasks such as matrix transposition. It is also worth mentioning that GPU memory normally has a much higher bandwidth compared to CPU memory, therefore making CUDA particularly effective for memory-bound operations.

### 1.2 Limitations in existing solutions

In spite of the advances that are being made in parallel computing for matrix operations, some limitations remain across existing solutions:

#### 1.2.1 Limitations of OpenMP

OpenMP has limited scalability with very large matrices, due to a high thread synchronization overhead and issues with memory contention. The performance gain is very often limited for larger matrices since the problem becomes more memory-bound than intensive in terms of computations.

#### 1.2.2 Limitations of MPI

MPI is highly efficient for distributed systems, but also faces a high communication overhead, which hinders its performance for smaller-sized matrices. This is due to a high cost of internal process commmunication that ends up outweighing the benefits of such an approach.

#### 1.2.3 Limitations of CUDA

CUDA offers excellent performance on devices that are equipped with a GPU, but is often limited by its strict hardware dependency, since it requires specific optimizations based on the GPU.

#### 1.2.4 Limitations of hybrid approaches

Hybrid approaches are able to deliver superior performance but require complex tuning and resource management, which ends up having the same per-hardware specificity issue that CUDA presents.

### 1.3 Proposed solutions

The combination of the issues mentioned above ultimately lead to the impossibility to identify a "one size fits all" solution to the problem, and a context specific approach is to be preferred.

## 2. Contribution and methodology

## 3. Experiments and system description

### 3.1 System configuration

Let's begin our examination of the experiments by first taking a step back, and outlining the technical specifications of the computing environment. The system used to run the scripts locally comprises the following specifications:

-   **CPU**: AMD Ryzen 7840HS (8 cores, 16 threads) with a base clock speed of 3.8GHz and maximum boost clock of 5.1GHz.
-   **Memory**: 32GB LPDDR5x operating at 6400MT/s with dual channel configuration.
-   **Storage**: 1TB NVMe SSD.
-   **Operating system**: Windows 11 Home version 23H2.
-   **Compiler**: GCC 6.3.0 with OpenMP enabled.
-   **Additional libraries**: Standard C libraries for numerical computation and OpenMP for parallel processing.

Note that due to the system running windows, gcc compiler has to be installed separately, in this case through MinGW, and the gcc version is 6.3.0. Additional information to ensure an accurate reproduction of the instance used for testing is provided in section **6**.

### 3.2 Peak performance evaluation

We can now take a brief look at what the system we perform the experiments on could theoretically allow in terms of performance. Not all of these informations provided above about the system are useful to our evaluation, the relevant specifics follow:

-   **Memory clock speed**: 6400MT/s
-   **Memory bus width**: 32bits (4 bytes) per channel
-   **Memory channels**: 4 (supported by the RAM)
-   **Memory channels**: 2 (supported by the CPU)

For DDR type memory, bandwidth can be simply computed as:
$$\text{Memory bandwdith}=\text{Memory clock speed}\times\text{Memory bus width}\times\text{Number of channels}$$
$$\text{Memory bandwidth}=6400\times10^6\times32\times4=819.2\text{Gbps}=102.4\text{GBps}$$
Sadly, even though the RAM works in a 4-channel configuration, the CPU only allows for dual-channel memory access, effectively cutting in half the usable bandwidth. The value used in calculations will therefore be $51.2\text{GBps}$, even through a more powerful CPU would allow the use of the entire memory bandwidth.  
With this number in mind, we can go on to compute the theoretical times it would take to transpose each size of matrix supposing full memory bandwidth utilization. The formula follows:

$$
\text{t}=\frac{\text{Total data transfer}}{\text{Memory bandwidth}}
$$

Computing this allows us two ways to approach the calculation of the effective memory bandwidth utilization (U) for each of the solution proposed: one way is to compute the ratio between the theoretical and experimental times (a), while the other is the ratio of experimental data transfer and what peak bandwidth would allow in that same amount of time (b)

$$
\text{(a)}~~\text{U}=\frac{\text{Experimental time}}{\text{Theoretical time}}\times 100
$$

$$
\text{(b)}~~\text{U}=\frac{\text{Total data transfer}}{\text{Experimental time}\times\text{Peak bandwidth}}\times 100
$$

## 4. Results

#### Table: Experimental times per approach and matrix size

The results for each matrix size and approach follows. These results were obtained when testing locally.

<table border="1" style="margin:auto">
    <thead>
        <th>Size</th>
        <th>Data (MB)</th>
        <th>Sequential (ms)</th>
        <th>Implicit (ms)</th>
        <th>OpenMP (ms)</th>
    </thead>
    <tbody>
        <tr>
            <td>16 x 16</td>
            <td>0.001</td>
            <td>0.000462</td>
            <td>0.000100</td>
            <td>0.088406</td>
        </tr>
        <tr>
            <td>32 x 32</td>
            <td>0.003</td>
            <td>0.001851</td>
            <td>0.000236</td>
            <td>0.097082</td>
        </tr>
        <tr>
            <td>64 x 64</td>
            <td>0.015</td>
            <td>0.007784</td>
            <td>0.000746</td>
            <td>0.094980</td>
        </tr>
        <tr>
            <td>128 x 128</td>
            <td>0.062</td>
            <td>0.032118</td>
            <td>0.004798</td>
            <td>0.098189</td>
        </tr>
        <tr>
            <td>256 x 256</td>
            <td>0.250</td>
            <td>0.128760</td>
            <td>0.014329</td>
            <td>0.117963</td>
        </tr>
        <tr>
            <td>512 x 512</td>
            <td>1.000</td>
            <td>0.449411</td>
            <td>0.097574</td>
            <td>0.162131</td>
        </tr>
        <tr>
            <td>1024 x 1024</td>
            <td>4.000</td>
            <td>2.789407</td>
            <td>0.596318</td>
            <td>0.440855</td>
        </tr>
        <tr>
            <td>2048 x 2048</td>
            <td>16.000</td>
            <td>14.779019</td>
            <td>4.474166</td>
            <td>3.134817</td>
        </tr>
        <tr>
            <td>4096 x 4096</td>
            <td>64.000</td>
            <td>72.229659</td>
            <td>24.779411</td>
            <td>13.321789</td>
        </tr>
    </tbody>
</table>
<br>

#### Table: Experimental times on the Unitn cluster

The results obtained when testing the code within the Unitn cluster follow. Only the OpenMP approach is in the table, alongside the sequential one which is only there to set a baseline for comparison. Additionally, results from small matrices have been removed, since they wouldn't hold any significance as the overhead from OpenMP bottlenecks the results, hence using more cores wouldn't cause any increase in performance.

Some observations can be done after looking at the data provided.

-   The matrix sizes we have used for testing aren't large enough to the point where using a full 96 cores (1 node) or more is necessary at all, so after INSERT WHEN IT DOESN'T CHANGE ANYMORE providing more resources doesn't return any relevant improvement in the results.
-   Parallelization through OpenMP for the smaller matrix sizes obtains marginal improvements, if any, compared to the others. This is due to the overhead that the use of OpenMP introduces in the program. The results are in fact "capped" to how fast this overhead can execute, and therefore present very similar results, which isn't the case for any other approach.

#### Table: Experimental memory bandwidth utilization

The following table contains the memory bandwidth utilization for each approach and matrix size, computed using the formulae above.

<table border="1" style="margin:auto">
    <thead>
        <th>Matrix Size</th>
        <th>Data (MB)</th>
        <th>Ideal time (ms)</th>
        <th>Sequential (%)</th>
        <th>Implicit (%)</th>
        <th>OpenMP (%)</th>
  </thead>
  <tbody>
    <tr>
        <td>16 x 16</td>
        <td>0.002</td>
        <td>0.000037</td>
        <td>8.063</td>
        <td>37.253</td>
        <td>0.042</td>
    </tr>
    <tr>
        <td>32 x 32</td>
        <td>0.008</td>
        <td>0.000149</td>
        <td>8.050</td>
        <td>63.141</td>
        <td>0.153</td>
    </tr>
    <tr>
        <td>64 x 64</td>
        <td>0.032</td>
        <td>0.000596</td>
        <td>7.657</td>
        <td>79.899</td>
        <td>0.628</td>
    </tr>
    <tr>
        <td>128 x 128</td>
        <td>0.128</td>
        <td>0.002384</td>
        <td>7.423</td>
        <td>49.691</td>
        <td>2.428</td>
    </tr>
    <tr>
        <td>256 x 256</td>
        <td>0.512</td>
        <td>0.009537</td>
        <td>7.407</td>
        <td>66.556</td>
        <td>8.085</td>
    </tr>
    <tr>
        <td>512 x 512</td>
        <td>2</td>
        <td>0.038147</td>
        <td>8.488</td>
        <td>39.095</td>
        <td>23.528</td>
    </tr>
    <tr>
        <td>1024 x 1024</td>
        <td>8</td>
        <td>0.152588</td>
        <td>5.470</td>
        <td>25.588</td>
        <td>34.612</td>
    </tr>
    <tr>
        <td>2048 x 2048</td>
        <td>32</td>
        <td>0.610352</td>
        <td>4.130</td>
        <td>13.642</td>
        <td>19.470</td>
    </tr>
    <tr>
        <td>4096 x 4096</td>
        <td>128</td>
        <td>2.441406</td>
        <td>3.380</td>
        <td>9.853</td>
        <td>18.326</td>
    </tr>
  </tbody>
</table>

#### Table: speedup compared to sequential approach

We can now also compare the three approaches in terms of speedup. By using the sequential approach as a baseline, we can express in percentage how much faster each of the other approaches are, per matrix size.  
We can compute the speedup using the formula that follows. Note that `Test time` represents the time for which we want to calculate the speedup:

$$
\text{Speedup (\%)}=\frac{\text{Baseline time - Test time}}{\text{Baseline time}}\times100
$$

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
            <td>16 x 16</td>
            <td>0.000462</td>
            <td>462</td>
            <td>0.52</td>
        </tr>
        <tr>
            <td>32 x 32</td>
            <td>0.001851</td>
            <td>784</td>
            <td>1.91</td>
        </tr>
        <tr>
            <td>64 x 64</td>
            <td>0.007784</td>
            <td>1046</td>
            <td>8.20</td>
        </tr>
        <tr>
            <td>128 x 128</td>
            <td>0.032118</td>
            <td>670</td>
            <td>32.70</td>
        </tr>
        <tr>
            <td>256 x 256</td>
            <td>0.128760</td>
            <td>899</td>
            <td>109.23</td>
        </tr>
        <tr>
            <td>512 x 512</td>
            <td>0.449411</td>
            <td>460</td>
            <td>277.41</td>
        </tr>
        <tr>
            <td>1024 x 1024</td>
            <td>2.789407</td>
            <td>468</td>
            <td>632.54</td>
        </tr>
        <tr>
            <td>2048 x 2048</td>
            <td>14.779019</td>
            <td>331</td>
            <td>471.50</td>
        </tr>
        <tr>
            <td>4096 x 4096</td>
            <td>72.229659</td>
            <td>293</td>
            <td>542.61</td>
        </tr>
    </tbody>
</table>

## 5.Conclusion

## 6. Installation and reproducibility of the results

The repository containing all the code for the project can be found here [Github link](https://github.com/pedwoo/matrix_transposition.git).  
After downloading the code (or cloning through the link), if not already installed, install gcc compiler. On a Linux machine this can simply be done from a command prompt, while on Windows third-party software is required. Since testing was also done on a Windows machine MinGW was used.  
The specific configurations for MinGW can be found in the `utils` folder of the repository. The file in there was obtained by running the following command: `mingw-get list mingw-components.txt`. To reproduce the instance of MinGW, complete the installation through the installer, then in a command prompt use `mingw-get install <package-name>` to install the different packages.
