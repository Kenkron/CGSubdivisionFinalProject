// main.cpp

//
// For this project, we use OpenGL, GLUT
// and GLEW (to load OpenGL extensions)
//
#include "stglew.h"

#include <stdio.h>
#include <string.h>
#include <map>

#define ARRAY_SIZE(array) (sizeof((array))/sizeof((array[0])))
//
// Globals used by this application.
// As a rule, globals are Evil, but this is a small application
// and the design of GLUT makes it hard to avoid them.
//

int globalCount;

typedef struct {
	int i1;
	int i2;
	int i3;
} TriangleIndices;

// Window size, kept for screenshots
static int gWindowSizeX = 0;
static int gWindowSizeY = 0;

// File locations
std::string vertexShader;
std::string fragmentShader;
std::string normalMap;
std::string displacementMap;
std::string meshOBJ;

// Light source attributes
static float ambientLight[] = { 1.0f, 1.0f, 1.0f, 1.0 };
static float diffuseLight[] = { 1.00, 1.00, 1.00, 1.0 };
static float specularLight[] = { 1.00, 1.00, 1.00, 1.0 };

float lightPosition[] = { 10.0f, 15.0f, 10.0f, 1.0f };

STImage *surfaceNormImg;
STTexture *surfaceNormTex;

STImage *surfaceDisplaceImg;
STTexture *surfaceDisplaceTex;

STShaderProgram *shader;

STVector3 mPosition;
STVector3 mLookAt;
STVector3 mRight;
STVector3 mUp;

// Stored mouse position for camera rotation, panning, and zoom.
int gPreviousMouseX = -1;
int gPreviousMouseY = -1;
int gMouseButton = -1;
bool mesh = true; // draw mesh
bool smooth = true; // smooth/flat shading for mesh
bool normalMapping = true; // true=normalMapping, false=displacementMapping
bool proxyType = false; // false: use cylinder; true: use sphere

int currentMesh = 1; //use this to choose a mesh
std::vector<STTriangleMesh*> gTriangleMeshes;
STPoint3 gMassCenter;
std::pair<STPoint3, STPoint3> gBoundingBox;

STTriangleMesh* gManualTriangleMesh = 0;

int TesselationDepth = 100;

void SetUpAndRight() {
	mRight = STVector3::Cross(mLookAt - mPosition, mUp);
	mRight.Normalize();
	mUp = STVector3::Cross(mRight, mLookAt - mPosition);
	mUp.Normalize();
}

void resetCamera() {
	//I modified these, but I chose a generic setup
	//because I made the texture and model
	//command line supplied
	mLookAt = STVector3(0.f, 0.f, 0.f);
	mPosition = STVector3(15.0f,15.0f,15.0f);
	mUp = STVector3(0.f, 1.f, 0.f);

	SetUpAndRight();
}

void resetUp() {
	mUp = STVector3(0.f, 1.f, 0.f);
	mRight = STVector3::Cross(mLookAt - mPosition, mUp);
	mRight.Normalize();
	mUp = STVector3::Cross(mRight, mLookAt - mPosition);
	mUp.Normalize();
}

