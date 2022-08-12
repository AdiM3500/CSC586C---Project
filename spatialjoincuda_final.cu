#include <iostream>
#include <algorithm>
#include<iterator>
#include <cmath>
#include <optional>
#include <utility>
#include <vector>
#include <array>
#include <variant>
#include <queue>
#include <tuple>
#include <chrono>
#include <random>
#include <immintrin.h>
#include <emmintrin.h>
#include <omp.h>


//global variable to keep track of the number of joins that occured
long long int joins_count = 0;
long long int naive_joins_count = 0;
long long int naive_naive_joins_count = 0;
long long int tiling_joins_count = 0;
using coordinate_t = std::pair <int, int>;
using pair_of_coordinates_t = std::pair<coordinate_t, coordinate_t>;
using rectangle_list_t = std::vector<pair_of_coordinates_t>;
using spatial_join_list_t = std::vector<std::pair < pair_of_coordinates_t, pair_of_coordinates_t> >;

class RectangleSet_Coordinates {

public:

	int x_min[100000];
	int y_min[100000];
	int x_max[100000];
	int y_max[100000];
	int count = 0;

	RectangleSet_Coordinates(rectangle_list_t const& coords_to_insert) {

		//x_min.resize(coords_to_insert.size()); y_min.resize(coords_to_insert.size()); x_max.resize(coords_to_insert.size()); y_max.resize(coords_to_insert.size());

		for (auto i = 0; i < coords_to_insert.size(); i++) {

			x_min[i] = coords_to_insert[i].first.first;
			y_min[i] = coords_to_insert[i].first.second;

			x_max[i] = coords_to_insert[i].second.first;
			y_max[i] = coords_to_insert[i].second.second;
			count++;
		}

	}


};



void NAIVE_SOA_SPATIAL_JOIN(RectangleSet_Coordinates R, RectangleSet_Coordinates S) {

	for (auto i = 0; i < R.count; i++) {

		//inner loop iterating through S
		for (auto j = 0; j < S.count; j++) {

			if ((R.x_min[i] < S.x_max[j])
				&& (R.x_max[i] > S.x_min[j])
				&& (R.y_min[i] < S.y_max[j])
				&& (R.y_max[i] > S.y_min[j])
				) {

				naive_joins_count += 1;
			}


			//condition to check if a particular MBR object in R overlaps with ANY objects in S

		}
	}

}


void TILING_SOA_SPATIAL_JOIN(RectangleSet_Coordinates R, RectangleSet_Coordinates S) {

	//outer loop iterating through R
	for (auto i = 0; i < R.count - 7; i += 8) {

		for (auto j = 0; j < S.count; j++) {


				for (auto k = 0; k < 8; k++) {

					if ((R.x_min[i + k] < S.x_max[j])
						&& (R.x_max[i + k] > S.x_min[j])
						&& (R.y_min[i + k] < S.y_max[j])
						&& (R.y_max[i + k] > S.y_min[j])
						) {

						tiling_joins_count += 1;
					}

				}
			
		}

	}


}


