/*
 * World.cpp
 *
 *  Created on: Feb 19, 2009
 *      Author: njoubert
 */

#include "World.h"

World::World() {
}

World::~World() {
    // TODO Auto-generated destructor stub
}

Primitive* World::intersect(Ray & r, double & t) {
	int mini = 0;
	double min = INT_MAX;
	if (_primitives[0]->intersect(r)>0)
		min = _primitives[0]->intersect(r);

	for (int x = 1; x < _primitives.size(); x++) {
		if (_primitives[x]->intersect(r) >0 && _primitives[x]->intersect(r) < min) {
			min = _primitives[0]->intersect(r);
			mini = x;
		}
	}
	if (min != INT_MAX) {
		return _primitives[mini];
	}
	else {
		return NULL;
	}
}

void World::addPrimitive(Primitive* p) {
    _primitives.push_back(p);
}

void World::addLight(Light* l) {
    _lights.push_back(l);
}

void World::setAmbientLightColor(RGB ambientColor) {
    _ambientLight.setColor(ambientColor);
}

const RGB & World::getAmbientLightColor() {
    return _ambientLight.getColor();
}

vector<Light*>::const_iterator World::getLightsBeginIterator() {
    return _lights.begin();
}

vector<Light*>::const_iterator World::getLightsEndIterator() {
    return _lights.end();
}

void World::printStats() {
	cout << "World data:" << endl;
	cout << " primitives: " << _primitives.size() << endl;
	cout << " lights: " << _lights.size() << endl;
	
}
