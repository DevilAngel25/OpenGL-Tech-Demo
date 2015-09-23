#pragma once
class fVector3d
{
public:
	float x,y,z;
	fVector3d(void);
	fVector3d(const fVector3d &vec);
	fVector3d(float x,float y, float z);
	~fVector3d(void);

	fVector3d crossProduct(const fVector3d& v) const;

	void fVector3d::operator =(const fVector3d &vec);
};

