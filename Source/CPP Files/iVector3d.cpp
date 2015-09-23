#include "iVector3d.h"

iVector3d::iVector3d(void)
{
	this->x = 0;
	this->y = 0;
	this->z = 0;
}
iVector3d::iVector3d(const iVector3d &vec)
{
	//copy constructor
	this->x = vec.x;
	this->y = vec.y;
	this->z = vec.z;
}
void iVector3d::operator =(const iVector3d &vec)
{
//copy constructor
	this->x = vec.x;
	this->y = vec.y;
	this->z = vec.z;
}
iVector3d::	iVector3d(int x,int y, int z)
{
//assigns x to x y to y and z to z
	this->x = x;
	this->y = y;
	this->z = z;
}
iVector3d::~iVector3d(void)
{
}

