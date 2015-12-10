/*
 * simplification.h
 *
 *  Created on: Nov 24, 2015
 *      Author: kenkron
 */

#ifndef SRC_MAINSRC_SIMPLIFICATION_H_
#define SRC_MAINSRC_SIMPLIFICATION_H_

#include "STTriangleMesh.h"

/**Defines a method to simplify a complex mesh.*/
class Simplification{
public:

	Simplification();
	virtual ~Simplification();

	/**Given a mesh ($(original)) and a maximum number of triangles ($(maxTriangles)),
	 * this function will return a new mesh based off of the original mesh,
	 * but containing no more than $(maxTriangles) triangles.  $(original) will not
	 * be changed.*/
	virtual STTriangleMesh* simplify(STTriangleMesh* original, int maxTriangles);
};
#endif /* SRC_MAINSRC_SIMPLIFICATION_H_ */