void CreateYourOwnMesh() {
	float leftX = -2.0f;
	float rightX = -leftX;
	float nearZ = -2.0f;
	float farZ = -nearZ;

	gManualTriangleMesh = new STTriangleMesh();
	for (int i = 0; i < TesselationDepth + 1; i++) {
		for (int j = 0; j < TesselationDepth + 1; j++) {
			float s0 = (float) i / (float) TesselationDepth;
			float x0 = s0 * (rightX - leftX) + leftX;
			float t0 = (float) j / (float) TesselationDepth;
			float z0 = t0 * (farZ - nearZ) + nearZ;

			gManualTriangleMesh->AddVertex(x0, (x0 * x0 + z0 * z0) * 0.0f, z0,
					s0, t0);
		}
	}
	for (int i = 0; i < TesselationDepth; i++) {
		for (int j = 0; j < TesselationDepth; j++) {
			unsigned int id0 = i * (TesselationDepth + 1) + j;
			unsigned int id1 = (i + 1) * (TesselationDepth + 1) + j;
			unsigned int id2 = (i + 1) * (TesselationDepth + 1) + j + 1;
			unsigned int id3 = i * (TesselationDepth + 1) + j + 1;
			gManualTriangleMesh->AddFace(id0, id2, id1);
			gManualTriangleMesh->AddFace(id0, id3, id2);
		}
	}
	gManualTriangleMesh->Build();
	gManualTriangleMesh->mMaterialAmbient[0] = 0.1f;
	gManualTriangleMesh->mMaterialAmbient[1] = 0.1f;
	gManualTriangleMesh->mMaterialAmbient[2] = 0.3f;
	gManualTriangleMesh->mMaterialDiffuse[0] = 0.2f;
	gManualTriangleMesh->mMaterialDiffuse[1] = 0.2f;
	gManualTriangleMesh->mMaterialDiffuse[2] = 0.6f;
	gManualTriangleMesh->mMaterialSpecular[0] = 0.6f;
	gManualTriangleMesh->mMaterialSpecular[1] = 0.6f;
	gManualTriangleMesh->mMaterialSpecular[2] = 0.6f;
	gManualTriangleMesh->mShininess = 8.0f;
}
//
// Initialize the application, loading all of the settings that
// we will be accessing later in our fragment shaders.
//
void Setup() {
	// Set up lighting variables in OpenGL
	// Once we do this, we will be able to access them as built-in
	// attributes in the shader (see examples of this in normalmap.frag)
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);

	surfaceNormImg = new STImage(normalMap);
	surfaceNormTex = new STTexture(surfaceNormImg);

	surfaceDisplaceImg = new STImage(displacementMap);
	surfaceDisplaceTex = new STTexture(surfaceDisplaceImg);

	shader = new STShaderProgram();
	shader->LoadVertexShader(vertexShader);
	shader->LoadFragmentShader(fragmentShader);

	resetCamera();

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	STTriangleMesh::LoadObj(gTriangleMeshes, meshOBJ);
	gMassCenter = STTriangleMesh::GetMassCenter(gTriangleMeshes);
	std::cout << "Mass Center: " << gMassCenter << std::endl;
	gBoundingBox = STTriangleMesh::GetBoundingBox(gTriangleMeshes);
	std::cout << "Bounding Box: " << gBoundingBox.first << " - "
			<< gBoundingBox.second << std::endl;

	CreateYourOwnMesh();
}

void CleanUp() {
	for (unsigned int id = 0; id < gTriangleMeshes.size(); id++)
		delete gTriangleMeshes[id];
	if (gManualTriangleMesh != 0)
		delete gManualTriangleMesh;
}

/**
 * Camera adjustment methods
 */
void RotateCamera(float delta_x, float delta_y) {
	float yaw_rate = 1.f;
	float pitch_rate = 1.f;

	mPosition -= mLookAt;
	STMatrix4 m;
	m.EncodeR(-1 * delta_x * yaw_rate, mUp);
	mPosition = m * mPosition;
	m.EncodeR(-1 * delta_y * pitch_rate, mRight);
	mPosition = m * mPosition;

	mPosition += mLookAt;
}

void ZoomCamera(float delta_y) {
	STVector3 direction = mLookAt - mPosition;
	float magnitude = direction.Length();
	direction.Normalize();
	float zoom_rate = 0.1f * magnitude < 0.5f ? .1f * magnitude : .5f;
	if (delta_y * zoom_rate + magnitude > 0) {
		mPosition += (delta_y * zoom_rate) * direction;
	}
}

void StrafeCamera(float delta_x, float delta_y) {
	float strafe_rate = 0.05f;

	mPosition -= strafe_rate * delta_x * mRight;
	mLookAt -= strafe_rate * delta_x * mRight;
	mPosition += strafe_rate * delta_y * mUp;
	mLookAt += strafe_rate * delta_y * mUp;
}

