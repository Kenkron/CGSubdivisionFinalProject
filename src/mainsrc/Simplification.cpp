/*
 * simplification.cpp
 *
 *  Created on: Nov 24, 2015
 *      Author: kenkron
 */

#include "GL/glew.h"
#include <stdio.h>
#include <string.h>
#include <map>
#include "STTriangleMesh.h"
#include "Simplification.h"

Simplification::Simplification() {

}

Simplification::~Simplification() {

}

STTriangleMesh* Simplification::simplify(STTriangleMesh* original,
		int maxPolygons) {
	//TODO: implement a basic simplification algorithm
	return new STTriangleMesh(*original);
}
