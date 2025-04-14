# AlignedVsMisaligned

## Table of Contents
- [Introduction](#introduction)
- [Build and Run](#build-and-run)
- [Possible Output](#possible-output)
- [How does this work](#how-does-this-work)

## Introduction
This project compares the performance of scalar and SIMD-based summation algorithms under different memory alignment scenarios. It highlights how alignment and vector size impact execution time, providing insights into optimization techniques for numerical computations.

## Build and Run
To clone and run this project, ensure you have [Git](https://git-scm.com) and [CMake](https://cmake.org/) installed. Use the following commands:

```bash
# Clone this repository
$ git clone https://github.com/Hayk10002/AlignedVsMisaligned

# Navigate to the repository
$ cd AlignedVsMisaligned

# Generate build files
$ cmake -DCMAKE_BUILD_TYPE=Release -S . -B build

# Build the project
$ cmake --build build --config Release

# Run the executable with the desired vector size
$ your/path/to/exe/main.exe {vector_size}
# Example: .../main.exe 300000000
```

## Possible Output
Example output for a vector size of 300000000:

```
Running with vector size: 300000000
Scalar addition (11b aligned): Time: 1458ms, Sum: 298355040
SIMD   addition (11b aligned): Time:  294ms, Sum: 298355040
Scalar addition (16b aligned): Time: 1249ms, Sum: 298355040
SIMD   addition (16b aligned): Time:  236ms, Sum: 298355040
Scalar addition (32b aligned): Time: 1116ms, Sum: 298355040
SIMD   addition (32b aligned): Time:  227ms, Sum: 298355040
--------------------------
```

## How does this work
This project evaluates the performance of scalar and SIMD-based summation algorithms under varying memory alignment scenarios. By analyzing execution times for different alignments and vector sizes, it demonstrates the impact of memory alignment on computational efficiency and provides insights into optimization strategies for numerical computations. The overall sum is displayed in output to ensure the programm did the right thing, as alignments not divisible by 4 are `Undefined Behavior` and so with the 11b alignment code the compiler can do whatever it wants, so if the sum isn't consistent, the time results are invalid. 