//
// Display the output image from our vertex and fragment shaders
//
void DisplayCallback() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	SetUpAndRight();

	gluLookAt(mPosition.x, mPosition.y, mPosition.z, mLookAt.x, mLookAt.y,
			mLookAt.z, mUp.x, mUp.y, mUp.z);

	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	// Texture 0: surface normal map
	glActiveTexture(GL_TEXTURE0);
	surfaceNormTex->Bind();

	// Texture 1: surface normal map
	glActiveTexture(GL_TEXTURE1);
	surfaceDisplaceTex->Bind();

	// Bind the textures we've loaded into openGl to
	// the variable names we specify in the fragment
	// shader.
	shader->SetTexture("normalTex", 0);
	shader->SetTexture("displacementTex", 1);
	shader->SetTexture("colorTex", 2);

	// Invoke the shader.  Now OpenGL will call our
	// shader programs on anything we draw.
	shader->Bind();

	if (mesh) {
		shader->SetUniform("normalMapping", -1.0);
		shader->SetUniform("displacementMapping", -1.0);
		shader->SetUniform("colorMapping", 1.0);

		glPushMatrix();
		// Pay attention to scale
		STVector3 size_vector = gBoundingBox.second - gBoundingBox.first;
		float maxSize = (std::max)((std::max)(size_vector.x, size_vector.y),
				size_vector.z);
		glScalef(3.0f / maxSize, 3.0f / maxSize, 3.0f / maxSize);
		glTranslatef(-gMassCenter.x, -gMassCenter.y, -gMassCenter.z);
		//for(unsigned int id=0;id<gTriangleMeshes.size();id++) {
		//	gTriangleMeshes[id]->Draw(smooth);
		//}

		//just draw the selected mesh

		gTriangleMeshes[currentMesh]->Draw(smooth);

		glPopMatrix();
	} else {
		if (normalMapping) {
			shader->SetUniform("displacementMapping", -1.0);
			shader->SetUniform("normalMapping", 1.0);
			shader->SetUniform("colorMapping", -1.0);
		} else {
			shader->SetUniform("displacementMapping", 1.0);
			shader->SetUniform("normalMapping", -1.0);
			shader->SetUniform("colorMapping", -1.0);
			shader->SetUniform("TesselationDepth", TesselationDepth);
		}
		if (gManualTriangleMesh)
			gManualTriangleMesh->Draw(smooth);
		if (gTriangleMeshes.size()) {
			for (unsigned int id = 0; id < gTriangleMeshes.size(); id++) {
				if (gManualTriangleMesh[id].mSurfaceColorTex){
					gTriangleMeshes[id]->Draw(smooth);
				}else{
					gTriangleMeshes[id]->Draw(smooth);
				}
			}
		}
	}

	shader->UnBind();

	glActiveTexture(GL_TEXTURE0);
	surfaceNormTex->UnBind();

	glActiveTexture(GL_TEXTURE1);
	surfaceDisplaceTex->UnBind();

	glutSwapBuffers();
}

