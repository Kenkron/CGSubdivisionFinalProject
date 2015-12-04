 /*
 * QuadricErrorSimplification.cpp
 *
 *  Created on: Nov 24, 2015
 *      Author: kenkron
 */

#include "QuadricErrorSimplification.h"

QuadricErrorSimplification::QuadricErrorSimplification() {
	// TODO Auto-generated constructor stub

}

QuadricErrorSimplification::~QuadricErrorSimplification() {
	// TODO Auto-generated destructor stub
}

  /**Populates qMatrixes based on the planes surrounding each vertex
     in the mesh m - Ankit*/
  void generateQMatrices(STMesh* m, STVector3* v);
  
  /**Finds the cost of contracting two vertices together based on the
     location of the new vertex (w), and the Q matrixes of the previous
	 two vectors -Nisarg*/
  float computeCost(STMesh* m, STVector3* w, STMatrix4* Q1, STMatrix4* Q2);
  
  /**Fills `edges` with all of the edges in the provided mesh, computes their
   cost (with computeCost), then sorts them. -CJ*/
  float populateEdges(STMesh* m);