void SIMD_SPATIAL_JOIN(RectangleSet_Coordinates R, RectangleSet_Coordinates S) {

//	int  num_threads = 6;
//	omp_set_num_threads(num_threads);

	for (auto i = 0; i < R.count - 7; i += 8) {

		#pragma omp parallel for reduction (+:joins_count) 
		for (auto j = 0; j < S.count; j++) {

            for(auto k = 0; k < 8; k+=4){

                    

				__m128i setR_min_x = _mm_set_epi32(R.x_min[i + k], R.x_min[i + k + 1], R.x_min[i + k + 2], R.x_min[i + k + 3]);
				__m128i setR_min_y = _mm_set_epi32(R.y_min[i + k], R.y_min[i + k + 1], R.y_min[i + k + 2], R.y_min[i + k + 3]);
				__m128i	setR_max_x = _mm_set_epi32(R.x_max[i + k], R.x_max[i+ k + 1 ], R.x_max[i + k + 2], R.x_max[i + k + 3]);
				__m128i	setR_max_y = _mm_set_epi32(R.y_max[i + k], R.y_max[i + k + 1], R.y_max[i + k + 2], R.y_max[i + k + 3]);


				__m128i setS_min_x = _mm_set_epi32(S.x_min[j], S.x_min[j], S.x_min[j], S.x_min[j]);
				__m128i setS_min_y = _mm_set_epi32(S.y_min[j], S.y_min[j], S.y_min[j], S.y_min[j]);
				__m128i	setS_max_x = _mm_set_epi32(S.x_max[j], S.x_max[j], S.x_max[j], S.x_max[j]);
				__m128i	setS_max_y = _mm_set_epi32(S.y_max[j], S.y_max[j], S.y_max[j], S.y_max[j]);

				auto first_comparison = _mm_cmplt_epi32(setR_min_x, setS_max_x);
				auto second_comparison = _mm_cmpgt_epi32(setR_max_x, setS_min_x);
				auto third_comparison = _mm_cmplt_epi32(setR_min_y, setS_max_y);
				auto fourth_comparison = _mm_cmpgt_epi32(setR_max_y, setS_min_y);

               

				joins_count += _mm_extract_epi32(first_comparison, 3) * _mm_extract_epi32(second_comparison, 3) * _mm_extract_epi32(third_comparison, 3) * _mm_extract_epi32(fourth_comparison, 3);
				joins_count += _mm_extract_epi32(first_comparison, 2) * _mm_extract_epi32(second_comparison, 2) * _mm_extract_epi32(third_comparison, 2) * _mm_extract_epi32(fourth_comparison, 2);
				joins_count += _mm_extract_epi32(first_comparison, 1) * _mm_extract_epi32(second_comparison, 1) * _mm_extract_epi32(third_comparison, 1) * _mm_extract_epi32(fourth_comparison, 1);
				joins_count += _mm_extract_epi32(first_comparison, 0) * _mm_extract_epi32(second_comparison, 0) * _mm_extract_epi32(third_comparison, 0) * _mm_extract_epi32(fourth_comparison, 0);
		

            }


			//8 Rectangles of set R disintegrated into its smallest elements i.e. coordinates, which are 32 bit integers. Packed into an _m128i datatype. Each _m128i datatype can store 4 32-bit integers

			/* 
				__m128i setR_min_x = _mm_set_epi32(R.x_min[i], R.x_min[i + 1], R.x_min[i + 2], R.x_min[i + 3]);
				__m128i setR_min_y = _mm_set_epi32(R.y_min[i], R.y_min[i + 1], R.y_min[i + 2], R.y_min[i + 3]);
				__m128i	setR_max_x = _mm_set_epi32(R.x_max[i], R.x_max[i+ 1 ], R.x_max[i + 2], R.x_max[i + 3]);
				__m128i	setR_max_y = _mm_set_epi32(R.y_max[i], R.y_max[i + 1], R.y_max[i + 2], R.y_max[i + 3]);


				__m128i setS_min_x = _mm_set_epi32(S.x_min[j], S.x_min[j], S.x_min[j], S.x_min[j]);
				__m128i setS_min_y = _mm_set_epi32(S.y_min[j], S.y_min[j], S.y_min[j], S.y_min[j]);
				__m128i	setS_max_x = _mm_set_epi32(S.x_max[j], S.x_max[j], S.x_max[j], S.x_max[j]);
				__m128i	setS_max_y = _mm_set_epi32(S.y_max[j], S.y_max[j], S.y_max[j], S.y_max[j]);


				auto first_comparison = _mm_cmplt_epi32(setR_min_x, setS_max_x);
				auto second_comparison = _mm_cmpgt_epi32(setR_max_x, setS_min_x);
				auto third_comparison = _mm_cmplt_epi32(setR_min_y, setS_max_y);
				auto fourth_comparison = _mm_cmpgt_epi32(setR_max_y, setS_min_y);


				joins_count += _mm_extract_epi32(first_comparison, 3) * _mm_extract_epi32(second_comparison, 3) * _mm_extract_epi32(third_comparison, 3) * _mm_extract_epi32(fourth_comparison, 3);
				joins_count += _mm_extract_epi32(first_comparison, 2) * _mm_extract_epi32(second_comparison, 2) * _mm_extract_epi32(third_comparison, 2) * _mm_extract_epi32(fourth_comparison, 2);
				joins_count += _mm_extract_epi32(first_comparison, 1) * _mm_extract_epi32(second_comparison, 1) * _mm_extract_epi32(third_comparison, 1) * _mm_extract_epi32(fourth_comparison, 1);
				joins_count += _mm_extract_epi32(first_comparison, 0) * _mm_extract_epi32(second_comparison, 0) * _mm_extract_epi32(third_comparison, 0) * _mm_extract_epi32(fourth_comparison, 0);

				
				// 4 more now:
				 setR_min_x = _mm_set_epi32(R.x_min[i + 4], R.x_min[i + 5], R.x_min[i + 6], R.x_min[i + 7]);
				 setR_min_y = _mm_set_epi32(R.y_min[i + 4], R.y_min[i + 5], R.y_min[i + 6], R.y_min[i + 7]);
				 setR_max_x = _mm_set_epi32(R.x_max[i + 4], R.x_max[i + 5], R.x_max[i + 6], R.x_max[i + 7]);
				 setR_max_y = _mm_set_epi32(R.y_max[i + 4], R.y_max[i + 5], R.y_max[i + 6], R.y_max[i + 7]);


			//	 setS_min_x = _mm_set_epi32(S.x_min[j], S.x_min[j], S.x_min[j], S.x_min[j]);
			//	 setS_min_y = _mm_set_epi32(S.y_min[j], S.y_min[j], S.y_min[j], S.y_min[j]);
			 //    setS_max_x = _mm_set_epi32(S.x_max[j], S.x_max[j], S.x_max[j], S.x_max[j]);
			//	 setS_max_y = _mm_set_epi32(S.y_max[j], S.y_max[j], S.y_max[j], S.y_max[j]);


				 first_comparison = _mm_cmplt_epi32(setR_min_x, setS_max_x);
				 second_comparison = _mm_cmpgt_epi32(setR_max_x, setS_min_x);
				 third_comparison = _mm_cmplt_epi32(setR_min_y, setS_max_y);
				 fourth_comparison = _mm_cmpgt_epi32(setR_max_y, setS_min_y);


				joins_count += _mm_extract_epi32(first_comparison, 3) * _mm_extract_epi32(second_comparison, 3) * _mm_extract_epi32(third_comparison, 3) * _mm_extract_epi32(fourth_comparison, 3);
				joins_count += _mm_extract_epi32(first_comparison, 2) * _mm_extract_epi32(second_comparison, 2) * _mm_extract_epi32(third_comparison, 2) * _mm_extract_epi32(fourth_comparison, 2);
				joins_count += _mm_extract_epi32(first_comparison, 1) * _mm_extract_epi32(second_comparison, 1) * _mm_extract_epi32(third_comparison, 1) * _mm_extract_epi32(fourth_comparison, 1);
				joins_count += _mm_extract_epi32(first_comparison, 0) * _mm_extract_epi32(second_comparison, 0) * _mm_extract_epi32(third_comparison, 0) * _mm_extract_epi32(fourth_comparison, 0);


			*/

			
		}

	}
}
			

