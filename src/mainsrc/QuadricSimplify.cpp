#include <stdio.h>
#include "STTriangleMesh.h"
#include "QuadricErrorSimplification.h"
int main(int narg, char* argv[]){
    if (narg == 3){
	std::string inputFile = std::string(argv[1]);
	std::vector<STTriangleMesh*> meshes;
	STTriangleMesh::LoadObj(meshes, inputFile);
	QuadricErrorSimplification* simplifier = new QuadricErrorSimplification();
	int i;
	for (i = 0; i<meshes.size();i++){
	    //cut the mesh size in half
	    simplifier->simplify(meshes[i],meshes[i]->mFaces.size()/2)->Write(argv[2]);
	}
    }else{
	printf("Requires input and output obj files.");
    }
}
