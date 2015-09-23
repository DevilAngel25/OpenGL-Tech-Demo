#include "StdAfx.h"
#include "Test.h"


Test::Test(void)
{
}


Test::~Test(void)
{
}

Model* Test::givemeamodel()
{
			float tmpx,tmpy,tmpz;
			tmpx = -1, tmpy = -1,tmpz =-1; 
			loadedmodel.AddVertex(fVector3d(tmpx,tmpy,tmpz));
			tmpx = -1, tmpy = -1,tmpz = 1; 
			loadedmodel.AddVertex(fVector3d(tmpx,tmpy,tmpz));
			tmpx = -1, tmpy = 1,tmpz =1; 
			loadedmodel.AddVertex(fVector3d(tmpx,tmpy,tmpz));
			tmpx = -1, tmpy = 1,tmpz =-1; 
			loadedmodel.AddVertex(fVector3d(tmpx,tmpy,tmpz));

			tmpx = 1, tmpy = 1,tmpz =-1; 
			loadedmodel.AddVertex(fVector3d(tmpx,tmpy,tmpz));
			tmpx = 1, tmpy = -1,tmpz =-1; 
			loadedmodel.AddVertex(fVector3d(tmpx,tmpy,tmpz));
			tmpx = 1, tmpy = 1,tmpz =1; 
			loadedmodel.AddVertex(fVector3d(tmpx,tmpy,tmpz));
			tmpx = 1, tmpy = -1,tmpz =1; 
			loadedmodel.AddVertex(fVector3d(tmpx,tmpy,tmpz));

			int fa,fb,fc;
			fa = 1,fb= 2, fc = 3;
			loadedmodel.AddTriangle(fTriangle(iVector3d(fa-1,fb-1,fc-1)));
			fa = 1,fb= 3, fc = 4;
			loadedmodel.AddTriangle(fTriangle(iVector3d(fa-1,fb-1,fc-1)));
			fa = 4,fb= 5, fc = 6;
			loadedmodel.AddTriangle(fTriangle(iVector3d(fa-1,fb-1,fc-1)));
			fa = 6,fb= 1, fc = 4;
			loadedmodel.AddTriangle(fTriangle(iVector3d(fa-1,fb-1,fc-1)));
			fa = 5,fb= 7, fc = 8;
			loadedmodel.AddTriangle(fTriangle(iVector3d(fa-1,fb-1,fc-1)));
			fa = 5,fb= 8, fc = 6;
			loadedmodel.AddTriangle(fTriangle(iVector3d(fa-1,fb-1,fc-1)));			
			fa = 2,fb= 8, fc = 7;
			loadedmodel.AddTriangle(fTriangle(iVector3d(fa-1,fb-1,fc-1)));			
			fa = 2,fb= 7, fc = 3;
			loadedmodel.AddTriangle(fTriangle(iVector3d(fa-1,fb-1,fc-1)));
			fa = 2,fb= 1, fc = 6;
			loadedmodel.AddTriangle(fTriangle(iVector3d(fa-1,fb-1,fc-1)));			
			fa = 2,fb= 6, fc = 8;
			loadedmodel.AddTriangle(fTriangle(iVector3d(fa-1,fb-1,fc-1)));			
			fa = 7,fb= 5, fc = 4;
			loadedmodel.AddTriangle(fTriangle(iVector3d(fa-1,fb-1,fc-1)));			
			fa = 7,fb= 4, fc = 3;
			loadedmodel.AddTriangle(fTriangle(iVector3d(fa-1,fb-1,fc-1)));

	return &loadedmodel; 
}