class RectangleSet {

public:

	//list of mbrsspa
	rectangle_list_t mbrs;

	//spatial join function that returns a set of rectangles that overlap with each other
	spatial_join_list_t spatial_join(rectangle_list_t R, rectangle_list_t S);

	//naive spatial join function
	spatial_join_list_t naive_spatial_join(rectangle_list_t R, rectangle_list_t S);

	spatial_join_list_t tiling_spatial_join(rectangle_list_t R, rectangle_list_t S);

	RectangleSet(rectangle_list_t const& mbrs_to_insert) {

		mbrs.resize(mbrs_to_insert.size());
		std::copy(mbrs_to_insert.cbegin(), mbrs_to_insert.cend(), mbrs.begin());

	}

	~RectangleSet() {

		std::cout << "object destroyed" << std::endl;
	}
};


spatial_join_list_t RectangleSet::naive_spatial_join(rectangle_list_t R, rectangle_list_t S) {

	//R x S
	spatial_join_list_t spatial_list;
	spatial_list.reserve(10000000);
	//outer loop iterating through R
	for (auto i = 0; i < R.size(); i++) {
		//inner loop iterating through S
		for (auto j = 0; j < S.size(); j++) {

			//condition to check if a particular MBR object in R overlaps with ANY objects in S
			if ((R[i].first.first < S[j].second.first)
				&& (S[j].first.first < R[i].second.first)
				&& (R[i].first.second < S[j].second.second)
				&& (S[j].first.second < R[i].second.second)
				) {

				//spatial_list.push_back({ R[i], S[j] });
				naive_naive_joins_count += 1;


			}



		}
	}


	return spatial_list;
}

