/*
 * Viewport.cpp
 *
 *  Created on: Feb 19, 2009
 *      Author: njoubert
 */

#include "Viewport.h"

Viewport::Viewport(vec4 eye, vec4 LL, vec4 UL, vec4 LR, vec4 UR, int width, int height) {
    _eye = eye;	//You should use this for the viewing ray
    _LL = LL;
    _UL = UL;
    _LR = LR;
    _UR = UR;
    _pixelsWide = width;
    _pixelsHigh = height;
	_raysPerPixel = RAYS_PER_PIXEL;
	_incPP = sqrt((float)_raysPerPixel);
}

Viewport::~Viewport() {

}

void Viewport::resetSampler() {
    _x = _y = 1.0 / (_incPP*2); //Starts off at the center of a pixel.
}


/**
 * This ONLY fills in the (i, j) coordinates of the point according
 * to the pixels_wide and pixels_high settings
 */
bool Viewport::getSample(Sample & s) {
    //YOUR CODE HERE
	double x = s.x() + 1;
	double y = s.y() + 1;
	if (x <= _pixelsWide) {
		s.setX(x);
		return true;
	}
	else if (y <= _pixelsHigh){
		s.setX(0);
		s.setY(y);
		return true;
	}
	else {
		return false;
	}
}

Ray Viewport::createViewingRay(Sample & s) {
    //YOUR CODE HERE
	vec3 myEye(_eye);
	double myX = _LL[VX] + (s.x() + 0.5) * (_UR[VX] - _LL[VX]) / (_pixelsWide);
	double myY = _LL[VY] + (s.y() + 0.5) * (_UR[VY] - _LL[VY]) / (_pixelsHigh);
	vec3 myPoints(myX, myY, _LL[VZ]);

	return Ray(myEye, myPoints, 1.0);

}

vec4 Viewport::getViewVector(vec4 & pos) {
    //YOUR CODE HERE
	IMPLEMENT_ME(__FILE__,__LINE__);
}

int Viewport::getW() { return _pixelsWide; }

int Viewport::getH() { return _pixelsHigh; }

