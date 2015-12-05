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
void QuadricErrorSimplification::generateQMatrices(STTriangleMesh* m, STVector3* v){
}
  
/**Finds the cost of contracting two vertices together based on the
   location of the new vertex (w), and the Q matrixes of the previous
   two vectors -Nisarg*/
float QuadricErrorSimplification::computeCost(STTriangleMesh* m, STVector3* w, STMatrix4* Q1, STMatrix4* Q2){
  return 0;
}
  

/**Helper function: are two vectors the same?*/
boolean vectorEquals(STVector3* a, STVector3* b){
  return a->x==b->x && a->y == b->y && a->z == b->z;
}

/**Helper function: do two edges describing the same thing?
 * (cost is not considered, nor is the order of a and b)*/
boolean edgeEquals(Edge a, Edge b){
  return (vectorEquals(a.vertex1,b.vertex1) && vectorEquals(a.vertex2,b.vertex2))||
    (a.vertex1 == b.vertex2 && a.vertex2==b.vertex1);
}

/**Helper function: where is this edge in `edges`?
 * returns -1 if this edge is not found*/
int findEdge(Edge a){
  int i;
  for (i=0;i<edges.size();i++){
    if (edgeEquals(a,edges[i])){
      return i;
    }
  }
  return -1;
}

/**Fills `edges` with all of the edges in the provided mesh, computes their
cost (with computeCost), then sorts them. -CJ*/
void QuadricErrorSimplification::populateEdges(STTriangleMesh* m){
  int i,j;
  Vector3* v1,v2;
  for (i=0; i < m->mFaces->size(); i++){
    for (j=0; j<3; j++){
      v1=m->mFaces[i]->v[j];
      v2=m->mFaces[i]->v[(j+1)%3];
      if (findEdge(Edge(v1,v2)) == -1){
	edges.pushBack(Edge(v1,v2));
      }
    }
  }
  float cost;
  for (i=0;i<edges.size();i++){
    //cost = computeCost(m, );
  }
}


