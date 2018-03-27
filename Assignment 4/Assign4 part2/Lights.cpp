/*
 * Lights.cpp
 *
 *  Created on: Feb 19, 2009
 *      Author: njoubert
 */

#include "Lights.h"


Light::Light() {
    RGB black(0,0,0);
    _illumination = black;
	_falloff = 0;
	_angularFalloff = 0;
	_deadDistance = 1;
}

Light::Light(RGB illumination) {
    _illumination = illumination;
}
Light::Light(RGB illumination, double falloff, double deadDistance) {
	_illumination = illumination;
		_falloff = falloff;
		_deadDistance = deadDistance;
}

Light::~Light() { }

const RGB & Light::getColor() {
    return _illumination;
}
const RGB Light::getColor(vec4 & p) {
    return _illumination;
}

void Light::setColor(RGB c) {
    _illumination = c;
}

AmbientLight::AmbientLight() {
}

AmbientLight::AmbientLight(RGB illumination) : Light(illumination) {
}

void AmbientLight::getIncidenceVector(vec4 & position, vec4 & returnValue) {
	throw "AMBIENT LIGHTS DO NOT HAVE A SENSE OF DIRECTION OR POSITION`";
}
Ray AmbientLight::getShadowRay(vec4 & position, bool & useDist) {
	throw "AMBIENT LIGHTS DO NOT HAVE A SENSE OF DIRECTION OR POSITION";
}


PointLight::PointLight(RGB illumination) : Light(illumination){
}
PointLight::PointLight(RGB illumination, double falloff, double deadDistance) : 
	Light(illumination, falloff, deadDistance) {
}

const RGB PointLight::getColor(vec4 & p) {
	//YOUR CODE HERE
	double toR = 1/pow((_pos - p).length() + _deadDistance, _falloff);
	return toR * _illumination;

}
void PointLight::setPosition(vec4 pos) {
    _pos = pos;
}

void PointLight::getIncidenceVector(vec4 & position, vec4 & returnValue) {
    //YOUR CODE HERE
	returnValue = (_pos - position).normalize();

}
Ray PointLight::getShadowRay(vec4 & position, bool & useDist) {
	//YOUR CODE HERE
	return Ray((vec3) position, (vec3) _pos, 0.1);
}


DirectionalLight::DirectionalLight(RGB illumination) : Light(illumination){
}

void DirectionalLight::setDirection(vec4 dir) {
    _dir = dir;
    _dir[VW] = 0.0;
}

void DirectionalLight::getIncidenceVector(vec4 & position, vec4 & returnValue) {
    //YOUR CODE HERE
	returnValue = (-1 * _dir).normalize();
}
Ray DirectionalLight::getShadowRay(vec4 & position, bool & useDist) {
	//YOUR CODE HERE
	vec4 end = position - _dir;
	return Ray((vec3) position, (vec3)end, 0.1);
}
