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
		
		CImg<double> reducedImage(image.width()/2, image.height()/2);

		for (int x = 0; x < image.width(); x+=2) {
			for (int y = 0; y < image.height(); y+=2) {
				for (int canal = 0; canal < image.spectrum(); canal++){
					reducedImage(x,y,0,canal) = 0.0;
					// multiplicacao de matrizes
					for (int i = -2; i <= 2; i++){
						for (int j = -2; j <= 2; j++) {
							// TRATAR EXCECOES (BORDAS) -- ESPELHAR IMAGEM PARA FORA
							// if (x < 2) or if (y < 2) or if (x > image.width()/2)
							reducedImage(x,y,0,canal) += image(x + i, y + j,0,canal)*filter2d[i+2][j+2];
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