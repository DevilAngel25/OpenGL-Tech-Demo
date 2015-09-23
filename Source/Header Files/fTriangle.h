#pragma once
#include <Windows.h>
#include "iVector3d.h"

class fTriangle
{
public:
	iVector3d _ver, _nor, _tex;

	fTriangle(void);
	fTriangle(const fTriangle &tri);
	fTriangle(iVector3d ver, iVector3d nor, iVector3d tex);
	~fTriangle(void);

	void fTriangle::operator =(const fTriangle &tri);
};

