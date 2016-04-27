#include "libs/CImg.h"
#include <iostream>

using namespace cimg_library;

class LaplacianPyramid {
public:
	LaplacianPyramid() {

	}
	~LaplacianPyramid() {

	}

	CImg<double> expand (CImg<double> image, int w = 0, int h = 0) {
		int width       = (2*image.width()) + w;
		int height      = (2*image.height()) + h;
		int imageCanals = image.spectrum();

		CImg<double> expandedImage(width, height, 1, imageCanals,0);
		
		for (int canal = 0; canal < imageCanals; canal++) {
			// Iterating over each pixel of the image
			for (int y = 0; y < height; y+=2) {
				for (int x = 0; x < width; x+=2) {
					expandedImage(x,y,0,canal) = image(x/2,y/2,0,canal);
				}
			}
		}

		CImg<double> filteredImage(width, height, 1, imageCanals,0);

		for (int canal = 0; canal < imageCanals; canal++) {
			for (int y = 0; y < height; y++){
				for (int x = 0; x < width; x++){
					filteredImage(x,y,0,canal) = 0.0;
					for (int i = -2; i <= 2; i++){
						for (int j = -2; j <= 2; j++) {
							// we need to verify the edges
							int newValueOfX = ((x + i) < 0) ? (-x - i - 1) : x + i;
							newValueOfX = (newValueOfX >= (width-w)) ? ((2 * (width-w)) - x - i - 1) : newValueOfX;
							int newValueOfY = ((y + j) < 0) ? (-y - j - 1) : y + j;
							newValueOfY = (newValueOfY >= (height-h)) ? ((2 * (height-h)) - y - j - 1) : newValueOfY;

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

	CImg<double> filterImage(CImg<double> image){
		int width       = image.width();
		int height      = image.height();
		int imageCanals = image.spectrum();
		
		CImg<double> filteredImage(width, height, 1, imageCanals,0);

		for (int canal = 0; canal < imageCanals; canal++) {
			for (int y = 0; y < height; y++){
				for (int x = 0; x < width; x++){
					filteredImage(x,y,0,canal) = 0.0;
					for (int i = -2; i <= 2; i++){
						for (int j = -2; j <= 2; j++) {
							// we need to verify the edges
							int newValueOfX = ((x + i) < 0) ? (-x - i - 1) : x + i;
							newValueOfX = (newValueOfX >= (width)) ? ((2 * (width)) - x - i - 1) : newValueOfX;
							int newValueOfY = ((y + j) < 0) ? (-y - j - 1) : y + j;
							newValueOfY = (newValueOfY >= (height)) ? ((2 * (height)) - y - j - 1) : newValueOfY;

							filteredImage(x,y,0,canal) += image(newValueOfX, newValueOfY,0,canal) * filter2d[i+2][j+2];
						}
					}
				}
			}
		}
		return filteredImage;
	}

private:	
	double filter2d[5][5];
};