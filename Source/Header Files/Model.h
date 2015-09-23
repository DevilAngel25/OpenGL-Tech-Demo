#pragma once
#include <Windows.h>
#include <vector>
#include "fTriangle.h"
#include "fVector3d.h"
#include "fTexCoords.h"
#include "iVector3d.h"
#include <glew.h>
#include <glut.h>
#include <GL\GL.h>
#include <stdlib.h>
#include <glext.h>
class Model
{
public:
	//con/destructors	
	Model(void);
	Model(float x, float y, float z, float angle, float rotx, float roty, float rotz, float scx, float scy, float scz);
	Model(Model &mdl);
	~Model(void);
	//class variables
	std::vector<fTriangle> _triangles;
	std::vector<fVector3d> _verticies , _normals;
	std::vector<fTexCoords3d> _uvcoords;
	std::vector<fVector3d> _vboVer, _vboNor;
	std::vector<fTexCoords3d> _vboTex;
	std::vector<GLuint> _vboInd;
	bool _hasNor, _hasTex, _vboInit;
	float _x, _y, _z;
	float _angle, _rotx, _roty, _rotz;
	float _scx, _scy, _scz;
	GLuint verBuff, norBuff, texBuff, indexBuff;

	//model functions
	int Clear();
	int AddVertex(fVector3d &vertexadd);
	int AddUV(fTexCoords3d &texcoord);
	int AddTriangle(fTriangle &triangleadd);
	int AddNormal(fVector3d & normaladd);
	int AddFace(fTriangle &triangle);
	std::vector<fTriangle>* GetTriangles();
	std::vector<fVector3d>* GetVerticies();
	std::vector<fTexCoords3d>* GetTextureCoordinates();
	std::vector<fVector3d>* GetNormals();
	void makegeo(float r, float g, float b);
	void initVBO();
	void createIndex();

};

