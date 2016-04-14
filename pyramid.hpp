#include "libs/CImg.h"

using namespace cimg_library;

class GaussianPyramid {
public:
	GaussianPyramid () {

	}

	~GaussianPyramid () {

	}

	CImg<double> reduce (CImg<double> image) {
		// Suavizar e reduzir Imagem pela metade de forma alternada linhas e colunas
		
		int width  = image.width();
		int height = image.height();

		CImg<double> reducedImage(width/2, height/2);

		// Iterating over each pixel of the image, skipping columns and lines that are odd
		for (int x = 0; x < width; x+=2) {
			for (int y = 0; y < height; y+=2) {
				// Iterating over each canal
				for (int canal = 0; canal < image.spectrum(); canal++) {
					// multiplying matrices
					reducedImage(x,y,0,canal) = 0.0;
					for (int i = -2; i <= 2; i++) {
						for (int j = -2; j <= 2; j++) {
							// Making exceptions for edges. We need to mirror the image
							int newValueOfX = ((x + i) < 0) ?  (-x - i - 1) : x + i;
							newValueOfX = (newValueOfX >= width) ? ((2 * width) - x - i - 1) : newValueOfX;
							int newValueOfY = ((y + j) < 0) ? (-y - j - 1) : y + j;
							newValueOfY = (newValueOfY >= height) ? (PREENCHER) : newValueOfY;

							reducedImage(x,y,0,canal) += image(newValueOfX, newValueOfY,0,canal)*filter2d[i+2][j+2];
						}
					}
				}
				
			}
		}

		return reducedImage;
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