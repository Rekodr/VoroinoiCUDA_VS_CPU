#include <vector>
#include <stdio.h>
#include <iostream>
#include <cuda.h>
#include<cuda_runtime.h>
#include<device_launch_parameters.h>


#define BLOCKSIZE_x 32
#define BLOCKSIZE_y 32

using namespace std;


__device__ double eucludianDist(int Ax, int Ay, int Bx, int By) {
	double d = sqrt(pow((Ax - Bx), 2) +
									pow((Ay - By), 2)
							);
	return d;
}

__global__ void voronoiKernel(int* vec, int cols, int rows, int pitch, int* Px, int* Py, int numSeeds) {
	  int x = blockIdx.x * blockDim.x + threadIdx.x;
		int y = blockIdx.y * blockDim.y + threadIdx.y;

		int* row_a = (int*)((char*)vec + y * pitch) + x;

		__syncthreads();
    if((x < cols) && (y < rows)){
      double d = eucludianDist(x, y, Px[0], Py[0]);
			__syncthreads();
			//atomicExch(row_a, 0);
			*row_a = 0;
      for (int i = 0; i < numSeeds ; i++) {
				double temp = eucludianDist(x, y, Px[i], Py[i]);
				__syncthreads();
        if (temp < d) {
          d = temp;
					*row_a = i;
        }
      }
    }
}

void voronoi_cuda(int* h_vec, std::vector<std::pair<unsigned int, unsigned int>> &P, int cols = 0, int rows = 0) {

  cudaEvent_t start, end;
	cudaEventCreate(&start);
	cudaEventCreate(&end);

	int* vec_dev;
	int* Px_dev;
	int* Py_dev;
	size_t pitch;
	int nSeeds = P.size();
  int ln = nSeeds * sizeof(int) ;

	int* Px = (int*) malloc(ln);
	int* Py = (int*) malloc(ln);

	int i = 0;
	// could not figure an easy way to copy vector of pair to the gpu
	for(auto& p : P){
		Px[i] = p.first;
		Py[i] = p.second;
		i++;
	}

 std::cout << "computing veronoi->Parallel..." << std::endl;
	cudaMallocPitch((void**)&vec_dev, &pitch, sizeof(int)*cols, rows);

	cudaEventRecord(start); //start counting event on GPU

	cudaMalloc((void**)&Px_dev, ln);
	cudaMalloc((void**)&Py_dev, ln);
	cudaMemcpy(Px_dev, Px, ln, cudaMemcpyHostToDevice);
	cudaMemcpy(Py_dev, Py, ln, cudaMemcpyHostToDevice);

	dim3 blockDim(BLOCKSIZE_y, BLOCKSIZE_x);
	dim3 gridDim(ceil((float)cols /blockDim.x) , ceil((float)rows /blockDim.y));


	voronoiKernel<<<gridDim, blockDim>>>(vec_dev, cols, rows, pitch, Px_dev, Py_dev, nSeeds);

	cudaMemcpy2D(h_vec, sizeof(int)*cols, vec_dev, pitch, sizeof(int)*cols, rows, cudaMemcpyDeviceToHost);
	cudaDeviceSynchronize();

	cudaEventRecord(end);
	cudaEventSynchronize(end);
	std::cout << "done..." << std::endl;
  float ms{0};
	cudaEventElapsedTime(&ms, start, end);
	std::cout << "computation time(ms): " << ms << std::endl;
	cudaFree(vec_dev);
	cudaFree(Px_dev);
	cudaFree(Py_dev);
	free(Px);
	free(Py);

}
