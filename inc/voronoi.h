#ifndef __VORONOI_H__
#define __VORONOI_H__

#include <vector>

void voronoi(int* vec, std::vector<std::pair<unsigned int, unsigned int>> &P, int xDim = 0, int yDim = 0);
std::vector<std::pair<unsigned int, unsigned int>> genSeeds(int n, int maxX, int maxY);

#endif // !__VORONOI_H__

