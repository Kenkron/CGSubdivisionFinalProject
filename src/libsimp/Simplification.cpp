/*
 * simplification.cpp
 *
 *  Created on: Nov 24, 2015
 *      Author: kenkron
 */
#include "stglew.h"

#include <stdio.h>
#include <string.h>
#include <map>
#include "STTriangleMesh.h"
#include "Simplification.h"

/**Base simplification class.  Uses simple vertex decimation to meet the maximum number of polygons*/
class Simplification{

	Simplification(){

	}

	~Simplification(){

	}

	STTriangleMesh simplify(STTriangleMesh original, int maxPolygons){
		//TODO: implement a basic simplification algorithm
		return new STTriangleMesh(original);
	}
};
