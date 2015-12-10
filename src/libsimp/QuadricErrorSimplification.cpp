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
void QuadricErrorSimplification::generateQMatrices(STTriangleMesh* mesh){

	STVertex coordMin = mesh->mVertices[0];
	STVertex coordMax = mesh->mVertices[0];
	STVertex coord;
	for (size_t p = 1; p < mesh.mVertices.size(); ++p)
	{
		coord = mesh->mVertices[p];
		if (coordMin.x > coord.x) coordMin.x = coord.x;
		if (coordMin.y > coord.y) coordMin.y = coord.y;
		if (coordMin.z > coord.z) coordMin.z = coord.z;
		if (coordMax.x < coord.x) coordMax.x = coord.x;
		if (coordMax.y < coord.y) coordMax.y = coord.y;
		if (coordMax.z < coord.z) coordMax.z = coord.z;
	}
	coordMax -= coordMin;
	m_diagBB = coordMax.GetNorm();

	STVector3 i, j, k;
	STVector3 n;
	Float d = 0;
	Float area = 0;
	for (size_t v = 0; v < mesh.mVertices.size(); ++v)
	{
	//	memset(m_vertices[v].m_Q, 0, 10 * sizeof(Float));
		
		for (size_t itT = 0; itT < mesh.mVertices[v].faces.size(); ++itT)
		{
			idTriangle = mesh.mVertices[v].faces[itT];
			i = mesh.mVertices[v].faces[itT].v[0];
			j = mesh.mVertices[v].faces[itT].v[1];
			k = mesh.mVertices[v].faces[itT].v[2];
			n = (j - i) ^ (k - i);
			area = n.Length();
			n.Normalize();
			d = -(mesh.mVertices[v] * n);

			mesh.mVertices[v].QMatrix[0][0] += area * (n.x * n.x);
			mesh.mVertices[v].QMatrix[0][1] += area * (n.x * n.y);
			mesh.mVertices[v].QMatrix[0][2] += area * (n.x * n.z);
			mesh.mVertices[v].QMatrix[0][3] += area * (n.x * d);
			mesh.mVertices[v].QMatrix[1][0] += area * (n.y * n.x);
			mesh.mVertices[v].QMatrix[1][1] += area * (n.y * n.y);
			mesh.mVertices[v].QMatrix[1][2] += area * (n.y * n.z);
			mesh.mVertices[v].QMatrix[1][3] += area * (n.y * d);
			mesh.mVertices[v].QMatrix[2][0] += area * (n.z * n.x);
			mesh.mVertices[v].QMatrix[2][1] += area * (n.z * n.y);
			mesh.mVertices[v].QMatrix[2][2] += area * (n.z * n.z);
			mesh.mVertices[v].QMatrix[2][3] += area * (n.z * d);
			mesh.mVertices[v].QMatrix[3][0] += area * (d * n.x);
			mesh.mVertices[v].QMatrix[3][1] += area * (d * n.y);
			mesh.mVertices[v].QMatrix[3][2] += area * (d * n.z);
			mesh.mVertices[v].QMatrix[3][3] += area * (d * d);
			
		}
	}
	STVector3 u1, u2;
	const Float w = static_cast<Float>(1000);
	int t, v1, v2, v3;
	for (size_t e = 0; e < mesh.mEdges.size(); ++e)
	{
		v1 = mesh.mEdges[e].v[0];
		v2 = mesh.mEdges[e].v[1];
		t = IsBoundaryEdge(v1, v2);
		if (t != -1)
		{
			if (mesh.mFaces[t].v[0] != v1 && mesh.mFaces[t].v[0] != v2) v3 = mesh.mFaces[t].v[0];
			else if (mesh.mFaces[t].v[1] != v1 && mesh.mFaces[t].v[1] != v2) v3 = mesh.mFaces[t].v[1];
			else                                                           v3 = mesh.mFaces[t].v[2];
			u1 = mesh.mVertices[v2] - mesh.mVertices[v1];
			u2 = mesh.mVertices[v3] - mesh.mVertices[v1];
			area = w * (u1^u2).GetNorm();
			u1.Normalize();
			n = u2 - (u2 * u1) * u1;
			n.Normalize();

			d = -(mesh.mVertices[v1] * n);
			mesh.mVertices[v].QMatrix[0][0] += area * (n.x * n.x);
			mesh.mVertices[v].QMatrix[0][1] += area * (n.x * n.y);
			mesh.mVertices[v].QMatrix[0][2] += area * (n.x * n.z);
			mesh.mVertices[v].QMatrix[0][3] += area * (n.x * d);
			mesh.mVertices[v].QMatrix[1][0] += area * (n.y * n.x);
			mesh.mVertices[v].QMatrix[1][1] += area * (n.y * n.y);
			mesh.mVertices[v].QMatrix[1][2] += area * (n.y * n.z);
			mesh.mVertices[v].QMatrix[1][3] += area * (n.y * d);
			mesh.mVertices[v].QMatrix[2][0] += area * (n.z * n.x);
			mesh.mVertices[v].QMatrix[2][1] += area * (n.z * n.y);
			mesh.mVertices[v].QMatrix[2][2] += area * (n.z * n.z);
			mesh.mVertices[v].QMatrix[2][3] += area * (n.z * d);
			mesh.mVertices[v].QMatrix[3][0] += area * (d * n.x);
			mesh.mVertices[v].QMatrix[3][1] += area * (d * n.y);
			mesh.mVertices[v].QMatrix[3][2] += area * (d * n.z);
			mesh.mVertices[v].QMatrix[3][3] += area * (d * d);

			d = -(m_points[v2] * n);
			mesh.mVertices[v].QMatrix[0][0] += area * (n.x * n.x);
			mesh.mVertices[v].QMatrix[0][1] += area * (n.x * n.y);
			mesh.mVertices[v].QMatrix[0][2] += area * (n.x * n.z);
			mesh.mVertices[v].QMatrix[0][3] += area * (n.x * d);
			mesh.mVertices[v].QMatrix[1][0] += area * (n.y * n.x);
			mesh.mVertices[v].QMatrix[1][1] += area * (n.y * n.y);
			mesh.mVertices[v].QMatrix[1][2] += area * (n.y * n.z);
			mesh.mVertices[v].QMatrix[1][3] += area * (n.y * d);
			mesh.mVertices[v].QMatrix[2][0] += area * (n.z * n.x);
			mesh.mVertices[v].QMatrix[2][1] += area * (n.z * n.y);
			mesh.mVertices[v].QMatrix[2][2] += area * (n.z * n.z);
			mesh.mVertices[v].QMatrix[2][3] += area * (n.z * d);
			mesh.mVertices[v].QMatrix[3][0] += area * (d * n.x);
			mesh.mVertices[v].QMatrix[3][1] += area * (d * n.y);
			mesh.mVertices[v].QMatrix[3][2] += area * (d * n.z);
			mesh.mVertices[v].QMatrix[3][3] += area * (d * d);
		}
	}
}

