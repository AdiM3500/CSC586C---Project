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
#include <omp.h>


//global variable to keep track of the number of joins that occured
long long int joins_count = 0;
long long int naive_joins_count = 0;
long long int tiling_joins_count = 0;
using coordinate_t = std::pair <int, int>;
using pair_of_coordinates_t = std::pair<coordinate_t, coordinate_t>;
using rectangle_list_t = std::vector<pair_of_coordinates_t>;
using spatial_join_list_t = std::vector<std::pair < pair_of_coordinates_t, pair_of_coordinates_t> >;

class RectangleSet {

public:

	//list of mbrs
	rectangle_list_t mbrs;

	//spatial join function that returns a set of rectangles that overlap with each other
	spatial_join_list_t spatial_join(rectangle_list_t R, rectangle_list_t S);

	//naive spatial join function
	spatial_join_list_t naive_spatial_join(rectangle_list_t R, rectangle_list_t S);

	spatial_join_list_t tiling_spatial_join(rectangle_list_t R, rectangle_list_t S);


	RectangleSet(rectangle_list_t const& mbrs_to_insert) {

		mbrs.resize(mbrs_to_insert.size());
		std::copy(mbrs_to_insert.cbegin(), mbrs_to_insert.cend(), mbrs.begin());

		/* 	for (auto i = 0; i < mbrs_to_insert.size(); i++) {

				mbrs.push_back(mbrs_to_insert[i]);
				//mbrs[i] = mbrs_to_insert[i]; doesnt work
			}*/


	}

	~RectangleSet() {

		std::cout << "object destroyed" << std::endl;
	}
};


spatial_join_list_t RectangleSet::tiling_spatial_join(rectangle_list_t R, rectangle_list_t S) {

	//R x S
	spatial_join_list_t tiling_spatial_list;
	tiling_spatial_list.reserve(10000000);

	//outer loop iterating through R
	for (auto i = 0; i < R.size() - 49; i += 50) {

		//inner loop iterating through S (S.sizr() - 3 to not go out of bounds) via loop unrolling
		for (auto j = 0; j < S.size(); j++) {

			for (int k = 0; k < 50; k++) {



				if ((R[i + k].first.first < S[j].second.first)
					&& (S[j].first.first < R[i + k].second.first)
					&& (R[i + k].first.second < S[j].second.second)
					&& (S[j].first.second < R[i + k].second.second)
					) {

					//tiling_spatial_list.push_back({ R[i + k], S[j] });
					tiling_joins_count += 1;

				}

			}



			//condition to check if a particular MBR object in R overlaps with ANY objects in S



		}
	}


	return tiling_spatial_list;
}





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
				naive_joins_count += 1;


			}



		}
	}


	return spatial_list;
}


