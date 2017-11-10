#include "voronoi.h"
#include <cmath>
#include <utility>
#include <cstdlib>
#include <sys/time.h>
#include <iostream>


using namespace std;
// d = sqrt( Ax - Bx)^2 + (Ay - By)^2)
double eucludianDist(pair<int, int> a, pair< int, int> b) {

	double d = sqrt(pow((a.first - b.first), 2) +
		pow((a.second - b.second), 2)
	);
	return d;
}


void voronoi(int* vec, vector<pair<unsigned int, unsigned int>> &P, int xDim, int yDim) {
	timeval start, end;
	cout << "computing veronoi->Seq ..." << endl;
	gettimeofday(&start, NULL);
	for (int y = 0; y < yDim; y++) {
		for (int x = 0; x < xDim; x++) {
			auto d = eucludianDist(make_pair(x, y), P.at(0));
			int pos = 0;
			vec[y * yDim + x] = pos;
			for (auto &p : P) {
				auto temp = eucludianDist(make_pair(x, y), p);
				if (temp < d) {
					d = temp;
					vec[y * yDim + x] = pos;
				}
				pos++;
			}
		}
	}
	gettimeofday(&end, NULL);
	cout << "done... " << endl;
	float ms = (float)(end.tv_sec - start.tv_sec) * 1000.0 + (float)(end.tv_usec - start.tv_usec) / 1000.0;
	cout << "computation time(ms): " << ms << endl;
}

vector<pair<unsigned int, unsigned int>> genSeeds(int n, int maxX, int maxY) {

	vector<pair<unsigned int, unsigned int>> v;

	for (int i = 0; i < n; ) {
		bool exist = false;
		unsigned int x = rand() % maxX;
		unsigned int y = rand() % maxY;

		for (auto e : v) {
			if (x == e.first && y == e.second) {
				exist = true;
				break;
			}
		}

		if (exist == false) {
			v.push_back(make_pair(x, y));
			i++;
		}
	}

	return v;
}
