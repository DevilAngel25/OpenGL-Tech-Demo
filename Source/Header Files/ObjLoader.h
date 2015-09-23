#pragma once
#include <Windows.h>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include "Model.h"
#include "fTexCoords.h"
#include "fTriangle.h"
#include "fVector3d.h"

class ObjLoader
{
public:
	ObjLoader(void);
	~ObjLoader(void);

	void Load(char* path, Model &loadedmodel);
};

