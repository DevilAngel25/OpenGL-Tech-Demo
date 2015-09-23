#include "stdafx.h"
#include "TerrainGenerator.h"

TerrainGenerator::TerrainGenerator(void)
{
	scaleValue = 10;
	_vboInit = true;
}

TerrainGenerator::~TerrainGenerator(void)
{
}

void TerrainGenerator::init()
{
	g_HeightMap = loader.Load("Textures\\Terrain.raw");
}

int TerrainGenerator::Height(int X, int Y)				// This Returns The Height From A Height Map Index
{
	int x = X % MAP_SIZE;								// Error Check Our x Value
	int y = Y % MAP_SIZE;								// Error Check Our y Value

	if(!g_HeightMap.size()) return -12345;							// Make Sure Our Data Is Valid
	int tmo = g_HeightMap[x + (y * MAP_SIZE)];
	return 	tmo;			// Index Into Our Height Array And Return The Height
}

void TerrainGenerator::SetVertice(int x, int y, int z)		// Sets The Color Value For A Particular Index, Depending On The Height Index
{
	iVector3d Vertice(x,y,z);

	_vboVer.push_back(Vertice);
}

void TerrainGenerator::SetNormal(float x, float y, float z)		// Sets The Color Value For A Particular Index, Depending On The Height Index
{
	fVector3d Normal(x,y,z);

	_vboNor.push_back(Normal);
}

void TerrainGenerator::SetTex(float u, float v)		// Sets The Color Value For A Particular Index, Depending On The Height Index
{
	fTexCoords3d Tex(u,v);

	_vboTex.push_back(Tex);
}

void TerrainGenerator::RenderHeightMap()					// This Renders The Height Map As Quads
{
	glPushMatrix();
	int X = 0, Y = 0;									// Create Some Variables To Walk The Array With.
	int x, y, z;										// Create Some Variables For Readability
	glScalef(scaleValue, scaleValue * HEIGHT_RATIO, scaleValue);
	glTranslatef(-512,-120,-512);
	if(!g_HeightMap.size()) return;								// Make Sure Our Height Data Is Valid
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	fVector3d first;
	fVector3d second;
	fVector3d third;
	fVector3d a;
	fVector3d b;
	fVector3d n;
	double l;

	if(_vboInit) 
	{ 
		for ( X = 0; X < (MAP_SIZE-STEP_SIZE); X += STEP_SIZE )
		{
			for ( Y = 0; Y < (MAP_SIZE-STEP_SIZE); Y += STEP_SIZE )
			{
				//calcNormals
				first.x = X;
				first.y = Height(X, Y );
				first.z = Y;
				second.x = X;
				second.y = Height(X, Y + STEP_SIZE);
				second.z = Y + STEP_SIZE;
				third.x = X + STEP_SIZE;
				third.y = Height(X + STEP_SIZE, Y );
				third.z = Y;

				a.x = second.x - first.x;
				a.y = second.y - first.y;
				a.z = second.z - first.z;

				b.x = third.x - first.x;
				b.y = third.y - first.y;
				b.z = third.z - first.z;

				n.x = (a.y * b.z) - (a.z * b.y);
				n.y = (a.z * b.x) - (a.x * b.z);
				n.z = (a.x * b.y) - (a.y * b.x);

				// Normalize (divide by root of dot product)
				l = sqrt(n.x * n.x + n.y * n.y + n.z * n.z);
				n.x /= l;
				n.y /= l;
				n.z /= l;

				// Get The (X, Y, Z) Value For The Bottom Left Vertex
				x = X;							
				y = Height(X, Y );	
				z = Y;							

				SetTex(1.0f, 1.0f);
				SetNormal(n.x,n.y,n.z);
				SetVertice(x,y,z);
				
				// Get The (X, Y, Z) Value For The Top Left Vertex
				x = X;										
				y = Height(X, Y + STEP_SIZE );  
				z = Y + STEP_SIZE ;	

				SetTex(0.0f, 1.0f);
				SetNormal(n.x,n.y,n.z);
				SetVertice(x,y,z);							// Send This Vertex a VBO then To OpenGL To Be Rendered

				// Get The (X, Y, Z) Value For The Top Right Vertex
				x = X + STEP_SIZE; 
				y = Height(X + STEP_SIZE, Y + STEP_SIZE ); 
				z = Y + STEP_SIZE ;

				SetTex(0.0f, 0.0f);
				SetNormal(n.x,n.y,n.z);
				SetVertice(x,y,z);
				
				// Get The (X, Y, Z) Value For The Bottom Right Vertex
				x = X + STEP_SIZE; 
				y = Height(X + STEP_SIZE, Y ); 
				z = Y;

				SetTex(1.0f, 0.0f);
				SetNormal(n.x,n.y,n.z);
				SetVertice(x,y,z);
			}
		}
		InitVBO(); 
		_vboInit = false;
	}

	CreateVBO();							// Render Polygon
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glPopMatrix();
}

void TerrainGenerator::InitVBO()
{
	SetInd();
//generates the vertex buffer
	glGenBuffers(1, &verBuff);
	glBindBuffer(GL_ARRAY_BUFFER, verBuff);
	glBufferData(GL_ARRAY_BUFFER, sizeof(iVector3d) * _vboVer.size(), &_vboVer.front(), GL_STATIC_DRAW);
//generates the normal buffer
	glGenBuffers(1, &norBuff);
	glBindBuffer(GL_ARRAY_BUFFER, norBuff);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fVector3d) * _vboNor.size(), &_vboNor.front(), GL_STATIC_DRAW);
//generates the texture buffer	
	glGenBuffers(1, &texBuff);
	glBindBuffer(GL_ARRAY_BUFFER, texBuff);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fTexCoords3d) * _vboTex.size(), &_vboTex.front(), GL_STATIC_DRAW);
//generates the index buffer	
	glGenBuffers(1, &indBuff);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBuff);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _vboInd.size() * sizeof(GLuint), &_vboInd.front(), GL_STATIC_DRAW);
}

void TerrainGenerator::SetInd()
{
	for(unsigned int i = 0; i < _vboVer.size(); i++)
	{
	//pushes the index to the list
		_vboInd.push_back(i);
	}
}

void TerrainGenerator::CreateVBO()
{
//draws the conetents of the vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, verBuff);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_INT, sizeof(iVector3d), 0);
//draws the contents of the normal buffer
		glBindBuffer(GL_ARRAY_BUFFER, norBuff);
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, sizeof(fVector3d), 0);
//draws the contents of the texture buffer 
		glBindBuffer(GL_ARRAY_BUFFER, texBuff);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, sizeof(fTexCoords3d), 0);
//binds the indicies
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBuff);

		glDrawElements(GL_QUADS, _vboInd.size(), GL_UNSIGNED_INT, 0);
//draws and resets state
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}