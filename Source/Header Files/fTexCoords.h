#pragma once
class fTexCoords3d
{
public:
	float u,v;

	fTexCoords3d(void);
	fTexCoords3d(const fTexCoords3d &tex);
	fTexCoords3d(float u, float v);
	~fTexCoords3d(void);

	void fTexCoords3d::operator =(const fTexCoords3d &tex);
};