//
// Reshape the window and record the size so
// that we can use it for screenshots.
//
void ReshapeCallback(int w, int h) {
	gWindowSizeX = w;
	gWindowSizeY = h;

	glViewport(0, 0, gWindowSizeX, gWindowSizeY);

	glMatrixMode( GL_PROJECTION);
	glLoadIdentity();
	// Set up a perspective projection
	float aspectRatio = (float) gWindowSizeX / (float) gWindowSizeY;
	gluPerspective(30.0f, aspectRatio, .1f, 10000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void SpecialKeyCallback(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		StrafeCamera(10, 0);
		break;
	case GLUT_KEY_RIGHT:
		StrafeCamera(-10, 0);
		break;
	case GLUT_KEY_DOWN:
		StrafeCamera(0, -10);
		break;
	case GLUT_KEY_UP:
		StrafeCamera(0, 10);
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void KeyCallback(unsigned char key, int x, int y) {
	// TO DO: Any new key press events must be added to this function

	switch (key) {
	case 's': {
		//
		// Take a screenshot, and save as screenshot.jpg
		//
		STImage* screenshot = new STImage(gWindowSizeX, gWindowSizeY);
		screenshot->Read(0, 0);
		screenshot->Save("../../data/images/screenshot.jpg");
		delete screenshot;
	}
		break;
	case 'r':
		resetCamera();
		break;
	case 'u':
		resetUp();
		break;
	case 'm': // switch between the mesh you create and the mesh from file
		mesh = !mesh;
		break;
	case 'n': // switch between normalMapping and displacementMapping
		normalMapping = !normalMapping;
		break;
		// TO:DO do loop subdivision, uncomment this code
		// modify it so that it works with your sphere
		// simply make sure you are calling the functions
		// on the correct mesh.
		//---------------------------------
	case 'l':

		if (mesh) {
			//Instead of subdividing on the fly, I had subdivisions 0-10 created
			//in the beginning, and you can cycle through those.
			currentMesh = (currentMesh + 1) % gTriangleMeshes.size();
		}
		break;
	case 'f': // switch between smooth shading and flat shading
		smooth = !smooth;
		break;
	case 'w':
		for (unsigned int id = 0; id < gTriangleMeshes.size(); id++)
			gTriangleMeshes[id]->Write("output.obj");
		break;
	case 'a':
		for (unsigned int id = 0; id < gTriangleMeshes.size(); id++)
			gTriangleMeshes[id]->mDrawAxis = !gTriangleMeshes[id]->mDrawAxis;
		if (gManualTriangleMesh != 0)
			gManualTriangleMesh->mDrawAxis = !gManualTriangleMesh->mDrawAxis;
		break;
	case 'q':
		exit(0);
	default:
		break;
	}

	glutPostRedisplay();
}

/**
 * Mouse event handler
 */
void MouseCallback(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON || button == GLUT_RIGHT_BUTTON
			|| button == GLUT_MIDDLE_BUTTON) {
		gMouseButton = button;
	} else {
		gMouseButton = -1;
	}

	if (state == GLUT_UP) {
		gPreviousMouseX = -1;
		gPreviousMouseY = -1;
	}
}

/**
 * Mouse active motion callback (when button is pressed)
 */
void MouseMotionCallback(int x, int y) {
	if (gPreviousMouseX >= 0 && gPreviousMouseY >= 0) {
		//compute delta
		float deltaX = x - gPreviousMouseX;
		float deltaY = y - gPreviousMouseY;
		gPreviousMouseX = x;
		gPreviousMouseY = y;

		//orbit, strafe, or zoom
		if (gMouseButton == GLUT_LEFT_BUTTON) {
			RotateCamera(deltaX, deltaY);
		} else if (gMouseButton == GLUT_MIDDLE_BUTTON) {
			StrafeCamera(deltaX, deltaY);
		} else if (gMouseButton == GLUT_RIGHT_BUTTON) {
			ZoomCamera(deltaY);
		}

	} else {
		gPreviousMouseX = x;
		gPreviousMouseY = y;
	}

}

//-------------------------------------------------
// Inits the TriangleIndices
//-------------------------------------------------
TriangleIndices makeTIndices(int a, int b, int c) {
	TriangleIndices T;
	T.i1 = a;
	T.i2 = b;
	T.i3 = c;
	return (T);
}

//-----------------------------------------------
// offset each point to the sphere surface
//-----------------------------------------------
int offset(STVector3 p, std::vector<STVector3> *vertices) {
	double length = sqrtf(p.x * p.x + p.y * p.y + p.z * p.z);
	vertices->push_back(STVector3(p.x / length, p.y / length, p.z / length));
	return (globalCount + 1);
}

//-----------------------------------------------------------------------
// TO DO:Returns index of smallest point
// Find the middle point at each edge, remove duplicates
// call the offset() function to offset the point from the icocahedron to the
// sphere surface
//-----------------------------------------------------------------------
int midPoint(int p1, int p2, std::multimap<long, int> *midPointIndices,
		std::vector<STVector3> *vertices) {
	STVector3 point1 = (*vertices)[p1];
	//point the new vector in between the first two
	STVector3 midpoint = STVector3((*vertices)[p1].x + (*vertices)[p2].x,
			(*vertices)[p1].y + (*vertices)[p2].y,
			(*vertices)[p1].z + (*vertices)[p2].z);
	//the vector is now in the right direction,
	//but has the wrong magnitude
	midpoint.Normalize();

	//if this point already exists, return it
	for (int i = 0; i < vertices->size(); i++) {
		if ((*vertices)[i].x == midpoint.x && (*vertices)[i].y == midpoint.y
				&& (*vertices)[i].z == midpoint.z) {
			return i;
		}
	}

	//otherwise, add the point, then return it
	int index = vertices->size();
	vertices->push_back(midpoint);
	return (index);
}

//----------------------------------------------------------
// TO DO: Using the CreateYourOwnMesh as an example
// Store your mesh data in the triangleMesh
//-----------------------------------------------------------
void createMySphereMesh(STTriangleMesh *tmesh, TriangleIndices face,
		std::vector<STVector3> *vertices) {
	//I'm not sure what I was meant to put in here.
}

//---------------------------------------------------------
// Spliting them into 4 smaller triangles
//----------------------------------------------------------
void subDivideTriangles(int level, std::vector<TriangleIndices> *facesIn,
		std::vector<TriangleIndices> *facesOut,
		std::vector<STVector3> *vertices) {
	std::multimap<long, int> midPointIndices;

	for (int i = 0; i < level; i++) {

		for (int j = 0; j < facesIn->size(); ++j) {

			int a = midPoint((*facesIn)[j].i1, (*facesIn)[j].i2,
					&midPointIndices, vertices);
			int b = midPoint((*facesIn)[j].i2, (*facesIn)[j].i3,
					&midPointIndices, vertices);
			int c = midPoint((*facesIn)[j].i3, (*facesIn)[j].i1,
					&midPointIndices, vertices);

			facesOut->push_back(makeTIndices((*facesIn)[j].i1, a, c));
			facesOut->push_back(makeTIndices((*facesIn)[j].i2, b, a));
			facesOut->push_back(makeTIndices((*facesIn)[j].i3, c, b));
			facesOut->push_back(makeTIndices(a, b, c));
		}
		(*facesIn) = (*facesOut);
	}
}

//-------------------------------------------------------
// TO DO: Create faces for the Iscohedron
// Each TriangleIndices stores the counter clocksise vertex indices
// See definition of TriangleIndices
//-------------------------------------------------------
void initFaces(std::vector<TriangleIndices> *faces) {
	TriangleIndices tri;
	int zero_corner[] = { 1, 7, 10, 11, 5 };
	int three_corner[] = { 6, 8, 9, 4, 2 };
	int strip[] = { 1, 8, 7, 6, 10, 2, 11, 4, 5, 9 };
	//get all of the top corners
	for (int i = 0; i < 5; i++) {
		tri.i1 = 0;
		tri.i2 = zero_corner[i];
		tri.i3 = zero_corner[(i + 1) % 5];
		faces->push_back(tri);
	}
	//all of the bottom corners
	for (int i = 0; i < 5; i++) {
		tri.i1 = 3;
		tri.i2 = three_corner[i];
		tri.i3 = three_corner[(i + 1) % 5];
		faces->push_back(tri);
	}
	//and the sides, as if they were part of a tri-strip
	for (int i = 0; i < 10; i++) {
		if (i % 2 == 0) {
			tri.i1 = strip[i];
			tri.i2 = strip[(i + 1) % 10];
			tri.i3 = strip[(i + 2) % 10];
			faces->push_back(tri);
		} else {
			tri.i1 = strip[(i + 1) % 10];
			tri.i2 = strip[i];
			tri.i3 = strip[(i + 2) % 10];
			faces->push_back(tri);
		}
	}
}

// Initialize 12 verticies for an iscohedron
// centered at zero. See icosphere_visual.pdf in the docs/folder
void initVertices(std::vector<STVector3> *vertices) {
	float pos = (1.0 + sqrtf(5.0)) / 2.0;

	vertices->push_back(STVector3(-1, pos, 0));
	vertices->push_back(STVector3(1, pos, 0));
	vertices->push_back(STVector3(-1, -pos, 0));
	vertices->push_back(STVector3(1, -pos, 0));

	vertices->push_back(STVector3(0, -1, pos));
	vertices->push_back(STVector3(0, 1, pos));
	vertices->push_back(STVector3(0, -1, -pos));
	vertices->push_back(STVector3(0, 1, -pos));

	vertices->push_back(STVector3(pos, 0, -1));
	vertices->push_back(STVector3(pos, 0, 1));
	vertices->push_back(STVector3(-pos, 0, -1));
	vertices->push_back(STVector3(-pos, 0, 1));
}

//----------------------------------------------------------------
// TO DO: Complete this function to create your sphere
// The sphere is a unit iscosphere centered at the origin (0,0,0).
// First add a key press event in KeyCallback that will call this function
//----------------------------------------------------------------
void createSphere(void) {
	globalCount = 0; //for subdivisions // TO DO: Optional remove this as a global var

	// triangle mesh object
	std::vector<STTriangleMesh *> gTriangleMeshes_sphere;

	// vertices
	std::vector<STVector3> vertices;

	// Creates the initial verticies
	initVertices(&vertices);

	//-----------------------------------------------------
	// TO DO: create the faces of the intIcosahedron
	// Place your code in the function initFaces()
	//-----------------------------------------------------
	std::vector<TriangleIndices> faces;
	initFaces(&faces);
	STTriangleMesh* mesh = new STTriangleMesh();
	STVector3 t;
	TriangleIndices f;
	for (long i = 0; i < vertices.size(); i++) {
		t = vertices[i];
		t.Normalize();
		mesh->AddVertex(t.x, t.y, t.z);
	}
	for (long i = 0; i < faces.size(); i++) {
		f = faces[i];
		mesh->AddFace(f.i1, f.i2, f.i3);
	}
	mesh->Build();
	mesh->mMaterialAmbient[0] = 0.1f;
	mesh->mMaterialAmbient[1] = 0.1f;
	mesh->mMaterialAmbient[2] = 0.3f;
	mesh->mMaterialDiffuse[0] = 0.2f;
	mesh->mMaterialDiffuse[1] = 0.2f;
	mesh->mMaterialDiffuse[2] = 0.6f;
	mesh->mMaterialSpecular[0] = 0.6f;
	mesh->mMaterialSpecular[1] = 0.6f;
	mesh->mMaterialSpecular[2] = 0.6f;
	mesh->mShininess = 8.0f;
	gTriangleMeshes_sphere.push_back(mesh);

	//---------------------------------------------------------------
	// TO DO: Recursively split each triangle into four triangles
	// See images in docs/icosahedron/
	// Determine the levels for the recursion by changing the value in levels
	//----------------------------------------------------------------

	//create up to 5 subdivision models.  If you need more than 20k triangles for
	//a sphere, you're probably doomed anyway.
	for (int levels = 1; levels <= 3; levels++) {
		std::vector<TriangleIndices> newfaces;
		initFaces(&newfaces);
		subDivideTriangles(1, &faces, &newfaces, &vertices);

		//-----------------------------------------------------------------
		// TO DO: Once faces are generated, add each triangle to the
		// call createMySphereMesh here
		// mesh. Place your code in createMySphereMesh()
		//-----------------------------------------------------------------

		mesh = new STTriangleMesh();
		for (int i = 0; i < vertices.size(); i++) {
			t = vertices[i];
			t.Normalize();
			mesh->AddVertex(t.x, t.y, t.z);
		}
		for (int i = 0; i < newfaces.size(); i++) {
			f = newfaces[i];
			mesh->AddFace(f.i1, f.i2, f.i3);
		}

		mesh->Build();
		mesh->mMaterialAmbient[0] = 0.2f;
		mesh->mMaterialAmbient[1] = 0.2f;
		mesh->mMaterialAmbient[2] = 0.6f;
		mesh->mMaterialDiffuse[0] = 0.2f;
		mesh->mMaterialDiffuse[1] = 0.2f;
		mesh->mMaterialDiffuse[2] = 0.6f;
		mesh->mMaterialSpecular[0] = 0.6f;
		mesh->mMaterialSpecular[1] = 0.6f;
		mesh->mMaterialSpecular[2] = 0.6f;
		mesh->mShininess = 8.0f;
		gTriangleMeshes_sphere.push_back(mesh);

		faces = newfaces;
	}
	//createMySphereMesh(mesh,faces[i],vertices);

	// save the result sphere
	gTriangleMeshes_sphere[3]->Write("../../data/meshes/mysphere.obj");
	for (unsigned int id = 0; id < gTriangleMeshes_sphere.size(); id++) {
		//changed for linux
		gTriangleMeshes.push_back(gTriangleMeshes_sphere[id]);
	}

}

void usage() {
	// TO DO
	printf(
			"usage: proj1_mesh vertShader fragShader objMeshFile normalMappingTexture displacementMappingTexture\n");
}

STTriangleMesh* loadOBJ(char* objfilename, char* texturefilename) {
	STTriangleMesh* result = new STTriangleMesh();
	FILE* objfile = fopen(objfilename, "r");
	while (!(feof(objfile))) {
		char ch;
		float x, y, z;
		int read = fscanf(objfile, "%c %f %f %f", &ch, &x, &y, &z);
		if (read == 4) {
			if (ch == 'v') {
				result->AddVertex(x, y, z);
			}
			if (ch == 'f') {
				//subtract one because c counts from 0
				//and obj counts from 1
				result->AddFace(x-1,y-1,z-1);
			}
		}
	}
	result->mSurfaceColorImg = new STImage(texturefilename);
	result->mSurfaceColorTex = new STTexture(result->mSurfaceColorImg);
	result->CalculateTextureCoordinatesViaSphericalProxy();
	result->Build();
	result->mMaterialAmbient[0] = 0.5;
	result->mMaterialAmbient[1] = 0.5;
	result->mMaterialAmbient[2] = 0.5;
	result->mMaterialDiffuse[0] = 1.0f;
	result->mMaterialDiffuse[1] = 1.0f;
	result->mMaterialDiffuse[2] = 1.0f;
	result->mMaterialSpecular[0] = 0.6f;
	result->mMaterialSpecular[1] = 0.6f;
	result->mMaterialSpecular[2] = 0.6f;
	result->mShininess = 8.0f;
	return result;
}

//-------------------------------------------------
// main program loop
//-------------------------------------------------
int main(int argc, char** argv) {
//---------------------------------------------------------------------------
// TO DO: Change this file name to change the .obj model that is loaded
// Optional: read in the file name from the command line > proj1_mesh myfile.obj
//--------------------------------------------------------------------------
meshOBJ = std::string("../../data/meshes/cone.obj");

vertexShader = std::string("kernels/default.vert");
fragmentShader = std::string("kernels/phong.frag");
normalMap = std::string("../../data/images/normalmap.png");
displacementMap = std::string("../../data/images/displacementmap.jpeg");

//
// Initialize GLUT.
//
glutInit(&argc, argv);
glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
glutInitWindowPosition(20, 20);
glutInitWindowSize(640, 480);
glutCreateWindow("proj1_mesh");

//
// Initialize GLEW.
//
#ifndef __APPLE__
glewInit();
if (!GLEW_VERSION_2_0) {
	printf("Your graphics card or graphics driver does\n"
			"\tnot support OpenGL 2.0, trying ARB extensions\n");

	if (!GLEW_ARB_vertex_shader || !GLEW_ARB_fragment_shader) {
		printf("ARB extensions don't work either.\n");
		printf("\tYou can try updating your graphics drivers.\n"
				"\tIf that does not work, you will have to find\n");
		printf("\ta machine with a newer graphics card.\n");
		exit(1);
	}
}
#endif

// Be sure to initialize GLUT (and GLEW for this assignment) before
// initializing your application.

Setup();

createSphere();

printf("%d args\n",argc);
if (argc==3){
	gTriangleMeshes.push_back(loadOBJ(argv[1],argv[2]));
}

glutDisplayFunc(DisplayCallback);
glutReshapeFunc(ReshapeCallback);
glutSpecialFunc(SpecialKeyCallback);
glutKeyboardFunc(KeyCallback);
glutMouseFunc(MouseCallback);
glutMotionFunc(MouseMotionCallback);
glutIdleFunc(DisplayCallback);

glutMainLoop();

// Cleanup code should be called here.
CleanUp();

return 0;
}
