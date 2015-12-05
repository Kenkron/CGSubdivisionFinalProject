/*
 * QuadricErrorSimplification.h
 *
 *  Created on: Nov 24, 2015
 *      Author: kenkron
 */

#ifndef SRC_LIBSIMP_QUADRICERRORSIMPLIFICATION_H_
#define SRC_LIBSIMP_QUADRICERRORSIMPLIFICATION_H_

#include "Simplification.h"
#include "STTriangleMesh.h"
#include "STMatrix4.h"
#include "STVector3.h"

struct Edge{
  Edge(int v1,int v2){
  }
  int vertex1, vertex2;  
  float cost;
}
  
class QuadricErrorSimplification: public Simplification {
 public:
  QuadricErrorSimplification();
  virtual ~QuadricErrorSimplification();
  std::vector<STMatrix4*> qMatrixes;
  std::vector<Edge> edges;

  /**Populates qMatrixes based on the planes surrounding each vertex
     in the mesh m*/
  void generateQMatrices(STTriangleMesh* m, STVector3* v);
  
  /**Finds the cost of contracting two vertices together based on the
     location of the new vertex (w), and the Q matrixes of the previous
	 two vectors*/
  float computeCost(STTriangleMesh* m, STVector3* w, STMatrix4* Q1, STMatrix4* Q2);
  
  /**Fills `edges` with all of the edges in the provided mesh, computes their
   cost (with computeCost), then sorts them.*/
  void populateEdges(STTriangleMesh* m);

  /**This function decimates an edge of m by combining vertexes 1 and 2.
     This function should also update the mVertices, mNormals, mTexPos, and mFaces
     inside of m.  It must also modify the Q matrixes vector, as well as the indices
     of edges.*/
  void contractEdge(STTriangleMesh* m, int vertex1, int vertex2);
  
};

#endif /* SRC_LIBSIMP_QUADRICERRORSIMPLIFICATION_H_ */
