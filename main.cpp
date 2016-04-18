#include "libs/CImg.h"
#include <iostream>
#include <string>
#include "gaussianpyramid.hpp"
#include "laplacianpyramid.hpp"

#define cimg_use_magick

using namespace cimg_library;

// In order to make it run
// g++ main.cpp -o main.out -L/opt/X11/lib -lX11 -pthread 

/* CImg pega os valores de 0 a 255. precisa dividir para pegar de 0 a 1 */

CImg<double> blend() {

}

int main(int argc, char * argv[]) {
	// Interation with user
	// std::string images;
	// std::cout << "Digite o nome das imagens, separado por espaco, que estao na pasta images." << std::endl;
	// std::cin >> images;

	int pyramidSize = 6;

	CImg<double> image("images/lena.jpg");

	// Gaussian pyramid
	GaussianPyramid gPyramid;

	double filter[5] = {1.0/16, 4.0/16, 6.0/16, 4.0/16, 1.0/16};
	gPyramid.generateFilter(filter);
	 
	CImg<double> * gaussianPyramid = new CImg<double>[pyramidSize];

	gaussianPyramid[0] = image;
	CImg<double> reducedImage  = image;
	for (int p = 1; p < pyramidSize; p++){
		gaussianPyramid[p] = gPyramid.reduce(reducedImage);
	 	reducedImage = gaussianPyramid[p];
	}

	// Laplacian Pyramid
	LaplacianPyramid lPyramid;

	double laplacianFilter[5] = {0.5/8, 2.0/8, 3.0/8, 2.0/8, 0.5/8};
	lPyramid.generateFilter(laplacianFilter);

	CImg<double> * laplacianPyramid = new CImg<double>[pyramidSize];

	for (int p = 1; p < pyramidSize; p++)
	{
		laplacianPyramid[p - 1] = gaussianPyramid[p - 1] - lPyramid.expand(gaussianPyramid[p]);
	}

	/* The laplacian pyramid has one level less than the gaussian pyramid
	so we repeat the last level of the gaussian pyramid */
	laplacianPyramid[pyramidSize - 1] = gaussianPyramid[pyramidSize - 1] - lPyramid.expand(gaussianPyramid[pyramidSize - 1]);

	// Collapsing
	// CImg<double> collapsedImage = laplacianPyramid[0] + lPyramid.expand(gaussianPyramid[1]);	
	// collapsedImage.display();

	delete [] gaussianPyramid;
	delete [] laplacianPyramid;

	return 0;
}