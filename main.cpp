// #define cimg_use_jpeg

#include "libs/CImg.h"
#include <iostream>
#include "pyramid.hpp"

#define cimg_use_magick

using namespace cimg_library;

// In order to make it run
// g++ main.cpp -o main -L/opt/X11/lib -lX11 -pthread 

int main(int argc, char * argv[])
{

	 CImg<double> image("images/lena.jpg");

	 GaussianPyramid gaussianPyramid;// = new GaussianPyramid();

	 double filter[5] = {1.0/16, 4.0/16, 6.0/16, 4.0/16, 1.0/16};
	 gaussianPyramid.generateFilter(filter);
	 
	 // image.display();

	 CImg<double> reducedImage = gaussianPyramid.reduce(image);

	 reducedImage.display();

	//  const unsigned char red[] = { 255,0,0 }, green[] = { 0,255,0 }, blue[] = { 0,0,255 };
	//  image.blur(2.5);
 //     CImgDisplay main_disp(image,"Click a point"), draw_disp(visu,"Intensity profile");
 //     while (!main_disp.is_closed() && !draw_disp.is_closed()) {
	//     main_disp.wait();
	//     if (main_disp.button() && main_disp.mouse_y()>=0) {
	//       const int y = main_disp.mouse_y();
	//       visu.fill(0).draw_graph(image.get_crop(0,y,0,0,image.width()-1,y,0,0),red,1,1,0,255,0);
	//       visu.draw_graph(image.get_crop(0,y,0,1,image.width()-1,y,0,1),green,1,1,0,255,0);
	//       visu.draw_graph(image.get_crop(0,y,0,2,image.width()-1,y,0,2),blue,1,1,0,255,0).display(draw_disp);
	//       }
	// }


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