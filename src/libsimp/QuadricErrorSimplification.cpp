 /*
 * QuadricErrorSimplification.cpp
 *
 *  Created on: Nov 24, 2015
 *      Author: kenkron
 */

#include "QuadricErrorSimplification.h"
#include <algorithm>
#include <vector>
#include <stdout>

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
float QuadricErrorSimplification::computeCost(STTriangleMesh* m, STVertex* a, STVertex* b, STVertex* w, STMatrix4* Q1, STMatrix4* Q2){
  float sum_Q[3][4];
  float temp_mat[3][4];
  float Q15 = Q1[4][4] + Q2[4][4]; // m_vertices[v2].m_Q[9] += area * (d * d); in the reference code
  STVector3* temp;
  sum_Q[0][0] = Q1[0][0] + Q2[0][0];
  sum_Q[0][1] = Q1[0][1] + Q2[0][1];
  sum_Q[0][2] = Q1[0][2] + Q2[0][2];   
  sum_Q[0][3] = Q1[0][3] + Q2[0][3];   
  sum_Q[1][0] = Q1[1][0] + Q2[1][0];   
  sum_Q[1][1] = Q1[1][1] + Q2[1][1];   
  sum_Q[1][2] = Q1[1][2] + Q2[1][2];   
  sum_Q[1][3] = Q1[1][3] + Q2[1][3];   
  sum_Q[2][0] = Q1[2][0] + Q2[2][0];   
  sum_Q[2][1] = Q1[2][1] + Q2[2][1];   
  sum_Q[2][2] = Q1[2][2] + Q2[2][2];   
  sum_Q[2][3] = Q1[2][3] + Q2[2][3];  
  float determinant =   sum_Q[0][0] * sum_Q[1][1] * sum_Q[2][1] 
     + sum_Q[0][1] * sum_Q[1][2] * sum_Q[2][0] 
     + sum_Q[0][2] * sum_Q[1][0] * sum_Q[2][1]   
     - sum_Q[0][0] * sum_Q[1][2] * sum_Q[2][1]  
     - sum_Q[0][1] * sum_Q[1][0] * sum_Q[2][2]
    - sum_Q[0][2] * sum_Q[1][1] * sum_Q[2][0]; 
  if(determinant != 0.0)
    {
      w->pt.x = (1/determinant)*  (    sum_q[0][1]*sum_q[1][3]*sum_q[2][2]
				      + sum_q[0][2]*sum_q[1][1]*sum_q[2][3] 
				      + sum_q[0][3]*sum_q[1][2]*sum_q[2][1] 
				      - sum_q[0][1]*sum_q[1][2]*sum_q[2][3] 
				      - sum_q[0][2]*sum_q[1][3]*sum_q[2][1]  
				   - sum_q[0][3]*sum_q[1][1]*sum_q[2][2]);
      w->pt.y = (1/determinant)*  (    sum_q[0][0]*sum_q[1][2]*sum_q[2][3] 
				      + sum_q[0][2]*sum_q[1][3]*sum_q[2][0]  
				      + sum_q[0][3]*sum_q[1][0]*sum_q[2][2]
				      - sum_q[0][0]*sum_q[1][3]*sum_q[2][2] 
				      - sum_q[0][2]*sum_q[1][0]*sum_q[2][3] 
				   - sum_q[0][3]*sum_q[1][2]*sum_q[2][0]);
      w->pt.z = (1/determinant)*  (    sum_q[0][0]*sum_q[1][3]*sum_q[2][1]  
				      + sum_q[0][1]*sum_q[1][0]*sum_q[2][3] 
				      + sum_q[0][3]*sum_q[1][1]*sum_q[2][0] 
				      - sum_q[0][0]*sum_q[1][1]*sum_q[2][3] 
				      - sum_q[0][1]*sum_q[1][3]*sum_q[2][0]  
				   - sum_q[0][3]*sum_q[1][0]*sum_q[2][1]);
      //w.x = static_cast<Float>(temp.x);
      //w.y = static_cast<Float>(temp.y);
      //w.z = static_cast<Float>(temp.z);
    }
  else
    {
      w->pt.x = (a->pt.x + b->pt.x)/2;
      w->pt.y = (a->pt.y + b->pt.y)/2;
      w->pt.z = (a->pt.z + b->pt.z)/2;
      //w.x = static_cast<double>(temp.x);
      //w.y = static_cast<double>(temp.y);
      //w.z = static_cast<double>(temp.z);
    }
  
  float qem = w->pt.x  * (sum_q[0][0] * w->pt.x + sum_q[0][1] * w->pt.y + sum_q[0][2] * w->pt.z + sum_q[0][3]) +  
    w->pt.y() * (sum_q[0][1] * w->pt.x + sum_q[1][1] * w->pt.y + sum_q[1][2] * w->pt.z + sum_q[1][3]) +
    w->pt.z() * (sum_q[0][2] * w->pt.x + sum_q[1][2] * w->pt.y + sum_q[2][2] * w->pt.z + sum_q[2][3]) +
    (sum_q[0][3] * w->pt.x + sum_q[1][3] * w->pt.y + sum_q[2][3] * w->pt.z + Q15) ;
  STVector3 a1,a2,norm1,norm2;
  std::vector<STFace> v_faces = a.faces;
  for(int itr=0; itr != b.faces.Size(); ++itr)
    {
      v_faces.push_back(b.faces[itr]);
    }
  for(int itr=0; itr != v_faces.Size(); ++itr)
    {
      a1 = STVector3(v_faces[itr].v2 - v_faces[itr].v1);
      a2 = STVector3(v_faces[itr].v3 - v_faces[itr].v2);
      norm1 = cross(a1,a2);
      STVector3 old_v1 = STVector3(v_faces[itr].v1);
      STVector3 old_v2 = STVector3(v_faces[itr].v2);
      STVector3 old_v3 = STVector3(v_faces[itr].v3);
      if(vectorEquals(v_faces[itr].v1,a) ||  vectorEquals(v_faces[itr].v1,b))
	{
	  v_faces[itr].v1 = w;
	}
      if(vectorEquals(v_faces[itr].v2,a) ||  vectorEquals(v_faces[itr].v2,b))
	{
	  v_faces[itr].v2 = w;
	}
      if(vectorEquals(v_faces[itr].v3,a) ||  vectorEquals(v_faces[itr].v3,b))
	{
	  v_faces[itr].v3 = w;
	}
      a1 = STVector3(v_faces[itr].v2 - v_faces[itr].v1);
      a2 = STVector3(v_faces[itr].v3 - v_faces[itr].v2);
      norm2 = cross(a1,a2);
      v_faces[itr].v1 = old_v1;
      v_faces[itr].v1 = old_v2;
      v_faces[itr].v1 = old_v3;
      norm1.Normalize();
      norm2.Normalize();
      if (n1*n2 < 0.0) 
        {
	  return std::numeric_limits<double>::max();
        }
    }
  if (!ManifoldConstraint(v1, v2))
    {
      return std::numeric_limits<double>::max();
    }
  return qem;
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
  STVector3* w=new STVector3();
  STMatrix4* Q1;
  STMatrix4* Q2;
  for (i=0;i<edges.size();i++){
    i1 = findVertex(m, edges[i].vertex1);
    i2 = findVertex(m, edges[i].vertex2);
    if (i1==-1){
      printf("Error: QuadricErrorSimplification::populateEdges could not find vertex at %d,%d,%d\n",
	     edges[i].vertex1.x,edges[i].vertex1.y,edges[i].vertex1.z);
    }
    if (i2==-1){
      printf("Error: QuadricErrorSimplification::populateEdges could not find vertex at %d,%d,%d\n",
	     edges[i].vertex2.x,edges[i].vertex2.y,edges[i].vertex2.z);
    }
    Q1 = qMatrixes[i1];
    Q2 = qMatrixes[i2];
    edges[i].cost = computeCost(m, &midpoint, Q1, Q2);
  }
  
  //Step 3: Sort them
  std::sort(edges.begin(),edges.end());
}

void QuadricErrorSimplification::contractEdge(STTriangleMesh* m, Edge e){
  //find the indexes of the vertices
  v1 = findVertex(m, e.vertex1);
  v2 = findVertex(m, e.vertex2);
  if (v1==-1){
      printf("Error: QuadricErrorSimplification::contractEdges could not find vertex at %d,%d,%d\n",
	     e.vertex1.x,e.vertex1.y,e.vertex1.z);
  }
  if (v2==-1){
      printf("Error: QuadricErrorSimplification::contractEdges could not find vertex at %d,%d,%d\n",
	     e.vertex2.x,e.vertex2.y,e.vertex2.z);
  }
  STMatrix4* Q1;
  STMatrix4* Q2;
}

STTriangleMesh* QuadricErrorSimplification::simplify(STTriangleMesh* original, int maxTriangles){
    populateEdges(original);
    while (original->mFaces.size()>maxTriangles){
      contractEdge(original,edges[1]);
    }
    return original;
}