bool ManifoldConstraint(STTriangleMesh* mesh, STPoint3 v1, STPoint3 v2) const
{
	std::set<STPoint3> vertices;
	STPoint3 a, b;
	int a1 = findVertex(mesh, v1);
	int a2 = findVertex(mesh, v2);
	int idEdge1;
	int idEdge2;
	int idEdgeV1V2;
	for (size_t itE1 = 0; itE1 < mesh.mVertices[a1].edges.size(); ++itE1)
	{
		//idEdge1 = m_vertices[v1].m_edges[itE1];
		a = (mesh.mVertices[a1].edges[itE1].v[0] == v1) ? mesh.mVertices[a1].edges[itE1].v[1] : mesh.mVertices[a1].edges[itE1].v[0];
		vertices.insert(a);
		if (a != v2)
		{
			for (size_t itE2 = 0; itE2 < mesh.mVertices[a2].edges.size(); ++itE2)
			{
				idEdge2 = mesh.mVertices[a2].edges[itE2];
				b = (m_edges[idEdge2].m_v1 == v2) ? m_edges[idEdge2].m_v2 : m_edges[idEdge2].m_v1;
				vertices.insert(b);
				if (a == b)
				{
					if (GetTriangle(v1, v2, a) == -1)
					{
						return false;
					}
				}
			}
		}
		else
		{
			idEdgeV1V2 = idEdge1;
		}
	}
	if (vertices.size() <= 4 || (m_vertices[v1].m_onBoundary && m_vertices[v2].m_onBoundary && !m_edges[idEdgeV1V2].m_onBoundary))
	{
		return false;
	}
	return true;
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
  

/* Helper function to get a list of triangles for given vertex.
*/

void QuadricErrorSimplification::listTriangles(STTriangleMesh* m, STVector3* v, STVector3* a)
{




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
/*Helper Function: where is the vertex in 'vertices'?
returns -1 if this vertex is not found
*/
int findVertex(STTriangle* mesh, STPoint3 a){
	int i;
	for (i = 0; i<mesh.mVertices.size(); i++){
		STPoint3 b = mesh.mVertices[i];
		if (a.x==b.x && a.y==b.y && a.z==b.z){
			return i;
		}
	}
	return -1;
}



/**Helper function: where is this edge in `edges`?
 * returns -1 if this edge is not found*/
int findEdge(Edge a, STEdge ){
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
  int i1,i2;
  STVector3* w = new STVector3();
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
