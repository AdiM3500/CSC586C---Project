#include <iostream>
#include <math.h>


__global__
void abc(int *count, int n) {
   __shared__ int a[256];

   int a_reg_count;
   a_reg_count = 0;
	//a = 0;
   //a+=1;
    int th_id = blockIdx.x * blockDim.x + threadIdx.x;

	a_reg_count+=1;
	//if(index < 256){
	a[th_id] = a_reg_count;
	//}
	//int reg_count = 0;
//	reg_count+=1;
	//*count+=1;
	//*count += a[th_id];

	//    __syncthreads();

	while( n > 0 )
	{
		n = (n / 2);
		if( th_id < n )
		{
			a[ th_id ] += a[ th_id + n ];
		}
	}
	// Observe that there is no need for synchronisation
	// here because n<32 and all threads in a warp are
	// "step-locked"; i.e., they must walk through the
	// instructions one-by-one together at the same time 
	
	// We need to ensure that this is executed by a thread
	// on the first warp; otherwise, a thread on a different
	// warp could perform this write before the value is ready. 
	if( th_id == 0 )
	{
		*count = a[ 0 ];
	}

	//atomicAdd(count, 1);
    
}

__global__
void abcd(int *count, int n, int n_phase_2) {
   __shared__ int a[1024];

    int th_id = blockIdx.x * blockDim.x + threadIdx.x;

int h[1000];
	int a_reg_count = 1;
//phase 1: n/t = 100

/* 
a[th_id] = 0;
for(int x = 0; x < 100; x++){

	a[th_id] += 1;
	
	
}
if(th_id == 999){
			printf("%d\n", a[th_id]);

			}

			*/

	for( int i = ( th_id * (100) ); i < ( (th_id + 1) * (100) ); i++){

		//a[i] = a_reg_count;
		a[th_id] +=1;
		if(th_id == 999)
			printf("%d, %d\n", a[th_id], i);

	}


__syncthreads();

//phase 2:


	while( n_phase_2 > 0 )
	{
		n_phase_2 = (n_phase_2 / 2);
		if( th_id < n_phase_2 )
		{
			a[ th_id ] += a[ th_id + n_phase_2 ];
				//		printf("%d\n\n",n_phase_2);

		}
	}
	// Observe that there is no need for synchronisation
	// here because n<32 and all threads in a warp are
	// "step-locked"; i.e., they must walk through the
	// instructions one-by-one together at the same time 
	
	// We need to ensure that this is executed by a thread
	// on the first warp; otherwise, a thread on a different
	// warp could perform this write before the value is ready. 
	if( th_id == 0 )
	{
		*count = a[ 0 ];
	}

	//atomicAdd(count, 1);

//	*count = (th_id) * (n/1000);
    
}

int main() {
	int h_count[1];
	int *d_count;

	auto n = 100000;
	auto n_phase_2 = 1024;
	auto d_n = 256;
	h_count[0] = 0;
	cudaMalloc((void**)&d_count, sizeof(int));
	cudaMemcpy(d_count, h_count, sizeof(int), cudaMemcpyHostToDevice);

	abcd << <1, 1000 >> > (d_count, n, n_phase_2);


	cudaMemcpy(h_count, d_count, sizeof(int), cudaMemcpyDeviceToHost);
   // cudaDeviceSynchronize();



	std::cout << h_count[0] <<std::endl;
}