//Function inspired from https://stackoverflow.com/questions/7560114/random-number-c-in-some-range


rectangle_list_t random_rectangle_generator() {

	rectangle_list_t random_rectangle_list;
	std::random_device rd; // obtain a random number from hardware
	std::mt19937 gen(rd()); // seed the generator
	std::uniform_int_distribution<> distr(1, 1000); // define the range

	//auto const length = 100000u;
	//random_rectangle_list.reserve(length);

	for (auto n = 0; n < 100000; n++) {

		int min_x = distr(gen);
		int min_y = distr(gen);
		int max_x, max_y, length, width;


		do {

			max_x = distr(gen) + min_x;
			max_y = distr(gen) + min_y;
			length = max_x - min_x;
			width = max_y - min_y;
		} while ((length >= 20) || (width >= 10));




		random_rectangle_list.push_back({ {min_x,min_y}, {max_x,max_y} });
	}

	return random_rectangle_list;

}


rectangle_list_t random_rectangle_generator_soa() {

	rectangle_list_t random_rectangle_list;

	std::random_device rd; // obtain a random number from hardware
	std::mt19937 gen(rd()); // seed the generator
	std::uniform_int_distribution<> distr(1, 1000); // define the range

	for (auto n = 0; n < 100000; n++) {

		int min_x = distr(gen);
		int min_y = distr(gen);
		int max_x, max_y, length, width;


		do {

			max_x = distr(gen) + min_x;
			max_y = distr(gen) + min_y;
			length = max_x - min_x;
			width = max_y - min_y;
		} while ((length >= 20) || (width >= 10));




		random_rectangle_list.push_back({ {min_x,min_y}, {max_x,max_y} });
	}

	return random_rectangle_list;

}


//cuda kernel

