# CSC586C---Project - REPORT 3

# The main branch is the only branch that contains all the files related to report 3. 

## How to Compile The code in Linux:

## NOTE: spatialjoincuda_final.cu is the file that contains all the different spatial join functions related to the GPU spatial join, SIMD+openmp, and tiling spatial join. This file should ideally be the only file ran to see the execution time for all the optimizations in one-go.


From the terminal, go into the directory where spatialjointerminal_final.cpp is and type the following command:

**nvcc spatialjoincuda_final.cu -Xcompiler -fopenmp -Xcompiler -march=native --expt-relaxed-constexpr -O3 -o spatialcuda**


**To run the code after compiling type:** ./spatialcuda


## If needed, you can see the output of the cuda spatial join function ONLY through using the cuda_diff.cu file in the main branch. 

To compile the split files,  go into the directory where cuda_diff.cu is and type the following command:


**nvcc cuda_diff.cu -Xcompiler -fopenmp -Xcompiler -march=native --expt-relaxed-constexpr -O3 -o diffcuda**

**To run the code after compiling type:** ./diffcuda

