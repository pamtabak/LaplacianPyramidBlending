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

int main(int argc, char * argv[])
{
	// Interation with user
	// std::string images;
	// std::cout << "Digite o nome das imagens, separado por espaco, que estao na pasta images." << std::endl;
	// std::cin >> images;

	int pyramidSize = 4;

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

	// gaussianPyramid[1].display();

	// Laplacian Pyramid
	LaplacianPyramid lPyramid;

	double laplacianFilter[5] = {1.0/8, 4.0/8, 6.0/8, 4.0/8, 1.0/8};
	lPyramid.generateFilter(laplacianFilter);

	CImg<double> a = lPyramid.expand(gaussianPyramid[0]);
	a.display();

	delete [] gaussianPyramid;

	return 0;
}