# CSC586C---Project - REPORT 2

# The main branch is the only branch that contains all the files related to report 2. 

## How to Compile The code in Linux:

## spatialjointerminal_final.cpp is the file that contains all the different spatial join functions (naive, naive_soa, tiling, and simd). This file should be run to see the time for all the optimizations in one-go.


From the terminal, go into the directory where spatialjointerminal_final.cpp is and type the following command:

**g++ -Wall -Wextra -mavx -march=native -std=c++17 -g -O3 -o spatial spatialjointerminal_final.cpp**


## To run the code after compiling type:

./spatial 
