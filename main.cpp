#include "libs/CImg.h"
#include <iostream>
#include <string>
#include <vector>
#include "gaussianpyramid.hpp"
#include "laplacianpyramid.hpp"

#define cimg_use_magick

using namespace cimg_library;

// In order to make it run
// g++ main.cpp -o main.out -L/opt/X11/lib -lX11 -pthread 

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
	std::cout << "Digite o nome (com a extensao) das imagens, separado por espaco, que estao na pasta images." << std::endl;
	std::cin >> imagesString;
	std::cout << "Digite o nome (com a extensao) da mascara contida na pasta images." << std::endl;
	std::cin >> maskString;

	std::vector<std::string> images = split(imagesString, ' ');
	for (int i = 0; i < images.size(); i++) {
		images[i] = "images/" + images[i];
	}
	maskString = "images/" + maskString;
	

	const int pyramidSize = 6;
	
	CImg<double> ** pyramidsG = new CImg<double>*[images.size()];
	CImg<double> ** pyramidsP = new CImg<double>*[images.size()];
	for (int i = 0; i < images.size(); i++) {
		pyramidsG[i] = new CImg<double>[pyramidSize];
		pyramidsP[i] = new CImg<double>[pyramidSize];
	}
	// for (int i = 0; i < images.size(); i++){
		// pyramidsG[i] = new CImg<double>[pyramidSize];
	// }
	// CImg<double> ** pyramidsP = new CImg<double>[images.size()][pyramidSize];

	for (int i = 0; i < images.size(); i++) {
		const char *const imagePath = images[i].c_str();
		std::cout << imagePath << std::endl;
		
		CImg<double> image(imagePath);
		image.display();

		// Gaussian pyramid
		GaussianPyramid gPyramid;

		double filter[5] = {1.0/16, 4.0/16, 6.0/16, 4.0/16, 1.0/16};
		gPyramid.generateFilter(filter);
		 
		// CImg<double> * gaussianPyramid = new CImg<double>[pyramidSize];

		pyramidsG[i][0] = image;
		// gaussianPyramid[0] = image;
		CImg<double> reducedImage  = image;
		for (int p = 1; p < pyramidSize; p++){
			pyramidsG[i][p] = gPyramid.reduce(reducedImage);
			reducedImage = pyramidsG[i][p];
			// gaussianPyramid[p] = gPyramid.reduce(reducedImage);
		 	// reducedImage = gaussianPyramid[p];
		}

		// pyramidsG.push_back(gaussianPyramid);

		// Laplacian Pyramid
		LaplacianPyramid lPyramid;

		double laplacianFilter[5] = {0.5/8, 2.0/8, 3.0/8, 2.0/8, 0.5/8};
		lPyramid.generateFilter(laplacianFilter);

		// CImg<double> * laplacianPyramid = new CImg<double>[pyramidSize];

		for (int p = 1; p < pyramidSize; p++)
		{
			pyramidsP[i][p - 1] = pyramidsG[i][p - 1] - lPyramid.expand(pyramidsG[i][p]);
			// laplacianPyramid[p - 1] = gaussianPyramid[p - 1] - lPyramid.expand(gaussianPyramid[p]);
		}

		/* The laplacian pyramid has one level less than the gaussian pyramid
		so we repeat the last level of the gaussian pyramid */
		pyramidsP[i][pyramidSize - 1] = pyramidsG[i][pyramidSize - 1] - lPyramid.expand(pyramidsG[i][pyramidSize - 1]);
		// laplacianPyramid[pyramidSize - 1] = gaussianPyramid[pyramidSize - 1] - lPyramid.expand(gaussianPyramid[pyramidSize - 1]);

		// pyramidsP.push_back(laplacianPyramid);

		// Collapsing
		CImg<double> collapsedImage = pyramidsP[i][0] + lPyramid.expand(pyramidsG[i][1]);	
		collapsedImage.display();
	}

	for (int i = 0; i < images.size(); i++) {
		delete [] pyramidsG[i];
		delete [] pyramidsP[i];
	}

	delete [] pyramidsG;
	delete [] pyramidsP;

	return 0;
}