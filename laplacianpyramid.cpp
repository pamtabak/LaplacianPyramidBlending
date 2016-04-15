#include "libs/CImg.h"
#include <iostream>

using namespace cimg_library;

class LaplacianPyramid() {

public:

	LaplacianPyramid () {

	}
	~LaplacianPyramid() {

	}


	void generateFilter (double filter[5]) {
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				filter2d[i][j] = filter[i]*filter[j];
			}
		}
	}

private:	
	double filter2d[5][5];
}