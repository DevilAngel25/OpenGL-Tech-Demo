#include "Model.h"

Model::Model(void)
{
//assigns construction based variables and sets them, to defaults
	_hasNor = false;
	_hasTex = false;
	_vboInit = true;

	_x = 0.0f, _y = 10.0f, _z = 0.0f;
	_angle = 0.0f, _rotx = 0.0f, _roty = 1.0f, _rotz = 0.0f;
	_scx = -1.0f, _scy = -1.0f, _scz = -1.0f;
}

Model::Model(float x, float y, float z, float angle, float rotx, float roty, float rotz, float scx, float scy, float scz)
{
//prefered constructor taking transfrmation info and setting internal vars to that value specified
	_hasNor = false;
	_hasTex = false;
	_vboInit = true;

	_x = x, _y = y, _z = z;
	_angle = angle, _rotx = rotx, _roty = roty, _rotz = rotz;
	_scx = scx, _scy = scy, _scz = scz;
}

Model::Model(Model &mdl)
{
//copy constructor
	this->_triangles = mdl._triangles;
	this->_verticies = mdl._verticies;
	this->_uvcoords = mdl._uvcoords;
	this->_normals = mdl._normals;

	this->_x = mdl._x; this->_y = mdl._y; this->_z = mdl._z;
	this->_angle = mdl._angle; this->_rotx = mdl._rotx; this->_roty = mdl._roty; this->_rotz = mdl._rotz;
	this->_scx = mdl._scx; this->_scy = mdl._scy; this->_scz = mdl._scz;
}

Model::~Model(void)
{

}

std::vector<fTriangle>* Model::GetTriangles()
{
//returns triangles
	return &_triangles;
}
std::vector<fVector3d>* Model::GetVerticies()
{
//returns verticies
	return &_verticies;
}
std::vector<fTexCoords3d>* Model::GetTextureCoordinates()
{
//returns uvs
	return &_uvcoords;
}
std::vector<fVector3d>* Model::GetNormals()
{
//returns normals
	return &_normals;
}

int Model::AddVertex(fVector3d &vertexadd)
{
//adds vertices to the list
	_verticies.push_back(vertexadd);
	return 1;
}
int Model::AddUV(fTexCoords3d &texcoord)
{
//adds uvs to the list
	//try catch
	_uvcoords.push_back(texcoord);
	_hasTex = true;
	return 1;
}
int Model::AddTriangle(fTriangle &triangleadd)
{
//adds triangles to teh list
	_triangles.push_back(triangleadd);
	return 1;
}
int Model::AddNormal(fVector3d &normaladd)
{
//adds normals to the list
	_normals.push_back(normaladd);
	_hasNor = true;
	return 1;
}
int Model::AddFace(fTriangle &triangle)
{
//adds faces to the list
	_triangles.push_back(triangle);
	return 1;
}
int Model::Clear()
{
//emptys the varible lists
	_triangles.clear();
	_verticies.clear();
	_uvcoords.clear();
	_normals.clear();
	return 1;
}
void Model::makegeo(float r, float g, float b)
{
//takes the rgb value to color the model with when drawn
	if(_vboInit)
	{ 
	//vbo initilsations
		initVBO(); 
		_vboInit = false; 
	}

	if(!_hasNor && !_hasTex)
	{
	//no normals or uvs so draw the vertices from vbo
		glBindBuffer(GL_ARRAY_BUFFER, verBuff);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, sizeof(fVector3d), 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuff);

		glColor3f(r,g,b);
		glDrawElements(GL_TRIANGLES, _vboInd.size(), GL_UNSIGNED_INT, 0);

		glDisableClientState(GL_VERTEX_ARRAY);
	}
	else if(_hasNor && !_hasTex)
	{
	//no uvs so draw the vertices and the normals from the vbo
		glBindBuffer(GL_ARRAY_BUFFER, verBuff);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, sizeof(fVector3d), 0);

		glBindBuffer(GL_ARRAY_BUFFER, norBuff);
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, sizeof(fVector3d), 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuff);

		glColor3f(r,g,b);
		glDrawElements(GL_TRIANGLES, _vboInd.size(), GL_UNSIGNED_INT, 0);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
	}
	else if(!_hasNor && _hasTex)
	{
	//no normals so draw the vertices and the uvs from the vbo
		glBindBuffer(GL_ARRAY_BUFFER, verBuff);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, sizeof(fVector3d), 0);

		glBindBuffer(GL_ARRAY_BUFFER, texBuff);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, sizeof(fTexCoords3d), 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuff);

		glColor3f(r,g,b);
		glDrawElements(GL_TRIANGLES, _vboInd.size(), GL_UNSIGNED_INT, 0);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}
	else if(_hasNor && _hasTex)
	{
	//draw the vertices and the normals and the uvs from the vbo
		glBindBuffer(GL_ARRAY_BUFFER, verBuff);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, sizeof(fVector3d), 0);

		glBindBuffer(GL_ARRAY_BUFFER, norBuff);
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, sizeof(fVector3d), 0);

		glBindBuffer(GL_ARRAY_BUFFER, texBuff);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, sizeof(fTexCoords3d), 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuff);

		glColor3f(r,g,b);
		glDrawElements(GL_TRIANGLES, _vboInd.size(), GL_UNSIGNED_INT, 0);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}
}

