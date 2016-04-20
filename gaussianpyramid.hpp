#include "libs/CImg.h"
#include <iostream>

using namespace cimg_library;

class GaussianPyramid {
public:
	GaussianPyramid () {

	}

	~GaussianPyramid () {

	}

	CImg<double> reduce (CImg<double> image) {
		// Suavizar e reduzir Imagem pela metade de forma alternada linhas e colunas
		int width       = image.width();
		int height      = image.height();
		int imageCanals = image.spectrum();

		int modifiedWidth = width;
		int modifiedHeight = height;

		if (width % 2 != 0){
			modifiedWidth++;
		}
		if (height % 2 != 0){
			modifiedHeight++;
		}

		CImg<double> reducedImage(modifiedWidth/2, modifiedHeight/2, 1, imageCanals,0);

		for (int x = 0; x < modifiedWidth; x+=2) {
			for (int y = 0; y < modifiedHeight; y+=2) {
				// Iterating over each canal
				for (int canal = 0; canal < imageCanals; canal++) {
					// multiplying matrices
					reducedImage(x/2,y/2,0,canal) = 0.0;
					for (int i = -2; i <= 2; i++) {
						for (int j = -2; j <= 2; j++) {
							// Making exceptions for edges. We need to mirror the image
							int newValueOfX = ((x + i) < 0) ? (-x - i - 1) : x + i;
							newValueOfX = (newValueOfX >= width) ? ((2 * width) - x - i - 1) : newValueOfX;
							int newValueOfY = ((y + j) < 0) ? (-y - j - 1) : y + j;
							newValueOfY = (newValueOfY >= height) ? ((2 * height) - y - j - 1) : newValueOfY;

							reducedImage(x/2,y/2,0,canal) += image(newValueOfX, newValueOfY,0,canal) * filter2d[i+2][j+2];
						}	
					}
				}
			}
		}

		return reducedImage;
	}

// 540 270 135 67

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