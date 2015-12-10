 /*
 * QuadricErrorSimplification.cpp
 *
 *  Created on: Nov 24, 2015
 *      Author: kenkron
 */

#include "QuadricErrorSimplification.h"
#include <algorithm>
#include <vector>
#include <stdio.h>

QuadricErrorSimplification::QuadricErrorSimplification() {
         // TODO Auto-generated constructor stub

}

QuadricErrorSimplification::~QuadricErrorSimplification() {
	// TODO Auto-generated destructor stub
}

/**Helper function: are two vertexes the same?*/
bool vertexEquals(STVertex* a, STVertex* b){
  return a->pt.x==b->pt.x && a->pt.y == b->pt.y && a->pt.z == b->pt.z;
}

/**Helper function: do two edges describing the same thing?
 * (cost is not considered, nor is the order of a and b)*/
bool edgeEquals(Edge* a, Edge* b){
  return (vertexEquals(a->vertex1,b->vertex1) && vertexEquals(a->vertex2,b->vertex2))||
    (vertexEquals(a->vertex1,b->vertex2) && vertexEquals(a->vertex2,b->vertex1));
}
/*Helper Function: where is the vertex in 'vertiexes'?
returns -1 if this vertex is not found
*/
int findVertex(STTriangleMesh* mesh, STVertex* a){
	int i;
	for (i = 0; i<mesh->mVertices.size(); i++){
		STVertex* b = mesh->mVertices[i];
		if (a->pt.x==b->pt.x && a->pt.y==b->pt.y && a->pt.z==b->pt.z){
			return i;
		}
	}
	return -1;
}

/**Helper function: where is this edge in `edges`?
 * returns -1 if this edge is not found*/
int findEdge(QuadricErrorSimplification* me, Edge* a){
  int i;
  for (i=0;i<me->edges.size();i++){
    if (edgeEquals(a,me->edges[i])){
      return i;
    }
  }
  return -1;
}

STVector3 vertex2Vector3(STVertex* v){
  return (STVector3(v->pt.x,v->pt.y,v->pt.z));
}

/**Populates qMatrixes based on the planes surrounding each vertex
   in the mesh m - Ankit*/
void QuadricErrorSimplification::generateQMatrices(STTriangleMesh* mesh){
	STVector3 i, j, k;
	STVector3 n;
	float d = 0;
	float area = 0;
	STFace* f;
	STVertex* vert;
	for (size_t v = 0; v < mesh->mVertices.size(); ++v)
	{
	        //memset(m_vertices[v].m_Q, 0, 10 * sizeof(Float));
	        vert = mesh->mVertices[v];
		//find all the faces that contain this vertex
		for (size_t itT = 0; itT < mesh->mFaces.size(); ++itT)
		{
		        f = mesh->mFaces[itT];
			if (vertexEquals(f->v[0], vert) || vertexEquals(f->v[1], vert) || vertexEquals(f->v[2], vert)){
			  i = vertex2Vector3(f->v[0]);
			  j = vertex2Vector3(f->v[1]);
			  k = vertex2Vector3(f->v[2]);
			  n = STVector3::Cross(j - i, k - i);
			  area = n.Length();
			  n.Normalize();
			  d = -(STVector3::Dot(vertex2Vector3(mesh->mVertices[v]), n));
			  
			  qMatrixes[v]->table[0][0] += area * (n.x * n.x);
			  qMatrixes[v]->table[0][1] += area * (n.x * n.y);
			  qMatrixes[v]->table[0][2] += area * (n.x * n.z);
			  qMatrixes[v]->table[0][3] += area * (n.x * d);
			  qMatrixes[v]->table[1][0] += area * (n.y * n.x);
			  qMatrixes[v]->table[1][1] += area * (n.y * n.y);
			  qMatrixes[v]->table[1][2] += area * (n.y * n.z);
			  qMatrixes[v]->table[1][3] += area * (n.y * d);
			  qMatrixes[v]->table[2][0] += area * (n.z * n.x);
			  qMatrixes[v]->table[2][1] += area * (n.z * n.y);
			  qMatrixes[v]->table[2][2] += area * (n.z * n.z);
			  qMatrixes[v]->table[2][3] += area * (n.z * d);
			  qMatrixes[v]->table[3][0] += area * (d * n.x);
			  qMatrixes[v]->table[3][1] += area * (d * n.y);
			  qMatrixes[v]->table[3][2] += area * (d * n.z);
			  qMatrixes[v]->table[3][3] += area * (d * d);
			}
		}
	}
}

/**Finds the cost of contracting two vertices together based on the
   location of the new vertex (w), and the Q matrixes of the previous
   two vectors -Nisarg*/
