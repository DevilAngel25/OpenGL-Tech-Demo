#include "fTriangle.h"

fTriangle::fTriangle(void)
{
}
fTriangle::fTriangle(const fTriangle &tri)
{
//copy constructor
	this->_ver = tri._ver;
	this->_nor = tri._nor;
	this->_tex = tri._tex;
}
fTriangle::fTriangle(iVector3d ver,iVector3d nor,iVector3d tex)
{
//assign ver to ver nor to nor and tex to tex
	this->_ver = ver;
	this->_nor = nor;
	this->_tex = tex;
}
fTriangle::~fTriangle(void)
{
}

void fTriangle::operator =(const fTriangle &tri)
{
//assign ver to ver nor to nor and tex to tex
	this->_ver = tri._ver;
	this->_nor = tri._nor;
	this->_tex = tri._tex;
}