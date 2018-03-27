/*
 * Primitive.cpp
 *
 *  Created on: Feb 19, 2009
 *      Author: njoubert
 */

#include "Primitives.h"

Primitive::Primitive(RGB & c, Material & m, mat4 modelToWorld) {
    _c = c;
    _m = m;
    _modelToWorld = modelToWorld;
    _worldToModel = modelToWorld.inverse();
}

Primitive::~Primitive() {
}

void Primitive::setColor(RGB & c) {
    _c = c;
}

void Primitive::setMaterial(Material & m) {
    _m = m;
}

const RGB& Primitive::getColor() {
    return _c;
}
const Material& Primitive::getMaterial() {
    return _m;
}


Sphere::Sphere(double radius, RGB & c, Material & m, mat4 m2w): Primitive(c,m,m2w) {
    _r = radius;
}


//Checks for intersection with the given ray
double Sphere::intersect(Ray & ray) {
	//YOUR CODE HERE
	/*
	c is sphere center which is stored in fourth column of _modelToWorld (vec4) (_modelToWorld.transpose())[3]

	rayStart is e
	raydirection is d

	*/
	vec4 c = (_modelToWorld.transpose())[3];
	double val1 = ray.direction() * (ray.start() - c);
	val1 = pow(val1, 2);

	double val2 = (ray.direction()*ray.direction())*((ray.start() - c)*(ray.start() - c) - pow(_r, 2));

	if (val1 - val2 < 0) return -1;
	val1 = pow(val1 - val2, 1 / 2);
	val2 = (-1*ray.direction())*(ray.start() - c);

	double val3 = 1 / (ray.direction()*ray.direction());

	if (val2 - val1 > val2 + val1) return val3*(val2 - val1);

	else return val3*(val2 + val1);
}

//Calculates the normal for the given position on this sphere.
inline vec4 Sphere::calculateNormal(vec4 & position) {
	//YOUR CODE HERE
	vec4 c = (_modelToWorld.transpose())[3];
	return vec4((c - position) / _r);

}


Triangle::Triangle(vec3 a, vec3 b, vec3 c, RGB & col, Material & m, mat4 m2w) : Primitive(col,m,m2w) {
    verts[0] = a; verts[1] = b; verts[2] = c;
}


//*
double Triangle::intersect(Ray & ray) {
	//OPTIONAL EXTRA CREDIT YOUR CODE HERE
    IMPLEMENT_ME(__FILE__,__LINE__);
}
// */

vec4 Triangle::calculateNormal(vec4 & position) {
	//OPTIONAL EXTRA CREDIT YOUR CODE HERE
    IMPLEMENT_ME(__FILE__,__LINE__);
}