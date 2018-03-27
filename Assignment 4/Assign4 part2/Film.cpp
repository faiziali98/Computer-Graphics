/*
 * Film.cpp
 *
 *  Created on: Feb 19, 2009
 *      Author: njoubert
 */

#include "Film.h"

Film::Film(int w, int h, string filename) {
	_w = w;
	_h = h;
	_filename = filename;
}

Film::~Film() {

}

/**
 * Here we stick the "saving" of the image.
 * In our first case, this involves using OpenGL to display the pixels.
 */

RGBQUAD arr[10000][10000];
void Film::expose(RGB c, Sample & s) {
	//YOUR CODE HERE

	RGBQUAD colour;
	colour.rgbBlue = c[BLUE] * 255;
	colour.rgbRed = c[RED] * 255;
	colour.rgbGreen = c[GREEN] * 255;
	int x = s.x();
	int y = s.y();
	arr[x][y] = colour;

}

void Film::bakeAndSave() {
	FreeImage_Initialise();	
	FIBITMAP *output = FreeImage_Allocate(_w, _h, 24);
	
	//YOUR CODE HERE
	for (int x = 0; x < _w; x++) {
		for (int y = 0; y < _h; y++) {
			FreeImage_SetPixelColor(output, x, y, &arr[x][y]);
		}
	}
	
	FreeImage_Save(FIF_PNG, output, _filename.c_str());
	FreeImage_DeInitialise();
}
