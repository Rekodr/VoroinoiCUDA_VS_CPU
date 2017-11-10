#include <cstdio>
#include <iostream>
#include <vector>
#include <utility>
#include <cmath>
#include "time.h"
#include <sys/time.h>
#include "voronoi.h"
#include "createBMP.h"

#define seqFile "voronoi_Seq.bmp"
#define parFile "voronoi_Par.bmp"

using namespace std;

extern void voronoi_cuda(int* h_vec, std::vector<std::pair<unsigned int, unsigned int>> &P, int cols = 0, int rows = 0);

int main(int argc, char* argv[])
{
	srand(time(NULL));

	if (argc < 4) {
		cerr << "use: program_name  Xdim  Ydim  numSeeds" << endl;
		exit(1);
	}


	int cols = atoi(argv[1]);
	int rows = atoi(argv[2]);
	int numSeeds = atoi(argv[3]);

	int* matrix = new int[rows * cols];
	vector<pair<unsigned int, unsigned int>> P = genSeeds(numSeeds, cols, rows);
	//vector<pair<unsigned int, unsigned int>> P { {7,7}, {22,7}, {7,22}, {22,22} };

	voronoi(matrix, P, cols, rows);
	createBMP(matrix, P, cols, rows, P.size(), seqFile);

	voronoi_cuda(matrix, P, cols, rows);
	createBMP(matrix, P, cols, rows, P.size(), parFile);


	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			printf("%d ", matrix[i * rows + j]);
		}
		printf("\n");
	}

	delete matrix;

	return 0;
}
