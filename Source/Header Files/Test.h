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

class Test
{
public:
	Model loadedmodel;
	Test(void);
	~Test(void);
	Model* givemeamodel();
};

