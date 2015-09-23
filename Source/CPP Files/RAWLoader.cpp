#include "stdafx.h"
#include "RAWLoader.h"

RAWLoader::RAWLoader(void)
{
}
RAWLoader::~RAWLoader(void)
{
}

std::vector<BYTE> RAWLoader::Load(LPSTR strName)
{
	FILE *pFile = NULL;
	std::vector<BYTE> retval;
	// open the file in read binary 
	pFile = fopen( strName, "rb" );
	if ( pFile == NULL )	
	{
		// if the file didnt open display an error message and stop the function
		std::cerr<<"Can't Find The raw file!"<<std::endl;
		return retval;
	}

	fread( g_HeightMap, 1, MAP_SIZE*MAP_SIZE, pFile );

	int result = ferror( pFile );
	if (result)
	{
	// error check
		std::cout<<"Failed To Get Data!"<<std::endl;
	}

	//close file
	fclose(pFile);
	
	retval.assign(g_HeightMap,g_HeightMap+ sizeof(g_HeightMap));
	return retval;
}