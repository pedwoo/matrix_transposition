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

### 1.1 Current research and development

As parallel computation became more popular and necessary in recent years, various strategies have been proposed to optimize operations such as matrix transposition, especially when working with large-scale computations.  
This section will present these strategies along with their limitations, to finally define the gap our project aims to address.

### 1.2 Limitations in existing solutions

In spite of the advances that are being made in parallel computing for matrix operations, some limitations remain across existing solutions:

-   **OpenMP** has limited scalability with very large matrices, due to a high thread synchronization overhead and issues with memory contention. The performance gain is very often limited for larger matrices since the problem becomes more memory-bound than intensive in terms of computations.
-   **MPI** is highly efficient for distributed systems, but also faces a high communication overhead, which hinders its performance for smaller-sized matrices. This is due to a high cost of internal process commmunication that ends up outweighing the benefits of such an approach.
-   **CUDA** offers excellent performance on devices that are equipped with a GPU, but is often limited by its strict hardware dependency, since it requires specific optimizations based on the GPU.
-   **Hybrid approaches** are able to deliver superior performance but require complex tuning and resource management, which ends up having the same per-hardware specificity issue that CUDA presents.

The combination of these issues ultimately lead to the impossibility to identify a "one size fits all" solution to the problem, and a context specific approach is to be preferred.

### 1.3 Proposed solutions

## 2. Contribution and methodology

## 3. Experiments and system description

### Computing system and platform

Let's begin our examination of the experiments by first taking a step back, and outlining the technical specifications of the computing environment. This windows-based system features an AMD Ryzen 7 7840HS CPU and 32GB of LPDDR5X-6400 RAM, mounted across four slots.

### Specifications and configurations

### Peak performance evaluation

We can now take a brief look at what the system we perform the experiments on could theoretically allow in terms of performance. As mentioned before, the machine in question's memory is composed of 4 sticks of 8GB LPDDR5X-6400 memory (32GB total) and mounts a Ryzen 7 7840HS CPU. Not all of these informations are useful to our evaluation, the relevant specifics follow:

-   Memory clock speed: 6400MT/s
-   Memory bus width: 32bits (8 bytes) per channel
-   Memory channels: 4 (supported by the RAM)
-   Memory channels: 2 (supported by the CPU)

For DDR type memory, bandwidth can be simply computed as:
$$\text{Memory bandwdith}=\text{Memory clock speed}\times\text{Memory bus width}\times\text{Number of channels}$$
Sadly, although the RAM would allow for 4-channel access, the CPU only supports 2-channel access, effectively cutting in half the actual achievable memory bandwidth, for a total of 102.4GB/s. This value will be used for the calculation, even though with a more powerful CPU the memory could allow higher transfer speeds.

With this number in mind, we can go on to compute the theoretical times it would take to transpose each size of matrix supposing full memory bandwidth utilization. The formula follows:

$$
\text{t}=\frac{\text{Total data transfer}}{\text{Memory bandwidth}}
$$

Computing this allows us two ways to approach the calculation of the effective memory bandwidth utilization (U) for each of the solution proposed: one way is to compute the ratio between the theoretical and experimental times (a), while the other is the ratio of experimental data transfer and what peak bandwidth would allow in that same amount of time (b)

$$
\text{(a)}~~\text{U}=\frac{\text{Theoretical time}}{\text{Experimental time}}\times 100
$$

$$
\text{(b)}~~\text{U}=\frac{\text{Total data transfer}}{\text{Experimental time}\times\text{Peak bandwidth}}\times 100
$$

## 4. Results

The results for each matrix size and approach follows. These results were obtained when testing locally.

<p align="center">
  <table border="1">
    <tr>
      <th>Size</th>
      <th>Data (MB)</th>
      <th>Sequential (ms)</th>
      <th>Implicit (ms)</th>
      <th>OpenMP (ms)</th>
    </tr>
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
  </table>
</p>

The results obtained when testing the code within the Unitn cluster follow

TABLE CONTAINING RESULTS FROM THE CLUSTER

Some observations can be done after looking at the data provided.

-   The matrix sizes we have used for testing aren't large enough to the point where using a full 96 cores (1 node) or more is necessary at all, so after INSERT WHEN IT DOESN'T CHANGE ANYMORE providing more resources doesn't return any relevant improvement in the results.
-   Parallelization through OpenMP for the smaller matrix sizes obtains marginal improvements, if any, compared to the others. This is due to the overhead that the use of OpenMP introduces in the program. The results are in fact "capped" to how fast this overhead can execute, and therefore present very similar results, which isn't the case for any other approach.

### Table: Experimental memory bandwidth utilization

The following table contains the memory bandwidth utilization for each approach and matrix size, computed using the formulae above.

<table border="1" style="margin:auto">
<tr>
    <th>Size</th>
    <th>Data (MB)</th>
    <th>Sequential (%)</th>
    <th>Implicit (%)</th>
    <th>OpenMP (%)</th>
</tr>
<tr>
    <td>16 x 16</td>
    <td>0.001</td>
    <td>2.016</td>
    <td>9.313</td>
    <td>0.011</td>
</tr>
<tr>
    <td>32 x 32</td>
    <td>0.003</td>
    <td>2.013</td>
    <td>15.785</td>
    <td>0.038</td>
</tr>
<tr>
    <td>64 x 64</td>
    <td>0.015</td>
    <td>1.914</td>
    <td>19.975</td>
    <td>0.157</td>
</tr>
<tr>
    <td>128 x 128</td>
    <td>0.062</td>
    <td>1.856</td>
    <td>12.423</td>
    <td>0.607</td>
</tr>
<tr>
    <td>256 x 256</td>
    <td>0.250</td>
    <td>1.852</td>
    <td>16.639</td>
    <td>2.021</td>
</tr>
<tr>
    <td>512 x 512</td>
    <td>1.000</td>
    <td>2.122</td>
    <td>9.774</td>
    <td>5.882</td>
</tr>
<tr>
    <td>1024 x 1024</td>
    <td>4.000</td>
    <td>1.368</td>
    <td>6.397</td>
    <td>8.653</td>
</tr>
<tr>
    <td>2048 x 2048</td>
    <td>16.000</td>
    <td>1.032</td>
    <td>3.410</td>
    <td>4.868</td>
</tr>
<tr>
    <td>4096 x 4096</td>
    <td>64.000</td>
    <td>0.845</td>
    <td>2.463</td>
    <td>4.582</td>
</tr>
</table>

## 5.Conclusion
