#include "libs/CImg.h"
#include <iostream>
#include <string>
#include <vector>
#include "gaussianpyramid.hpp"
#include "laplacianpyramid.hpp"

#define cimg_use_magick

using namespace cimg_library;

// In order to make it run
// g++ main.cpp -o main.out -L/opt/X11/lib -lX11 -pthread -j8

/* CImg pega os valores de 0 a 255. precisa dividir para pegar de 0 a 1 */

// CImg<double> blend() {

// }

std::vector<std::string> split(const std::string &text, char sep) {
    std::vector<std::string> tokens;
    std::size_t start = 0, end = 0;
    while ((end = text.find(sep, start)) != std::string::npos) {
        std::string temp = text.substr(start, end - start);
        if (temp != "") tokens.push_back(temp);
        start = end + 1;
    }
    std::string temp = text.substr(start);
    if (temp != "") tokens.push_back(temp);
    return tokens;
}

int main(int argc, char * argv[]) {
	
	// Interation with user
	std::string imagesString;
	std::string maskString;

	if(argc >= 2)
	{
		std::string arg1(argv[1]);
		imagesString = "images/" + arg1;
	}
	
	if (argc >= 3)
	{
		std::string arg2(argv[2]);
		imagesString += " images/" + arg2;
	}

	if (argc == 4)
	{
		std::string arg3(argv[3]);
		maskString = "images/" + arg3;
	}
	else { maskString = "images/default.png";}

	std::vector<std::string> images = split(imagesString, ' ');

	const int pyramidSize = 7;
	
	CImg<double> ** pyramidsG = new CImg<double>*[images.size()];
 	CImg<double> ** pyramidsP = new CImg<double>*[images.size()];
	for (int i = 0; i < images.size(); i++) {
		pyramidsG[i] = new CImg<double>[pyramidSize];
 		pyramidsP[i] = new CImg<double>[pyramidSize];
 	}

	// Creating Images Laplacian Pyramids
	for (int i = 0; i < images.size(); i++) {
		const char *const imagePath = images[i].c_str();
		
		CImg<double> image(imagePath);

		// Gaussian pyramid
		GaussianPyramid gPyramid;

		double filter[5] = {1.0/16, 4.0/16, 6.0/16, 4.0/16, 1.0/16};
		gPyramid.generateFilter(filter);

		pyramidsG[i][0] = image;
		CImg<double> reducedImage  = image;
		for (int p = 1; p < pyramidSize; p++){
			pyramidsG[i][p] = gPyramid.reduce(reducedImage);
			reducedImage = pyramidsG[i][p];
		}

		// Laplacian Pyramid
		LaplacianPyramid lPyramid;

		double laplacianFilter[5] = {0.5/8, 2.0/8, 3.0/8, 2.0/8, 0.5/8};
		lPyramid.generateFilter(laplacianFilter);

		for (int p = 1; p < pyramidSize; p++) {
			pyramidsP[i][p - 1] = pyramidsG[i][p - 1] - lPyramid.expand(pyramidsG[i][p]);
		}

		/* The laplacian pyramid has one level less than the gaussian pyramid
		so we repeat the last level of the gaussian pyramid */
		pyramidsP[i][pyramidSize - 1] = pyramidsG[i][pyramidSize - 1];

		// pyramidsG[i][2].display();
		// pyramidsG[i][3].display();
		// pyramidsG[i][3].display();
		// CImg<double> test = lPyramid.expand(pyramidsG[i][1]);
		// test.display();
		// lPyramid.expand(reducedImage).display();
		// pyramidsP[i][0].display();
		//lPyramid.expand(pyramidsP[i][pyramidSize - 1]).display();

		// Collapse Image
		// CImg<double> collapseImage = pyramidsP[i][pyramidSize - 2] + lPyramid.expand(pyramidsP[i][pyramidSize - 1]);
		// for (int a = pyramidSize - 3; a >= 0; a--){
		// 	collapseImage = pyramidsP[i][a] + lPyramid.expand(collapseImage);
		// }
		// collapseImage.display();
		// delete imagePath;
	}

	// Creating Mask Gaussian Pyramid
	const char *const maskPath = maskString.c_str();
	CImg<double> image(maskPath);
	std::vector<CImg<double> > maskGaussianPyramid;
	GaussianPyramid maskGPyramid;

	double filter[5] = {1.0/16, 4.0/16, 6.0/16, 4.0/16, 1.0/16};
	maskGPyramid.generateFilter(filter);

	maskGaussianPyramid.push_back(image);
	CImg<double> reducedImage  = image;
	for (int p = 1; p < pyramidSize; p++){
		CImg<double> maskReduced = maskGPyramid.reduce(reducedImage);
		maskGaussianPyramid.push_back(maskReduced);
		reducedImage = maskReduced;
	}
	// maskGaussianPyramid[3].display();


	// Blending images
	std::vector<CImg<double> > newLaplacianPyramid;
	// std::vector<CImg<double> > newGaussianPyramid;
	for (int i = 0; i < pyramidSize; i++) {
		CImg<double> lsImageLaplacian(pyramidsG[0][i].width(), pyramidsG[0][i].height(),1,pyramidsG[0][i].spectrum(),0);
		// CImg<double> lsImageGaussian(pyramidsG[0][i].width(), pyramidsG[0][i].height(),1,pyramidsG[0][i].spectrum());
		for (int canal = 0; canal < pyramidsG[0][i].spectrum(); canal++){
			for (int x = 0; x < pyramidsG[0][i].width(); x++) {
				for (int y = 0; y < pyramidsG[0][i].height(); y++) {
					lsImageLaplacian(x,y,0,canal) = ((maskGaussianPyramid[i](x,y,0,0)/255.0) * (pyramidsP[0][i](x,y,0,canal))) + ((1.0 - (maskGaussianPyramid[i](x,y,0,0)/255.0))*(pyramidsP[1][i](x,y,0,canal)));
					// lsImageGaussian(x,y,0,canal)  = ((maskGaussianPyramid[i](x,y,0,0)/255) * (pyramidsG[0][i](x,y,0,canal)/255)) + (1 - (maskGaussianPyramid[i](x,y,0,0)/255))*(pyramidsG[1][i](x,y,0,canal)/255);
				}
			}
		}
		newLaplacianPyramid.push_back(lsImageLaplacian);
		// newGaussianPyramid.push_back(lsImageGaussian);
	}

	// // Collapsing
	LaplacianPyramid lPyramid;

	double laplacianFilter[5] = {0.5/8, 2.0/8, 3.0/8, 2.0/8, 0.5/8};
	// double laplacianFilter[5] = {1.0/16, 4.0/16, 6.0/16, 4.0/16, 1.0/16};
	lPyramid.generateFilter(laplacianFilter);
	
	CImg<double> collapsedImage = newLaplacianPyramid[pyramidSize - 2] + lPyramid.expand(newLaplacianPyramid[pyramidSize - 1]);
	for (int i = pyramidSize - 3; i >= 0; i--){
		collapsedImage = newLaplacianPyramid[i] + lPyramid.expand(collapsedImage);
	}

	// collapsedImage.display();
	// collapsedImage.normalize(0, 255);
	// collapsedImage.save("result.png");

// 	imagina q vc tem 4 camadas:
// expande a camada 3, soma na 2 = x
// expande x, soma na 1 = y
// expande y, soma na 0 = piramide colapsada
	

	// delete maskPath;
	for (int i = 0; i < images.size(); i++) {
		delete [] pyramidsG[i];
		delete [] pyramidsP[i];
	}

	delete [] pyramidsG;
	delete [] pyramidsP;

	return 0;
}