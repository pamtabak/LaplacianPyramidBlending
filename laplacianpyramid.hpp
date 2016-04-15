#include "libs/CImg.h"
#include <iostream>

using namespace cimg_library;

class LaplacianPyramid {
public:
	LaplacianPyramid() {

	}
	~LaplacianPyramid() {

	}

	CImg<double> expand (CImg<double> image) {
		int width       = image.width();
		int height      = image.height();
		int imageCanals = image.spectrum();

		CImg<double> expandedImage(2 * width, 2 * height, 1, imageCanals);

		// Iterating over each canal
		for (int canal = 0; canal < imageCanals; canal++) {
			// Iterating over each pixel of the image
			for (int y = 0; y < 2*height; y+=2) {
				for (int x = 0; x < 2*width; x+=2) {
					expandedImage(x,y,0,canal) = image(x/2,y/2,0,canal);
					expandedImage(x+1, y+1,0,canal) = 0.0;
				}
			}
		}

		CImg<double> filteredImage(2 * width, 2 * height, 1, imageCanals);

		for (int canal = 0; canal < imageCanals; canal++) {
			for (int y = 0; y < 2 * height; y++){
				for (int x = 0; x < 2 * width; x++){
					filteredImage(x,y,0,canal) = 0.0;
					for (int i = -2; i <= 2; i++){
						for (int j = -2; j <= 2; j++) {
							// we need to verify the edges
							int newValueOfX = ((x + i) < 0) ? (-x - i - 1) : x + i;
							newValueOfX = (newValueOfX >= 2 * width) ? ((4 * width) - x - i - 1) : newValueOfX;
							int newValueOfY = ((y + j) < 0) ? (-y - j - 1) : y + j;
							newValueOfY = (newValueOfY >= 2 * height) ? ((4 * height) - y - j - 1) : newValueOfY;

							filteredImage(x,y,0,canal) += expandedImage(newValueOfX, newValueOfY,0,canal) * filter2d[i+2][j+2];
						}
					}
				}
			}
		}

		return filteredImage;
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
};