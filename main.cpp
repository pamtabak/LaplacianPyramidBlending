#include "libs/CImg.h"
#include <iostream>
#include <string>
#include <vector>
#include "gaussianpyramid.hpp"
#include "laplacianpyramid.hpp"

#define cimg_use_magick

using namespace cimg_library;

// g++ main.cpp -o main.out -L/opt/X11/lib -lX11 -pthread

int main(int argc, char * argv[]) {
	
	// Interation with user
	std::string maskString;
	std::vector<std::string> images;

	if(argc >= 2)
	{
		std::string arg1(argv[1]);
		images.push_back("images/" + arg1);
	}
	if (argc >= 3)
	{
		std::string arg2(argv[2]);
		images.push_back("images/" + arg2);
	}
	if (argc == 4)
	{
		std::string arg3(argv[3]);
		maskString = "images/" + arg3;
	}
	else { maskString = "images/default.png";}

	const int pyramidSize = 4;

 	std::vector<std::vector<CImg <double> > > pyramidsP;
 	std::vector<std::vector<CImg <double> > > pyramidsG;

 	GaussianPyramid gPyramid;
	double filter[5] = {1.0/16, 4.0/16, 6.0/16, 4.0/16, 1.0/16};
	gPyramid.generateFilter(filter);

	LaplacianPyramid lPyramid;
	lPyramid.generateFilter(filter);	

	for (int i = 0; i < images.size(); i++) {
		const char *const imagePath = images[i].c_str();		
		CImg<double> image(imagePath);

		std::vector<CImg<double> > gaussianPyramids;
		std::vector<CImg<double> > laplacianPyramids;
		
		gaussianPyramids.push_back(image);
		CImg<double> reducedImage  = image;
		for (int p = 1; p < pyramidSize; p++){
			CImg<double> imageGenerated = gPyramid.reduce(reducedImage);
			gaussianPyramids.push_back(imageGenerated);
			reducedImage = imageGenerated;
		}
		
		for (int p = 1; p < pyramidSize; p++) { 
			laplacianPyramids.push_back(gaussianPyramids[p - 1] - 4*lPyramid.expand(gaussianPyramids[p]));
		}
		laplacianPyramids.push_back(gaussianPyramids[pyramidSize - 1]);

		pyramidsG.push_back(gaussianPyramids);
		pyramidsP.push_back(laplacianPyramids);


		// Collapse Image
		// CImg<double> collapseImage = laplacianPyramids[pyramidSize - 2] + 4*lPyramid.expand(laplacianPyramids[pyramidSize - 1]);
		// for (int a = pyramidSize - 3; a >= 0; a--){
		// 	collapseImage = laplacianPyramids[a] + 4*lPyramid.expand(collapseImage);
		// }
		// collapseImage.display();
	}

	// Creating Mask Gaussian Pyramid
	const char *const maskPath = maskString.c_str();
	CImg<double> image(maskPath);
	std::vector<CImg<double> > maskGaussianPyramid;

	maskGaussianPyramid.push_back(image);
	CImg<double> reducedImage  = image;
	for (int p = 1; p < pyramidSize; p++){
		CImg<double> maskReduced = gPyramid.reduce(reducedImage);
		maskGaussianPyramid.push_back(maskReduced);
		reducedImage = maskReduced;
	}

	// Blending images
	std::vector< CImg<double> > newLaplacianPyramid;
	for (int i = 0; i < pyramidSize; i++) {
		CImg<double> lsImageLaplacian(pyramidsP[0][i].width(), pyramidsP[0][i].height(),1,pyramidsP[0][i].spectrum(),0);
		for (int canal = 0; canal < pyramidsP[0][i].spectrum(); canal++){
			for (int x = 0; x < pyramidsP[0][i].width(); x++) {
				for (int y = 0; y < pyramidsP[0][i].height(); y++) {
					lsImageLaplacian(x,y,0,canal) = ((maskGaussianPyramid[i](x,y)/255) * (pyramidsP[0][i](x,y,0,canal))) 
					+ ((1 - (maskGaussianPyramid[i](x,y)/255))*(pyramidsP[1][i](x,y,0,canal)));
				}
			}
		}
		newLaplacianPyramid.push_back(lsImageLaplacian);
	}	

	// newLaplacianPyramid[1].display();
	// (4*lPyramid.expand(newLaplacianPyramid[pyramidSize - 1])).display();
	CImg<double> collapsedImage = newLaplacianPyramid[pyramidSize - 2] + 4*lPyramid.expand(newLaplacianPyramid[pyramidSize - 1]);
	for (int i = pyramidSize - 3; i >= 0; i--){
		collapsedImage = newLaplacianPyramid[i] + 4*lPyramid.expand(collapsedImage);
	}
	collapsedImage.display();
	
	collapsedImage.normalize(0,255).save("result.png");
	return 0;
}