float QuadricErrorSimplification::computeCost(STTriangleMesh* m, STVertex* a, STVertex* b, STVertex* w, STMatrix4* Q1, STMatrix4* Q2){
  float sum_Q[3][4];
  float temp_mat[3][4];
  float Q15 = Q1->table[4][4] + Q2->table[4][4]; // m_vertices[v2].m_Q[9] += area * (d * d); in the reference code
  STVector3* temp;
  sum_Q[0][0] = Q1->table[0][0] + Q2->table[0][0];
  sum_Q[0][1] = Q1->table[0][1] + Q2->table[0][1];
  sum_Q[0][2] = Q1->table[0][2] + Q2->table[0][2];   
  sum_Q[0][3] = Q1->table[0][3] + Q2->table[0][3];   
  sum_Q[1][0] = Q1->table[1][0] + Q2->table[1][0];   
  sum_Q[1][1] = Q1->table[1][1] + Q2->table[1][1];   
  sum_Q[1][2] = Q1->table[1][2] + Q2->table[1][2];   
  sum_Q[1][3] = Q1->table[1][3] + Q2->table[1][3];   
  sum_Q[2][0] = Q1->table[2][0] + Q2->table[2][0];   
  sum_Q[2][1] = Q1->table[2][1] + Q2->table[2][1];   
  sum_Q[2][2] = Q1->table[2][2] + Q2->table[2][2];   
  sum_Q[2][3] = Q1->table[2][3] + Q2->table[2][3];  
  float determinant =   sum_Q[0][0] * sum_Q[1][1] * sum_Q[2][1] 
     + sum_Q[0][1] * sum_Q[1][2] * sum_Q[2][0] 
     + sum_Q[0][2] * sum_Q[1][0] * sum_Q[2][1]   
     - sum_Q[0][0] * sum_Q[1][2] * sum_Q[2][1]  
     - sum_Q[0][1] * sum_Q[1][0] * sum_Q[2][2]
    - sum_Q[0][2] * sum_Q[1][1] * sum_Q[2][0]; 
  if(determinant != 0.0)
    {
      w->pt.x = (1/determinant)*  (    sum_Q[0][1]*sum_Q[1][3]*sum_Q[2][2]
				      + sum_Q[0][2]*sum_Q[1][1]*sum_Q[2][3] 
				      + sum_Q[0][3]*sum_Q[1][2]*sum_Q[2][1] 
				      - sum_Q[0][1]*sum_Q[1][2]*sum_Q[2][3] 
				      - sum_Q[0][2]*sum_Q[1][3]*sum_Q[2][1]  
				   - sum_Q[0][3]*sum_Q[1][1]*sum_Q[2][2]);
      w->pt.y = (1/determinant)*  (    sum_Q[0][0]*sum_Q[1][2]*sum_Q[2][3] 
				      + sum_Q[0][2]*sum_Q[1][3]*sum_Q[2][0]  
				      + sum_Q[0][3]*sum_Q[1][0]*sum_Q[2][2]
				      - sum_Q[0][0]*sum_Q[1][3]*sum_Q[2][2] 
				      - sum_Q[0][2]*sum_Q[1][0]*sum_Q[2][3] 
				   - sum_Q[0][3]*sum_Q[1][2]*sum_Q[2][0]);
      w->pt.z = (1/determinant)*  (    sum_Q[0][0]*sum_Q[1][3]*sum_Q[2][1]  
				      + sum_Q[0][1]*sum_Q[1][0]*sum_Q[2][3] 
				      + sum_Q[0][3]*sum_Q[1][1]*sum_Q[2][0] 
				      - sum_Q[0][0]*sum_Q[1][1]*sum_Q[2][3] 
				      - sum_Q[0][1]*sum_Q[1][3]*sum_Q[2][0]  
				   - sum_Q[0][3]*sum_Q[1][0]*sum_Q[2][1]);
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
  
  float qem = w->pt.x  * (sum_Q[0][0] * w->pt.x + sum_Q[0][1] * w->pt.y + sum_Q[0][2] * w->pt.z + sum_Q[0][3]) +  
    w->pt.y * (sum_Q[0][1] * w->pt.x + sum_Q[1][1] * w->pt.y + sum_Q[1][2] * w->pt.z + sum_Q[1][3]) +
    w->pt.z * (sum_Q[0][2] * w->pt.x + sum_Q[1][2] * w->pt.y + sum_Q[2][2] * w->pt.z + sum_Q[2][3]) +
    (sum_Q[0][3] * w->pt.x + sum_Q[1][3] * w->pt.y + sum_Q[2][3] * w->pt.z + Q15) ;

  return qem;
}

/**Fills `edges` with all of the edges in the provided mesh, computes their
cost (with computeCost), then sorts them. -CJ*/
void QuadricErrorSimplification::populateEdges(STTriangleMesh* m){
  int i,j;
  STVertex *v1,*v2;
  for (i=0; i < m->mFaces.size(); i++){
    for (j=0; j<3; j++){
      v1=m->mFaces[i]->v[j];
      v2=m->mFaces[i]->v[(j+1)%3];
      Edge newEdge  = Edge(v1,v2);
      if (findEdge(this,&newEdge) == -1){
	edges.push_back(new Edge(v1,v2));
      }
    }
  }
  int i1,i2;
  STVertex w = STVertex(0,0,0,0,0);
  STMatrix4* Q1;
  STMatrix4* Q2;
  for (i=0;i<edges.size();i++){
    i1 = findVertex(m, edges[i]->vertex1);
    i2 = findVertex(m, edges[i]->vertex2);
    if (i1==-1){
      printf("Error: QuadricErrorSimplification::populateEdges could not find vertex at %f,%f,%f\n",
	     edges[i]->vertex1->pt.x,edges[i]->vertex1->pt.y,edges[i]->vertex1->pt.z);
    }
    if (i2==-1){
      printf("Error: QuadricErrorSimplification::populateEdges could not find vertex at %f,%f,%f\n",
	     edges[i]->vertex2->pt.x,edges[i]->vertex2->pt.y,edges[i]->vertex2->pt.z);
    }
    Q1 = qMatrixes[i1];
    Q2 = qMatrixes[i2];
    edges[i]->cost = computeCost(m, v1, v2, &w, Q1, Q2);
  }
}

void QuadricErrorSimplification::contractEdge(STTriangleMesh* m, Edge* e){
  //find the indexes of the vertices
  int i1 = findVertex(m, e->vertex1);
  int i2 = findVertex(m, e->vertex2);
  if (i1==-1){
      printf("Error: QuadricErrorSimplification::contractEdges could not find vertex at %f,%f,%f\n",
	     e->vertex1->pt.x,e->vertex1->pt.y,e->vertex1->pt.z);
  }
  if (i2==-1){
      printf("Error: QuadricErrorSimplification::contractEdges could not find vertex at %f,%f,%f\n",
	     e->vertex2->pt.x,e->vertex2->pt.y,e->vertex2->pt.z);
  }
  
  STMatrix4* Q1 = qMatrixes[i1];
  STMatrix4* Q2 = qMatrixes[i2];
  
  STVertex *a = m->mVertices[i1];
  STVertex *b = m->mVertices[i2];
  STVertex *w = new STVertex(0,0,0,0,0);
  computeCost(m, a, b, w, Q1, Q2); 

  //remove unnecessary faces:
  for(int itr=0; itr != m->mFaces.size(); ++itr)
    {
      int vertexesReplaced = 0;
      if(vertexEquals(m->mFaces[itr]->v[0],a) ||  vertexEquals(m->mFaces[itr]->v[0],b))
	{
	  m->mFaces[itr]->v[0] = w;
	  vertexesReplaced++;
	}
      if(vertexEquals(m->mFaces[itr]->v[1],a) ||  vertexEquals(m->mFaces[itr]->v[1],b))
	{
	  m->mFaces[itr]->v[1] = w;
	  vertexesReplaced++;
	}
      if(vertexEquals(m->mFaces[itr]->v[2],a) ||  vertexEquals(m->mFaces[itr]->v[2],b))
	{
	  m->mFaces[itr]->v[2] = w;
	  vertexesReplaced++;
	}
      //if a face needed to chance 2 points, that face has been eliminated
      if (vertexesReplaced>1){
	STFace* removed =  m->mFaces[itr];
	m->mFaces.erase(m->mFaces.begin()+itr);
	delete removed;
	itr--;
      }
    }

  //remove v1, v2, Q1, and Q2, and add w and sum_Q
  STMatrix4* sum_Q = new STMatrix4(); 
  sum_Q->table[0][0] = Q1->table[0][0] + Q2->table[0][0];
  sum_Q->table[0][1] = Q1->table[0][1] + Q2->table[0][1];
  sum_Q->table[0][2] = Q1->table[0][2] + Q2->table[0][2];   
  sum_Q->table[0][3] = Q1->table[0][3] + Q2->table[0][3];   
  sum_Q->table[1][0] = Q1->table[1][0] + Q2->table[1][0];   
  sum_Q->table[1][1] = Q1->table[1][1] + Q2->table[1][1];   
  sum_Q->table[1][2] = Q1->table[1][2] + Q2->table[1][2];   
  sum_Q->table[1][3] = Q1->table[1][3] + Q2->table[1][3];   
  sum_Q->table[2][0] = Q1->table[2][0] + Q2->table[2][0];   
  sum_Q->table[2][1] = Q1->table[2][1] + Q2->table[2][1];   
  sum_Q->table[2][2] = Q1->table[2][2] + Q2->table[2][2];   
  sum_Q->table[2][3] = Q1->table[2][3] + Q2->table[2][3];
  
  //The order the vertexes and Qs are removed is important:
  //the first removal may affect the second
  int max = std::max(i1,i2);
  int min = std::min(i1,i2);
  m->mVertices.erase(m->mVertices.begin()+max);
  m->mVertices.erase(m->mVertices.begin()+min);
  delete a;
  delete b;
  qMatrixes.erase(qMatrixes.begin()+max);
  qMatrixes.erase(qMatrixes.begin()+min);
  delete Q1;
  delete Q2;

  //We must also delete this edge
  int ie = findEdge(this, e);
  e = edges[ie];
  edges.erase(edges.begin()+ie);
  delete e;
  
  m->mVertices.push_back(w);
  qMatrixes.push_back(sum_Q);
}

STTriangleMesh* QuadricErrorSimplification::simplify(STTriangleMesh* original, int maxTriangles){
    populateEdges(original);
    while (original->mFaces.size()>maxTriangles){
      contractEdge(original,edges[1]);
      std::sort(edges.begin(), edges.end());
    }
    return original;
}
