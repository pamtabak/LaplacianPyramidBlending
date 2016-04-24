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
	    int width       = image.width();
	    int height      = image.height();
	    int imageCanals = image.spectrum();
	 	
	    int newWidth = width/2;
	    int newHeight = height/2;
	 
	    CImg<double> reducedImage(newWidth, newHeight, 1, imageCanals,0);
	 
	    for (int x = 0; x < newWidth; x++) {
	        for (int y = 0; y < newHeight; y++) {
	            for (int canal = 0; canal < imageCanals; canal++) {
	                reducedImage(x,y,0,canal) = 0.0;
	                for (int i = -2; i <= 2; i++) {
	                    for (int j = -2; j <= 2; j++) {
	                        int newValueOfX = ((x*2 + i) < 0) ? ((-x*2) - i - 1) : x*2 + i;
	                        newValueOfX = (newValueOfX >= width) ? ((2 * width) - x*2 - i - 1) : newValueOfX;
	                        int newValueOfY = ((y*2 + j) < 0) ? ((-y*2) - j - 1) : y*2 + j;
	                        newValueOfY = (newValueOfY >= height) ? ((2 * height) - y*2 - j - 1) : newValueOfY;
	                        
	                        reducedImage(x,y,0,canal) += image(newValueOfX, newValueOfY,0,canal) * filter2d[i+2][j+2];
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