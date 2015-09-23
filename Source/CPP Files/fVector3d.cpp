#include "fVector3d.h"

fVector3d::fVector3d(void)
{
//assigns 0 to all var's
	this->x = 0;
	this->y = 0;
	this->z = 0;
}
fVector3d::fVector3d(const fVector3d &vec)
{
//copy constructor
	this->x = vec.x;
	this->y = vec.y;
	this->z = vec.z;
}
void fVector3d::operator =(const fVector3d &vec)
{
//copy constructor
	this->x = vec.x;
	this->y = vec.y;
	this->z = vec.z;
}
fVector3d::	fVector3d(float x,float y, float z)
{
//assigns x to x y to y and z to z
	this->x = x;
	this->y = y;
	this->z = z;
}
fVector3d::~fVector3d(void)
{
}

fVector3d fVector3d::crossProduct(const fVector3d& v) const
{
//calculates the cross product of the current vector and v passed in
	fVector3d result;
	result.x = this->y * v.z - v.y * this->z;
	result.y = this->x * v.z - v.x * this->z;
	result.z = this->x * v.y - v.x * this->y;
	return result;
}

