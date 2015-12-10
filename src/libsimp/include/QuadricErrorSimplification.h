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
#include <algorithm>
#include <vector>


struct Edge{
  Edge(STVertex* v1,STVertex* v2){
    vertex1=v1;
    vertex2=v2;
  }
  /**Comparison operator used by std::sort*/
  friend bool operator<(const Edge& a, const Edge& b) {
    return a.cost<b.cost;
  }
  STVertex* vertex1;
  STVertex* vertex2;  
  float cost;
};
  
class QuadricErrorSimplification: public Simplification {
 public:
  QuadricErrorSimplification();
  virtual ~QuadricErrorSimplification();
  
  /**Stores the Q matrix for each of the vectors in the STTriangleMesh
   * The Q matrix at qMatrixes[i] should corrospond to mVertices[i]*/
  std::vector<STMatrix4*> qMatrixes;
  std::vector<Edge> edges;

  /**Populates qMatrixes based on the planes surrounding each vertex
     in the mesh m*/
  void generateQMatrices(STTriangleMesh* m);
  
  /**Finds the cost of contracting two vertices together based on the
     location of the new vertex (w), and the Q matrixes of the previous
	 two vectors*/
  float computeCost(STTriangleMesh* m, STVertex* a, STVertex* b, STVertex* w, STMatrix4* Q1, STMatrix4* Q2);
  
  /**Fills `edges` with all of the edges in the provided mesh, computes their
   cost (with computeCost), then sorts them.*/
  void populateEdges(STTriangleMesh* m);

  /**This function decimates an edge of m by combining vertexes 1 and 2.
     This function should also update the mVertices, mNormals, mTexPos, and mFaces
     inside of m.  It must also modify the Q matrixes vector, as well as the indices
     of edges.*/
  void contractEdge(STTriangleMesh* m, Edge e);
  
};

#endif /* SRC_LIBSIMP_QUADRICERRORSIMPLIFICATION_H_ */