spatial_join_list_t RectangleSet::spatial_join(rectangle_list_t R, rectangle_list_t S) {

	//R x S
	spatial_join_list_t spatial_list;
	spatial_list.reserve(10000000);
	int  num_threads = 6;
	omp_set_num_threads(6);

	//outer loop iterating through R but i increments by 4

	for (auto i = 0; i < R.size() - 3; i += 4) {

#pragma omp parallel for reduction (+:joins_count) 
		for (auto j = 0; j < S.size() ; j++) {


				//8 Rectangles of set R disintegrated into its smallest elements i.e. coordinates, which are 32 bit integers. Packed into an _m128i datatype. Each _m128i datatype can store 4 32-bit integers
				__m128i setR_min_x = _mm_set_epi32(R[i].first.first, R[i + 1].first.first, R[i + 2].first.first, R[i + 3].first.first);
				__m128i setR_min_y = _mm_set_epi32(R[i].first.second, R[i + 1].first.second, R[i + 2].first.second, R[i + 3].first.second);
				__m128i	setR_max_x = _mm_set_epi32(R[i].second.first, R[i + 1].second.first, R[i + 2].second.first, R[i + 3].second.first);
				__m128i	setR_max_y = _mm_set_epi32(R[i].second.second, R[i + 1].second.second, R[i + 2].second.second, R[i + 3].second.second);


				__m128i setS_min_x = _mm_set_epi32(S[j].first.first, S[j].first.first, S[j].first.first, S[j].first.first);
				__m128i setS_min_y = _mm_set_epi32(S[j].first.second, S[j].first.second, S[j].first.second, S[j].first.second);
				__m128i	setS_max_x = _mm_set_epi32(S[j].second.first, S[j].second.first, S[j].second.first, S[j].second.first);
				__m128i	setS_max_y = _mm_set_epi32(S[j].second.second, S[j].second.second, S[j].second.second, S[j].second.second);


				auto first_comparison = _mm_cmplt_epi32(setR_min_x, setS_max_x);
				auto second_comparison = _mm_cmpgt_epi32(setR_max_x, setS_min_x);
				auto third_comparison = _mm_cmplt_epi32(setR_min_y, setS_max_y);
				auto fourth_comparison = _mm_cmpgt_epi32(setR_max_y, setS_min_y);



				//overlap_satisfied_index[0] = _mm_extract_epi32(first_comparison, 3) * _mm_extract_epi32(second_comparison, 3) * _mm_extract_epi32(third_comparison, 3) * _mm_extract_epi32(fourth_comparison, 3);
				//overlap_satisfied_index[1] = _mm_extract_epi32(first_comparison, 2) * _mm_extract_epi32(second_comparison, 2) * _mm_extract_epi32(third_comparison, 2) * _mm_extract_epi32(fourth_comparison, 2);
				//overlap_satisfied_index[2] = _mm_extract_epi32(first_comparison, 1) * _mm_extract_epi32(second_comparison, 1) * _mm_extract_epi32(third_comparison, 1) * _mm_extract_epi32(fourth_comparison, 1);
				//overlap_satisfied_index[3] = _mm_extract_epi32(first_comparison, 0) * _mm_extract_epi32(second_comparison, 0) * _mm_extract_epi32(third_comparison, 0) * _mm_extract_epi32(fourth_comparison, 0);

				joins_count+= _mm_extract_epi32(first_comparison, 3) * _mm_extract_epi32(second_comparison, 3) * _mm_extract_epi32(third_comparison, 3) * _mm_extract_epi32(fourth_comparison, 3);
				joins_count+= _mm_extract_epi32(first_comparison, 2) * _mm_extract_epi32(second_comparison, 2) * _mm_extract_epi32(third_comparison, 2) * _mm_extract_epi32(fourth_comparison, 2);
				joins_count+= _mm_extract_epi32(first_comparison, 1) * _mm_extract_epi32(second_comparison, 1) * _mm_extract_epi32(third_comparison, 1) * _mm_extract_epi32(fourth_comparison, 1);
				joins_count+= _mm_extract_epi32(first_comparison, 0) * _mm_extract_epi32(second_comparison, 0) * _mm_extract_epi32(third_comparison, 0) * _mm_extract_epi32(fourth_comparison, 0);
				
			 /* 	for (auto ii = 0; ii < 4; ii++) {

					if (overlap_satisfied_index[ii] == 1) {

						//spatial_list.push_back({ R[i + ii], S[j] });
						joins_count += 1;
					}
				}

				*/
				



				/*DO THE ABOVE AGAIN FOR 4 MORE ITERATIONS*/


			

				//NEXT 8 ITERATIONS. TOTAL 16 ITERATIONS
				
				//next 4 iterations:

				




		//		__mmask8 second_comparison = _mm_cmpge_epi32_mask(setR_max_x, setS_min_x);
		//		__mmask8 third_comparison = _mm_cmple_epi32_mask(setR_min_y, setS_max_y);
		//		__mmask8 fourth_comparison = _mm_cmpge_epi32_mask(setR_max_y, setS_min_y);


			/*	if ((R[i + k].first.first < S[j].second.first)
					&& (S[j].first.first < R[i + k].second.first)
					&& (R[i + k].first.second < S[j].second.second)
					&& (S[j].first.second < R[i + k].second.second)
					) {

					spatial_list.push_back({ R[i + k], S[j] });
					joins_count += 1;

				}
				*/


			



			//condition to check if a particular MBR object in R overlaps with ANY objects in S



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




int main() {


	auto setR = random_rectangle_generator();
	auto setS = random_rectangle_generator();

	/* FOR DISPLAYING PURPOSES TO SEE THE RECTANGLES IN BOTH SETS
	std::cout << "set R\n";
	for (int n = 0; n < 10; ++n) {




		std::cout << setR[n].first.first << ' '; // generate numbers

		std::cout << setR[n].first.second << "\t"; //generate numbers higher than f

		std::cout << setR[n].second.first << ' ';



		std::cout << setR[n].second.second << std::endl;



	}


	std::cout << "set S\n";
	for (int n = 0; n < 10; ++n) {




		std::cout << setS[n].first.first << ' '; // generate numbers

		std::cout << setS[n].first.second << "\t"; //generate numbers higher than f

		std::cout << setS[n].second.first << ' ';

		std::cout << setS[n].second.second << std::endl;


	}

	*/




	RectangleSet R(setR);
	RectangleSet S(setS);



	// **********OPTIMIZED JOIN FUNCTION BENCHMARKING**************
	std::cout << std::endl;

	spatial_join_list_t JOIN;

	//CLOCK START
	auto start = std::chrono::high_resolution_clock::now();

	//CALL THE FUNCTION
	JOIN = R.spatial_join(R.mbrs, S.mbrs);

	//CLOCK END
	auto end = std::chrono::high_resolution_clock::now();

	auto elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>
		(end - start);
	std::cout << "Time in microseconds (optimized approach): " << elapsed_time.count() << "us" << std::endl;

	std::cout << "Number of joins (optimized approach): " << joins_count << std::endl;


	//**********TILING JOIN FUNCTION BENCHMARKING**********************
	std::cout << std::endl;

	spatial_join_list_t MID_JOIN;

	//CLOCK START
	auto mid_start = std::chrono::high_resolution_clock::now();

	//CALL THE FUNCTION
	MID_JOIN = R.tiling_spatial_join(R.mbrs, S.mbrs);

	//CLOCK END
	auto mid_end = std::chrono::high_resolution_clock::now();

	auto mid_elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>
		(mid_end - mid_start);

	std::cout << "Time in microseconds (Tiling approach): " << mid_elapsed_time.count() << "us" << std::endl;


	std::cout << "Number of joins (Tiling approach): " << tiling_joins_count << std::endl << std::endl;



	//**********NAIVE JOIN FUNCTION BENCHMARKING**********************
	std::cout << std::endl;

	spatial_join_list_t OLD_JOIN;

	//CLOCK START
	auto naive_start = std::chrono::high_resolution_clock::now();

	//CALL THE FUNCTION
	OLD_JOIN = R.naive_spatial_join(R.mbrs, S.mbrs);

	//CLOCK END
	auto naive_end = std::chrono::high_resolution_clock::now();

	auto naive_elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>
		(naive_end - naive_start);

	std::cout << "Time in microseconds (naive approach): " << naive_elapsed_time.count() << "us" << std::endl;


	std::cout << "Number of joins (naive approach): " << naive_joins_count << std::endl << std::endl;





	/* FOR DISPLAYING FIRST 5 JOINS BETWEEN R AND S
	for (auto i = 0; i < 5 ; i++) {


		std::cout << "[" << "(" << JOIN[i].first.first.first << "," << JOIN[i].first.first.second << "), (" << JOIN[i].first.second.first << "," << JOIN[i].first.second.second << ") JOINS "
			<< "(" << JOIN[i].second.first.first << "," << JOIN[i].second.first.second << "), (" << JOIN[i].second.second.first << "," << JOIN[i].second.second.second << ")" << "]" << std::endl;



	}

	*/


}