__global__
void cuda_join(int* dev_min_x1, int* dev_max_x2, int* dev_max_x1, int* dev_min_x2, int* dev_min_y1, int* dev_max_y2, 
				int* dev_max_y1, int* dev_min_y2, int n, int n_phase2, int* dev_block_reduction_array){

__shared__ int pract_a[1024];

	    int th_id = blockIdx.x * blockDim.x + threadIdx.x;

	pract_a[threadIdx.x] = 0;

	for(int i = 0; i < (n-7); i+=8) {
							

		for(int j = ( th_id * ceil(n/25000) ); j < ( (th_id + 1) * ceil(n/25000) ); j++) {
								
			for(int k = 0; k < 8; k++) {
				

       			 	pract_a[threadIdx.x] += ( (dev_min_x1[i + k] < dev_max_x2[j]) && (dev_max_x1[i + k] > dev_min_x2[j])
					&& (dev_min_y1[i + k] < dev_max_y2[j]) && (dev_max_y1[i + k] > dev_min_y2[j]) ) ;
						
     			

			}
	
    	}
	}

__syncthreads();
//phase 2:

	while( n_phase2 > 0 )
	{
		n_phase2 = ceil(n_phase2 / 2);
		if( threadIdx.x < n_phase2 )
		{
			pract_a[ threadIdx.x ] += pract_a[ threadIdx.x + n_phase2 ];

		}
	}

    __syncthreads();
    if( threadIdx.x == 0 )
	{
	
                dev_block_reduction_array[blockIdx.x] = pract_a[0];

          //  printf("%d on block %d: \n", dev_block_reduction_array[blockIdx.x], blockIdx.x);

	}

}