void Model::initVBO()
{
	createIndex();
	//generates vertex buffers
	glGenBuffers(1, &verBuff);
	glBindBuffer(GL_ARRAY_BUFFER, verBuff);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fVector3d) * _vboVer.size(), &_vboVer.front(), GL_STATIC_DRAW);
	
	if(_hasNor)
	{
	//generates normal buffers
		glGenBuffers(1, &norBuff);
		glBindBuffer(GL_ARRAY_BUFFER, norBuff);
		glBufferData(GL_ARRAY_BUFFER, sizeof(fVector3d) * _vboNor.size(), &_vboNor.front(), GL_STATIC_DRAW);
	}

	if(_hasTex) //_has texture
	{
	//gens texture buffer
		glGenBuffers(1, &texBuff);
		glBindBuffer(GL_ARRAY_BUFFER, texBuff);
		glBufferData(GL_ARRAY_BUFFER, sizeof(fTexCoords3d) * _vboTex.size(), &_vboTex.front(), GL_STATIC_DRAW);
	}
//creates the index buffer
	glGenBuffers(1, &indexBuff);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuff);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _vboInd.size() * sizeof(GLuint), &_vboInd.front(), GL_STATIC_DRAW);
}

void Model::createIndex()
{
	if(!_hasNor && !_hasTex)
	{
		//If it doesn't have normals do this. this is more effient!
		for(unsigned int i = 0; i < _verticies.size(); i++)
		{
			_vboVer.push_back(_verticies[i]);
		}
		
		for(unsigned int i = 0; i < _triangles.size(); i++)
		{
			_vboInd.push_back(_triangles[i]._ver.x);
			_vboInd.push_back(_triangles[i]._ver.y);
			_vboInd.push_back(_triangles[i]._ver.z);
		}
	}
	else if(_hasNor && !_hasTex)
	{
		//If it does have normals do this, many more verts but calculates normals correctly.
		for(unsigned int i = 0; i < _triangles.size(); i++)
		{
			_vboVer.push_back(_verticies[_triangles[i]._ver.x]);
			_vboNor.push_back(_normals[_triangles[i]._nor.x]);
			
			_vboVer.push_back(_verticies[_triangles[i]._ver.y]);
			_vboNor.push_back(_normals[_triangles[i]._nor.y]);

			_vboVer.push_back(_verticies[_triangles[i]._ver.z]);
			_vboNor.push_back(_normals[_triangles[i]._nor.z]);
		}

		for(unsigned int i = 0; i < _vboVer.size(); i++)
		{
			_vboInd.push_back(i);
		}

	}
	else if(_hasNor && _hasTex) // not yet working. no textures
	{
		//If it does have normals do this, many more verts but calculates normals correctly.
		for(unsigned int i = 0; i < _triangles.size(); i++)
		{
			_vboVer.push_back(_verticies[_triangles[i]._ver.x]);
			_vboNor.push_back(_normals[_triangles[i]._nor.x]);
			_vboTex.push_back(_uvcoords[_triangles[i]._tex.x]);

			_vboVer.push_back(_verticies[_triangles[i]._ver.y]);
			_vboNor.push_back(_normals[_triangles[i]._nor.y]);
			_vboTex.push_back(_uvcoords[_triangles[i]._tex.y]);

			_vboVer.push_back(_verticies[_triangles[i]._ver.z]);
			_vboNor.push_back(_normals[_triangles[i]._nor.z]);
			_vboTex.push_back(_uvcoords[_triangles[i]._tex.z]);
		}

		for(unsigned int i = 0; i < _vboVer.size(); i++)
		{
		//pushes the index to the list
			_vboInd.push_back(i);
		}
	}
}