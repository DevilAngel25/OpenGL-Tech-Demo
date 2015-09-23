#pragma once
#include <windows.h>									
#include <stdio.h>										
#include <gl\gl.h>										
#include <gl\glu.h>	
#include <iostream>
#include <vector>
#define MAP_SIZE 1024	
class RAWLoader
{
		

public:
	BYTE g_HeightMap[MAP_SIZE*MAP_SIZE];					
	RAWLoader(void);
	~RAWLoader(void);
	std::vector<BYTE> Load(LPSTR strName);
};

