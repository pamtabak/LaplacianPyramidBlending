// #define cimg_use_jpeg

#include "libs/CImg.h"
#include <iostream>
#include "gaussianpyramid.hpp"

#define cimg_use_magick

using namespace cimg_library;

// In order to make it run
// g++ main.cpp -o main.out -L/opt/X11/lib -lX11 -pthread 

int main(int argc, char * argv[])
{
	int pyramidSize = 3;

	 CImg<double> image("images/lena.jpg");


	 // Gaussian pyramid
	 GaussianPyramid gPyramid; //= new GaussianPyramid();

	 double filter[5] = {1.0/16, 4.0/16, 6.0/16, 4.0/16, 1.0/16};
	 gPyramid.generateFilter(filter);
	 
	 CImg<double> * gaussianPyramid = new CImg<double>[pyramidSize];

	 CImg<double> reducedImage  = image;
	 for (int p = 0; p < pyramidSize; p++){
	 	gaussianPyramid[p] = gPyramid.reduce(reducedImage);
	 	reducedImage = gaussianPyramid[p];
	 }

	 gaussianPyramid[1].display();

	 // delete gaussianPyramid;

	// if (argc == 2 && argv[1] == "--help") {
	// 	std::cout << "Voce precisa passar duas imagens" << std::endl;
	// }
	// if (argc == 3) {
	// 	// User passed 2 images and no binary mask
	// }
	// else if (argc == 4) {
	// 	// User passed 2 imagens and binary mask
	// }
	// else {
	// 	// Does nothing. Print error
	// 	return -1;
	// }
	// std::cout << argc << std::endl;

	// std::cout << "Hello World" << std::endl;
	return 0;
}