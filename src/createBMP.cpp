#include <map>
#include "createBMP.h"
#include "EasyBMP.h"
#include "EasyBMP_Geometry.h"


#define RADIUS 2

using namespace std;

void createBMP(int* Rx, vector<pair<unsigned int, unsigned int>>& P, int xDim, int yDim, int nP, std::string name) {

	map<int, RGBApixel> colors;
	//generating the colors map
	for (int i = 0; i < nP; i++) {
		RGBApixel c;
		c.Red = rand() % 255;
		c.Green = rand() % 255;
		c.Blue = rand() % 255;
		c.Alpha = 0;

		colors[i] = c;
	}

	BMP veronoi;
	veronoi.SetSize(xDim, yDim);
	// Set its color depth to 32-bits
	veronoi.SetBitDepth(32);


	//Drawing the veronoi
	for (int i = 0; i<yDim; i++)
	{
		for (int j = 0; j<xDim; j++)
		{
			auto v = Rx[i * yDim +j];
			auto color = colors[v];

			veronoi(j, i)->Red = color.Red;
			veronoi(j, i)->Green = color.Green;
			veronoi(j, i)->Blue = color.Blue;
			veronoi(j, i)->Alpha = color.Alpha;
		}
	}


	//drawing the seeds on the voronoi;
	RGBApixel c;
	c.Red = 0;
	c.Green = 0;
	c.Red = 0;
	c.Alpha = 0;
	for (auto& p : P) {
		auto x = p.first;
		auto y = p.second;

		veronoi(x, y)->Red = c.Red;
		veronoi(x, y)->Green = c.Green;
		veronoi(x, y)->Blue = c.Blue;
		veronoi(x, y)->Alpha = c.Alpha;
		DrawArc(veronoi, x, y, RADIUS, 0, 6.28, c);


	}
	veronoi.WriteToFile(name.c_str());


}