int main() {

	auto soa_setR = random_rectangle_generator_soa();
	auto soa_setS = random_rectangle_generator_soa();


	RectangleSet_Coordinates soa_R(soa_setR);
	RectangleSet_Coordinates soa_S(soa_setS);

	RectangleSet R(soa_setR);
	RectangleSet S(soa_setS);
	
	std::cout << "Rectangles generated. Starting Spatial Join function Benchmarking now------------------" << std::endl;
/*
	//**********NAIVE JOIN FUNCTION BENCHMARKING**********************
	//std::cout << std::endl;

	//spatial_join_list_t OLD_JOIN;

	//CLOCK START
	auto naive_start = std::chrono::high_resolution_clock::now();

	//CALL THE FUNCTION
	NAIVE_SOA_SPATIAL_JOIN(soa_R, soa_S);

	//CLOCK END
	auto naive_end = std::chrono::high_resolution_clock::now();

	auto naive_elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>
		(naive_end - naive_start);

	std::cout << "Time in microseconds (naive SoA approach): " << naive_elapsed_time.count() << "us" << std::endl;


	std::cout << "Number of joins (naive SoA approach): " << naive_joins_count << std::endl << std::endl;

*/

	

	//**********TILING JOIN FUNCTION BENCHMARKING**********************
	std::cout << std::endl;


	//CLOCK START
	auto mid_start = std::chrono::high_resolution_clock::now();

	//CALL THE FUNCTION
	TILING_SOA_SPATIAL_JOIN(soa_R, soa_S);

	//CLOCK END
	auto mid_end = std::chrono::high_resolution_clock::now();

	auto mid_elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>
		(mid_end - mid_start);

	std::cout << "Time in microseconds (Tiling approach): " << mid_elapsed_time.count() << "us" << std::endl;


	std::cout << "Number of joins (Tiling approach): " << tiling_joins_count << std::endl << std::endl;




/*
	
	//**********NAIVE_NAIVE JOIN FUNCTION BENCHMARKING**********************
	std::cout << std::endl;

	spatial_join_list_t OLD_JOIN;

	//CLOCK START
	auto naive_naive_start = std::chrono::high_resolution_clock::now();

	//CALL THE FUNCTION
	OLD_JOIN = R.naive_spatial_join(R.mbrs, S.mbrs);

	//CLOCK END
	auto naive_naive_end = std::chrono::high_resolution_clock::now();

	auto naive_naive_elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>
		(naive_naive_end - naive_naive_start);

	std::cout << "Time in microseconds (naive approach): " << naive_naive_elapsed_time.count() << "us" << std::endl;


	std::cout << "Number of joins (naive approach): " << naive_naive_joins_count << std::endl << std::endl;

*/
	

	// **********OPTIMIZED SIMD JOIN FUNCTION BENCHMARKING**************
	std::cout << std::endl;

	//spatial_join_list_t JOIN;
	//CLOCK START
	auto start = std::chrono::high_resolution_clock::now();

	//CALL THE FUNCTION
	SIMD_SPATIAL_JOIN(soa_R, soa_S);

	//CLOCK END
	auto end = std::chrono::high_resolution_clock::now();

	auto elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>
		(end - start);
	std::cout << "Time in microseconds (optimized SIMD approach): " << elapsed_time.count() << "us" << std::endl;

	std::cout << "Number of joins (optimized SIMD approach): " << joins_count << std::endl;



//CUDA CODE
int n = 100000; //size of array

int host_block_reduction_array[25];

for(int i = 0; i < 25; i++){


    host_block_reduction_array[i] = 0;
}

int* dev_block_reduction_array;



cudaMalloc((void**)&dev_block_reduction_array, sizeof(int)*25);
    cudaMemcpy(dev_block_reduction_array, host_block_reduction_array, sizeof(int)*25, cudaMemcpyHostToDevice);

int* dev_min_x1;
int* dev_max_x2;

int* dev_max_x1;
int* dev_min_x2;

int* dev_min_y1;
int* dev_max_y2;

int* dev_max_y1;
int* dev_min_y2;


cudaMalloc((void**)&dev_min_x1, sizeof(int)*n );
cudaMemcpy(dev_min_x1, soa_R.x_min, sizeof(int)*n, cudaMemcpyHostToDevice);
cudaMalloc((void**)&dev_max_x2, sizeof(int)*n );
cudaMemcpy(dev_max_x2, soa_S.x_max, sizeof(int)*n, cudaMemcpyHostToDevice);

cudaMalloc((void**)&dev_max_x1, sizeof(int)*n );
cudaMemcpy(dev_max_x1, soa_R.x_max, sizeof(int)*n, cudaMemcpyHostToDevice);
cudaMalloc((void**)&dev_min_x2, sizeof(int)*n );
cudaMemcpy(dev_min_x2, soa_S.x_min, sizeof(int)*n, cudaMemcpyHostToDevice);

cudaMalloc((void**)&dev_min_y1, sizeof(int)*n );
cudaMemcpy(dev_min_y1, soa_R.y_min, sizeof(int)*n, cudaMemcpyHostToDevice);
cudaMalloc((void**)&dev_max_y2, sizeof(int)*n );
cudaMemcpy(dev_max_y2, soa_S.y_max, sizeof(int)*n, cudaMemcpyHostToDevice);

cudaMalloc((void**)&dev_max_y1, sizeof(int)*n );
cudaMemcpy(dev_max_y1, soa_R.y_max, sizeof(int)*n, cudaMemcpyHostToDevice);
cudaMalloc((void**)&dev_min_y2, sizeof(int)*n );
cudaMemcpy(dev_min_y2, soa_S.y_min, sizeof(int)*n, cudaMemcpyHostToDevice);

//CLOCK START
	auto cuda_start = std::chrono::high_resolution_clock::now();

	//CALL THE FUNCTION
cuda_join<<<25,1000>>> (dev_min_x1, dev_max_x2, dev_max_x1, dev_min_x2, dev_min_y1, dev_max_y2, dev_max_y1, 
                        dev_min_y2, n, 1024, dev_block_reduction_array);

cudaDeviceSynchronize();
//CLOCK END
	auto cuda_end = std::chrono::high_resolution_clock::now();

	auto cuda_elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>
		(cuda_end - cuda_start);
	std::cout << "Time in microseconds (optimized CUDA approach): " << cuda_elapsed_time.count() << "us" << std::endl;
cudaMemcpy(host_block_reduction_array, dev_block_reduction_array, sizeof(int)*25, cudaMemcpyDeviceToHost);

int total_block_reduction_counter = 0;
for(int x = 0; x < 25; x++){
   total_block_reduction_counter += host_block_reduction_array[x];
}
	std::cout << "Number of joins (optimized CUDA approach): " << (total_block_reduction_counter) <<std::endl;

}


