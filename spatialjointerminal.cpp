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


//global variable to keep track of the number of joins that occured
long long int joins_count = 0;
long long int naive_joins_count = 0;
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




	RectangleSet(rectangle_list_t const& mbrs_to_insert) {

		mbrs.resize(mbrs_to_insert.size());
		std::copy(mbrs_to_insert.cbegin(), mbrs_to_insert.cend(), mbrs.begin());

		/* 	for (auto i = 0; i < mbrs_to_insert.size(); i++) {

				mbrs.push_back(mbrs_to_insert[i]);
				//mbrs[i] = mbrs_to_insert[i]; doesnt work
			}*/


	}

	~RectangleSet() {

		std::cout << "object destroyed"<<std::endl;
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
			if ((R[i].first.first <= S[j].second.first)
				&& (S[j].first.first <= R[i].second.first)
				&& (R[i].first.second <= S[j].second.second)
				&& (S[j].first.second <= R[i].second.second)
				) {

				spatial_list.push_back({ R[i], S[j] });
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

	//outer loop iterating through R
	for (auto i = 0; i < R.size() - 19; i+=20) {

		//inner loop iterating through S (S.sizr() - 3 to not go out of bounds) via loop unrolling
		for (auto j = 0; j < S.size(); j++) {

			//condition to check if a particular MBR object in R overlaps with ANY objects in S
			if ((R[i].first.first <= S[j].second.first)
				&& (S[j].first.first <= R[i].second.first)
				&& (R[i].first.second <= S[j].second.second)
				&& (S[j].first.second <= R[i].second.second)
				) {

				spatial_list.push_back({ R[i], S[j] });
				joins_count += 1;


			}

			if ((R[i+1].first.first <= S[j].second.first)
				&& (S[j].first.first <= R[i+1].second.first)
				&& (R[i+1].first.second <= S[j].second.second)
				&& (S[j].first.second <= R[i+1].second.second)
				) {

				spatial_list.push_back({ R[i+1], S[j] });
				joins_count += 1;


			}

			if ((R[i+2].first.first <= S[j].second.first)
				&& (S[j].first.first <= R[i+2].second.first)
				&& (R[i+2].first.second <= S[j].second.second)
				&& (S[j].first.second <= R[i+2].second.second)
				) {

				spatial_list.push_back({ R[i+2], S[j] });
				joins_count += 1;


			}

			if ((R[i+3].first.first <= S[j].second.first)
				&& (S[j].first.first <= R[i+3].second.first)
				&& (R[i+3].first.second <= S[j].second.second)
				&& (S[j].first.second <= R[i+3].second.second)
				) {

				spatial_list.push_back({ R[i+3], S[j] });
				joins_count += 1;

			}

			if ((R[i + 4].first.first <= S[j].second.first)
				&& (S[j].first.first <= R[i + 4].second.first)
				&& (R[i + 4].first.second <= S[j].second.second)
				&& (S[j].first.second <= R[i + 4].second.second)
				) {

				spatial_list.push_back({ R[i + 4], S[j] });
				joins_count += 1;

			}

			if ((R[i + 5].first.first <= S[j].second.first)
				&& (S[j].first.first <= R[i + 5].second.first)
				&& (R[i + 5].first.second <= S[j].second.second)
				&& (S[j].first.second <= R[i + 5].second.second)
				) {

				spatial_list.push_back({ R[i + 5], S[j] });
				joins_count += 1;

			}

			if ((R[i + 6].first.first <= S[j].second.first)
				&& (S[j].first.first <= R[i + 6].second.first)
				&& (R[i + 6].first.second <= S[j].second.second)
				&& (S[j].first.second <= R[i + 6].second.second)
				) {

				spatial_list.push_back({ R[i + 6], S[j] });
				joins_count += 1;

			}

			if ((R[i + 7].first.first <= S[j].second.first)
				&& (S[j].first.first <= R[i + 7].second.first)
				&& (R[i + 7].first.second <= S[j].second.second)
				&& (S[j].first.second <= R[i + 7].second.second)
				) {

				spatial_list.push_back({ R[i + 7], S[j] });
				joins_count += 1;

			}

			if ((R[i + 8].first.first <= S[j].second.first)
				&& (S[j].first.first <= R[i + 8].second.first)
				&& (R[i + 8].first.second <= S[j].second.second)
				&& (S[j].first.second <= R[i + 8].second.second)
				) {

				spatial_list.push_back({ R[i + 8], S[j] });
				joins_count += 1;

			}

			if ((R[i + 9].first.first <= S[j].second.first)
				&& (S[j].first.first <= R[i + 9].second.first)
				&& (R[i + 9].first.second <= S[j].second.second)
				&& (S[j].first.second <= R[i + 9].second.second)
				) {

				spatial_list.push_back({ R[i + 9], S[j] });
				joins_count += 1;

			}

			if ((R[i + 10].first.first <= S[j].second.first)
				&& (S[j].first.first <= R[i + 10].second.first)
				&& (R[i + 10].first.second <= S[j].second.second)
				&& (S[j].first.second <= R[i + 10].second.second)
				) {

				spatial_list.push_back({ R[i + 10], S[j] });
				joins_count += 1;

			}

			if ((R[i + 11].first.first <= S[j].second.first)
				&& (S[j].first.first <= R[i + 11].second.first)
				&& (R[i + 11].first.second <= S[j].second.second)
				&& (S[j].first.second <= R[i + 11].second.second)
				) {

				spatial_list.push_back({ R[i + 11], S[j] });
				joins_count += 1;

			}

			if ((R[i + 12].first.first <= S[j].second.first)
				&& (S[j].first.first <= R[i + 12].second.first)
				&& (R[i + 12].first.second <= S[j].second.second)
				&& (S[j].first.second <= R[i + 12].second.second)
				) {

				spatial_list.push_back({ R[i + 12], S[j] });
				joins_count += 1;

			}

			if ((R[i + 13].first.first <= S[j].second.first)
				&& (S[j].first.first <= R[i + 13].second.first)
				&& (R[i + 13].first.second <= S[j].second.second)
				&& (S[j].first.second <= R[i + 13].second.second)
				) {

				spatial_list.push_back({ R[i + 13], S[j] });
				joins_count += 1;

			}

			if ((R[i + 14].first.first <= S[j].second.first)
				&& (S[j].first.first <= R[i + 14].second.first)
				&& (R[i + 14].first.second <= S[j].second.second)
				&& (S[j].first.second <= R[i + 14].second.second)
				) {

				spatial_list.push_back({ R[i + 14], S[j] });
				joins_count += 1;

			}

			if ((R[i + 15].first.first <= S[j].second.first)
				&& (S[j].first.first <= R[i + 15].second.first)
				&& (R[i + 15].first.second <= S[j].second.second)
				&& (S[j].first.second <= R[i + 15].second.second)
				) {

				spatial_list.push_back({ R[i + 15], S[j] });
				joins_count += 1;

			}

			if ((R[i + 16].first.first <= S[j].second.first)
				&& (S[j].first.first <= R[i + 16].second.first)
				&& (R[i + 16].first.second <= S[j].second.second)
				&& (S[j].first.second <= R[i + 16].second.second)
				) {

				spatial_list.push_back({ R[i + 16], S[j] });
				joins_count += 1;

			}

			if ((R[i + 17].first.first <= S[j].second.first)
				&& (S[j].first.first <= R[i + 17].second.first)
				&& (R[i + 17].first.second <= S[j].second.second)
				&& (S[j].first.second <= R[i + 17].second.second)
				) {

				spatial_list.push_back({ R[i + 17], S[j] });
				joins_count += 1;

			}

			if ((R[i + 18].first.first <= S[j].second.first)
				&& (S[j].first.first <= R[i + 18].second.first)
				&& (R[i + 18].first.second <= S[j].second.second)
				&& (S[j].first.second <= R[i + 18].second.second)
				) {

				spatial_list.push_back({ R[i + 18], S[j] });
				joins_count += 1;

			}

			if ((R[i + 19].first.first <= S[j].second.first)
				&& (S[j].first.first <= R[i + 19].second.first)
				&& (R[i + 19].first.second <= S[j].second.second)
				&& (S[j].first.second <= R[i + 19].second.second)
				) {

				spatial_list.push_back({ R[i + 19], S[j] });
				joins_count += 1;

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


	std::cout << "Number of joins (naive approach): " << naive_joins_count << std::endl <<std::endl;

	

	/* FOR DISPLAYING FIRST 5 JOINS BETWEEN R AND S
	for (auto i = 0; i < 5 ; i++) {


		std::cout << "[" << "(" << JOIN[i].first.first.first << "," << JOIN[i].first.first.second << "), (" << JOIN[i].first.second.first << "," << JOIN[i].first.second.second << ") JOINS "
			<< "(" << JOIN[i].second.first.first << "," << JOIN[i].second.first.second << "), (" << JOIN[i].second.second.first << "," << JOIN[i].second.second.second << ")" << "]" << std::endl;



	}

	*/
	

}



