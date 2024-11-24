# matrix_transposition

Exploring different parallelization technologies to optimize matrix transposition

## 0. Introduction

## 1. State of the art

## 2. Contribution and methodology

## 3. Experiments and system description

## 4. Results

The results for each matrix size and approach follows.
| Size | Data (MB) | Sequential (ms) | Implicit (ms) | OpenMP (ms) |
| - | - | - | - | - |
| 16 x 16 | 0.001 | 0.000462 | 0.000100 | 0.088406 |
| 32 x 32 | 0.003 | 0.001851 | 0.000236 | 0.097082 |
| 64 x 64 | 0.015 | 0.007784 | 0.000746 | 0.094980 |
| 128 x 128 | 0.062 | 0.032118 | 0.004798 | 0.098189 |
| 256 x 256 | 0.250 | 0.128760 | 0.014329 | 0.117963 |
| 512 x 512 | 1.000 | 0.449411 | 0.097574 | 0.162131 |
| 1024 x 1024 | 4.000 | 2.789407 | 0.596318 | 0.440855 |
| 2048 x 2048 | 16.000 | 14.779019 | 4.474166 | 3.134817 |
| 4096 x 4096 | 64.000 | 72.229659 | 24.779411 | 13.321789 |

#### Peak performance evaluation

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

#### Memory bandwidth utilization

The following table shows the utilization of the memory bandwidth, based on the calculations in section INSERT SECTION HERE
| Size | Data (MB) | Sequential (%) | Implicit (%) | OpenMP (%) |
| - | - | - | - | - |
| 16 x 16 | 0.001 | 2.016 | 9.313 | 0.011 |
| 32 x 32 | 0.003 | 2.013 | 15.785| 0.038 |
| 64 x 64 | 0.015 | 1.914 | 19.975| 0.157 |
| 128 x 128 | 0.062 | 1.856 | 12.423| 0.607 |
| 256 x 256 | 0.25 | 1.852 | 16.639| 2.021 |
| 512 x 512 | 1 | 2.122 | 9.774 | 5.882 |
| 1024 x 1024 | 4 | 1.368 | 6.397 | 8.653 |
| 2048 x 2048 | 16 | 1.032 | 3.410 | 4.868 |
| 4096 x 4096 | 64 | 0.845 | 2.463 | 4.582 |

## 5.Conclusion
