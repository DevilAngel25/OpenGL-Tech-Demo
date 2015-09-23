#pragma once
#include <windows.h>									// Header File For Windows
#include <stdio.h>										// Header file for standard input output (IE, "FILE") (NEW)
#include <glew.h>
#include <glut.h>
#include <gl\gl.h>										// Header File For The OpenGL32 Library
#include <gl\glu.h>	
#include <iostream>
#include <vector>
#include "RAWLoader.h"
#include "iVector3d.h"
#include "fVector3d.h"
#include "fTexCoords.h"

#define MAP_SIZE 1024
#define	STEP_SIZE 8			// Width And Height Of Each Quad (NEW)
#define	HEIGHT_RATIO 0.3f	

class TerrainGenerator
{

public:
	float scaleValue;
	RAWLoader loader;
	std::vector<BYTE> g_HeightMap;
	std::vector<iVector3d> _vboVer;
	std::vector<fVector3d> _vboNor;
	std::vector<fTexCoords3d> _vboTex;
	std::vector<GLuint> _vboInd;
	GLuint verBuff;
	GLuint norBuff;
	GLuint texBuff;
	GLuint indBuff;
	bool _vboInit;

	TerrainGenerator(void);
	~TerrainGenerator(void);
	
	int Height(int X, int Y);																	
	void SetVertice(int x, int y, int z);
	void SetNormal(float x, float y, float z);
	void SetTex(float u, float v);
	void RenderHeightMap();		
	void init();
	void SetInd();
	void InitVBO();
	void CreateVBO();
};

