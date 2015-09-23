#pragma once
class iVector3d
{
public:
	int x,y,z;
	iVector3d(void);
	iVector3d(const iVector3d &vec);
	iVector3d(int x,int y, int z);
	~iVector3d(void);

	void iVector3d::operator =(const iVector3d &vec);
};

