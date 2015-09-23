#include "ObjLoader.h"

ObjLoader::ObjLoader(void)
{
}
ObjLoader::~ObjLoader(void)
{
}

void ObjLoader::Load(char* path, Model &loadedmodel)
{
//reads the model from the path specified and stores it to the model object specified by loaded model
	std::vector<std::string*> contentsoffile;
	char textbuffer[256];
	std::ifstream in(path);

    if(!in.is_open())
    {
            std::cerr<<"model can not be opened"<<std::endl;
    }
    while(!in.eof())
    {
	//read to end of file
            in.getline(textbuffer,256);
            contentsoffile.push_back(new std::string(textbuffer));
    }
	for(unsigned int i=0;i<contentsoffile.size();i++)
    {
	//loop through read file 
		if(contentsoffile[i]->c_str()[0]=='#')//# means its a comment
		{
			continue;
		}
		else if(contentsoffile[i]->c_str()[0]=='v'&& contentsoffile[i]->c_str()[1]==' ')//'v ' means a vertex
		{
			float tmpx,tmpy,tmpz;
			sscanf_s(contentsoffile[i]->c_str(),"v %f %f %f",&tmpx,&tmpy,&tmpz);   
			loadedmodel.AddVertex(fVector3d(tmpx,tmpy,tmpz));
		}
		else if(contentsoffile[i]->c_str()[0]=='v' && contentsoffile[i]->c_str()[1]=='t')//'vt' means a uv coordinate
		{
			float u,v;
			sscanf_s(contentsoffile[i]->c_str(),"vt %f %f %f",&u,&v);
			loadedmodel.AddUV(fTexCoords3d(u,v));
		}
		else if(contentsoffile[i]->c_str()[0]=='v'&& contentsoffile[i]->c_str()[1]=='n')//'vt' means a vector normal
		{
			float tmpx,tmpy,tmpz;
			sscanf_s(contentsoffile[i]->c_str(),"vn %f %f %f",&tmpx,&tmpy,&tmpz);
			loadedmodel.AddNormal(fVector3d(tmpx,tmpy,tmpz));
		}
		else if(contentsoffile[i]->c_str()[0]=='f')//'f ' means a face
        {
			int fa,fb,fc,na,nb,nc,ta,tb,tc;
			fa=fb=fc=na=nb=nc=ta=tb=tc= 0;
			for (;;)
			{
				if (sscanf_s(contentsoffile[i]->c_str(),"f %d/%d/%d %d/%d/%d %d/%d/%d",&fa,&ta,&na,&fb,&tb,&nb,&fc,&tc,&nc) == 9){
					break; }
				if (sscanf_s(contentsoffile[i]->c_str(),"f %d//%d %d//%d %d//%d",&fa,&na,&fb,&nb,&fc,&nc) == 6){
					break;}
				if (sscanf_s(contentsoffile[i]->c_str(),"f %d/%d %d/%d %d/%d",&fa,&ta,&fb,&tb,&fc,&tc) == 6){
					break;}
				if (sscanf_s(contentsoffile[i]->c_str(),"f %d %d %d",&fa,&fb,&fc) == 3){
					break;}
				else
				{
					std::cerr<<"the file you loaded is invalid"<<std::endl;
					break;
				}
			}
			loadedmodel.AddTriangle(fTriangle(iVector3d(fa-1,fb-1,fc-1),iVector3d(na-1,nb-1,nc-1),iVector3d(ta-1,tb-1,tc-1)));
		}
		else
		{
			continue;
		}
	}
}