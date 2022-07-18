# CSC586C---Project - REPORT 2

# The main branch is the only branch that contains all the files related to report 2. 

## How to Compile The code in Linux:

## NOTE: spatialjointerminal_final.cpp is the file that contains all the different spatial join functions (naive, naive_soa, tiling, and simd). This file should ideally be the only file ran to see the time for all the optimizations in one-go.


From the terminal, go into the directory where spatialjointerminal_final.cpp is and type the following command:

**g++ -Wall -Wextra -mavx -march=native -std=c++17 -g -O3 -fopenmp -o spatial spatialjointerminal_final.cpp**


**To run the code after compiling type:** ./spatial 


## If needed, you can see the output of the different spatial join functions individually through using the other 'split' .cpp files (final_split_naive, final_split_simd, final_split_threading etc.) in the main branch. They will show individual times only

To compile the split files,  go into the directory where the 'split' cpp files are and type the following command:


**g++ -Wall -Wextra -mavx -march=native -std=c++17 -g -O3 -fopenmp -o split_spatial [split_filename].cpp**

**To run the code after compiling type:** ./split_spatial 

