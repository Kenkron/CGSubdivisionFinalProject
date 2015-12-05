 /*
 * QuadricErrorSimplification.cpp
 *
 *  Created on: Nov 24, 2015
 *      Author: kenkron
 */

#include "QuadricErrorSimplification.h"
#include <algorithm>
#include <vector>

QuadricErrorSimplification::QuadricErrorSimplification() {
	// TODO Auto-generated constructor stub

}

QuadricErrorSimplification::~QuadricErrorSimplification() {
	// TODO Auto-generated destructor stub
}

/**Populates qMatrixes based on the planes surrounding each vertex
   in the mesh m - Ankit*/
void QuadricErrorSimplification::generateQMatrices(STTriangleMesh* m){
}
  
/**Finds the cost of contracting two vertices together based on the
   location of the new vertex (w), and the Q matrixes of the previous
   two vectors -Nisarg*/
float QuadricErrorSimplification::computeCost(STTriangleMesh* m, STVector3* w, STMatrix4* Q1, STMatrix4* Q2){
  return 0;
}
  

/**Helper function: are two vertices in the same position*/
bool vertexEquals(STVertex* a, STVertex* b){
  return a->pt.x==b->pt.x && a->pt.y == b->pt.y && a->pt.z == b->pt.z;
}

/**Helper function: do two edges describing the same thing?
 * (cost is not considered, nor is the order of a and b)*/
bool edgeEquals(Edge a, Edge b){
  return (vertexEquals(a.vertex1,b.vertex1) && vertexEquals(a.vertex2,b.vertex2))||
    (vertexEquals(a.vertex1,b.vertex2) && vertexEquals(a.vertex2,b.vertex1));
}

/**Helper function: where is edge a in `edges`?
 * returns -1 if this edge is not found*/
int findEdge(std::vector<Edge> edges, Edge a){
  int i;
  for (i=0;i<edges.size();i++){
    if (edgeEquals(a,edges[i])){
      return i;
    }
  }
  return -1;
}

/**Finds the index of a given verted in the mVertices list of m.
 * If no matching vertex is found, returns -1 */
int findVertex(STTriangleMesh* m, STVertex* v){
  int i;
  for (i=0; i < m->mVertices.size(); i++){
    if (vertexEquals(m->mVertices[i], v)){
      return i;
    }
  }
  return -1;
}

/**Fills `edges` with all of the edges in the provided mesh, computes their
cost (with computeCost), then sorts them. -CJ*/
void QuadricErrorSimplification::populateEdges(STTriangleMesh* m){
  
  //Step 1: Find all the edges
  int i,j;
  STVertex* v1;
  STVertex* v2;
  for (i=0; i < m->mFaces.size(); i++){
    for (j=0; j<3; j++){
      v1=m->mFaces[i]->v[j];
      v2=m->mFaces[i]->v[(j+1)%3];
      if (findEdge(edges,Edge(v1,v2)) == -1){
	edges.push_back(Edge(v1,v2));
      }
    }
  }

  //Step 2: Compute all of their costs
  float cost;
  int i1,i2;
  STVector3 midpoint=STVector3();
  STMatrix4* Q1;
  STMatrix4* Q2;
  for (i=0;i<edges.size();i++){
    midpoint.x = (edges[i].vertex1->pt.x+edges[i].vertex2->pt.x)/2;
    midpoint.y = (edges[i].vertex1->pt.y+edges[i].vertex2->pt.y)/2;
    midpoint.z = (edges[i].vertex1->pt.z+edges[i].vertex2->pt.z)/2;
    i1 = findVertex(m, edges[i].vertex1);
    i2 = findVertex(m, edges[i].vertex2);
    if (i1==-1 || i2==-1){
      //vertices couldn't be found.  Something is wrong.
      
    }
    Q1 = qMatrixes[i1];
    Q2 = qMatrixes[i2];
    edges[i].cost = computeCost(m, &midpoint, Q1, Q2);
  }
  
  //Step 3: Sort them
  std::sort(edges.begin(),edges.end());
}

