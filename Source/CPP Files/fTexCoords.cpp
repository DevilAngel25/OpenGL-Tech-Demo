#include "fTexCoords.h"

fTexCoords3d::fTexCoords3d(void)
{
//set the uv coords to 0
	this->u = 0.0f;
	this->v = 0.0f;
}

fTexCoords3d::fTexCoords3d(const fTexCoords3d &tex)
{
//copy constructor
	this->u = tex.u;
	this->v = tex.v;
}
void fTexCoords3d::operator =(const fTexCoords3d &tex)
{
//copy constructor
	this->u = tex.u;
	this->v = tex.v;
}
fTexCoords3d::fTexCoords3d(float u, float v)
{
//set u to u and v to v
	this->u = u;
	this->v = v;
}

fTexCoords3d::~fTexCoords3d(void)
{
}


