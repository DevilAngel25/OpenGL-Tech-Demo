#if ( (defined(__MACH__)) && (defined(__APPLE__)) )   
#include <stdlib.h>
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include <OpenGL/glext.h>
#else
#include <stdlib.h>
#include <glew.h>
#include <GL/gl.h>
#include <glut.h>
#include <glext.h>
#endif

#include "Shader.h"
#include "model.h"
#include "ObjLoader.h"
#include "TerrainGenerator.h"
#include "Node.h"

#define RENDER_HEIGHT 800
#define RENDER_WIDTH 1024

#pragma comment (lib,"glew32.lib")

Model one(1000, 0.0f, 0.0f, 100.0f, 0.0f, 1.0f, 0.0f, 20.0f, 30.0f, 20.0f);		
Model two(-280.0f, 0.0f, 50.0f, 0.0f, 0.0f, 1.0f, 0.0f, 10.0f, 10.0f, 10.0f);
Model three(30.0f, 0.0f, -1000.0f, 20.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f);

Model *object = NULL;

Node * gRoot = new Node;

ObjLoader load;
TerrainGenerator genTerrain;
Shader shader;

GLuint texture[15];
int indexoftex = 0;

float ly = -6, lx = 40, lz = -30, lw = 0, lyy =5,lxx = 70, lzz = 35;
float t_ly = ly, t_lx = lx, t_lz = lz, t_lw = lw, t_lyy =lyy ,t_lxx = lxx, t_lzz = lzz;
GLfloat light_pos[] = {lx, ly, lz, 1.0f};
GLfloat light_posx[] = {lxx, lyy, lzz, 1.0f};
GLfloat light_avg[] = {10.0f, 55.0f, -50.0f, 1.0f};
float l_light[3] = {0,0,-1};//Light lookAt
bool showlightpos = false;

//camera vectors
fVector3d camera_pos(30,20,-20);
fVector3d camera_dir(0,0,0);
fVector3d camera_right(0,0,0);
fVector3d camera_up(0,1,0);

GLfloat angle = 0.0; //set the angle of rotation

float oldTime = 0;
float currentTime = 0;
float deltaTime = 0;

float mouseSpeed = 0.0001f;
float mouseX = 0.0f;
float mouseY = 0.0f;
bool mouseWarp = false;
float angle_Y = 3.14f, angle_X = 0.0f;
float speed = 1.0f;

bool cameraMove = true;
bool translate = true;

bool genTree = false;
int	gNumDivisions = 5;

void camera(int x, int y);
void pick(int x, int y);
void moveObject(Model &object, int x, int y);
void generateTree();

GLuint LoadTexture( const char * filename, int width, int height )
{
	GLuint texture;
	unsigned char * data;
	FILE * file;
    
	//The following code will read in our RAW file
	file = fopen( filename, "rb" );
	if ( file == NULL ) return 0;
	data = (unsigned char *)malloc( width * height * 3 );
	fread( data, width * height * 3, 1, file );
	fclose( file );
    
	glGenTextures( 1, &texture ); //generate the texture with the loaded data
	glBindTexture( GL_TEXTURE_2D, texture ); //bind the texture to it's array
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE ); //set texture environment parameters
    
	//And if you go and use extensions, you can use Anisotropic filtering textures which are of an
	//even better quality, but this will do for now.
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    
	//Here we are setting the parameter to repeat the texture instead of clamping the texture
	//to the edge of our shape. 
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    
	//Generate the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	free( data ); //free the texture
    
	return texture; //return whether it was successfull
}

void FreeTexture( GLuint texture )
{
    glDeleteTextures( 1, &texture ); 
}

void init(void) 
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    camera_pos.y = camera_pos.y <= (genTerrain.Height(512+camera_pos.x/10,512+camera_pos.z/10)*(10*0.8) -600) ? (genTerrain.Height(512+camera_pos.x/10,512+camera_pos.z/10)*(10*0.8)-600) : camera_pos.y;


//load the textures
	shader.init("shaderjusttex.vert", "shaderjusttex.frag");
    texture[0] = LoadTexture("Textures\\Barel_blue_D.raw", 1024,1024);
	texture[1] = LoadTexture("Textures\\right.raw", 512, 512);
	texture[2] = LoadTexture("Textures\\left.raw", 512, 512);
	texture[3] = LoadTexture("Textures\\front.raw", 512, 512);
	texture[4] = LoadTexture("Textures\\back.raw", 512, 512);
	texture[5] = LoadTexture("Textures\\down.raw", 512, 512);
	texture[6] = LoadTexture("Textures\\top.raw", 512, 512);
	texture[7] = LoadTexture("Textures\\Untitled1.raw", 256, 256);
	texture[8] = LoadTexture("Textures\\Gotham.raw", 512, 512);
	texture[9] = LoadTexture("Textures\\statue.raw", 512, 512);
}

void setLighting(void) 
{
	 glEnable(GL_LIGHTING);
     glEnable(GL_LIGHT0);
     glEnable(GL_NORMALIZE);	
	 light_pos[0] = -3000;//lyy;
	 light_pos[1] = 2000;//lxx;
	 light_pos[2] = 2500;//lzz;

	// Light model parameters:
     // -------------------------------------------
     
     GLfloat lmKa[] = {1.0, 0.0, 0.0, 0.0 };
     glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmKa);
     
     glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0);
     glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0.0);
     
     // -------------------------------------------
     // Spotlight Attenuation
     
     GLfloat spot_direction[] = { one._x,  one._y,  one._z };
     GLint spot_exponent = 30;
     GLint spot_cutoff = 100;
     
	 if(showlightpos)
	 {
		glPushMatrix();
			glBegin(GL_LINES);
			  glColor3f(1,1,1);
			  glVertex3f(light_pos[0],light_pos[1],light_pos[2]);
			  glColor3f(1,1,1);
			  glVertex3f(spot_direction[0],spot_direction[1],spot_direction[2]);
			  glEnd();
		glPopMatrix();

		glPushMatrix();
			 glTranslatef(light_pos[0],light_pos[1],light_pos[2]);
			 glColor3f(1,1,1);
			glutSolidCube(100);	
		glPopMatrix();
	 }

     glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
     glLighti(GL_LIGHT0, GL_SPOT_EXPONENT, spot_exponent);
     glLighti(GL_LIGHT0, GL_SPOT_CUTOFF, spot_cutoff);
    
     GLfloat Kc = 1.0;
     GLfloat Kl = 0.0;
     GLfloat Kq = 0.0;
     
     glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION,Kc);
     glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, Kl);
     glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, Kq);
     
     
     // ------------------------------------------- 
     // Lighting parameters:


     GLfloat light_Ka[]  = {1.0f, 0.0f, 0.0f, 1.0f};
     GLfloat light_Kd[]  = {1.0f, 0.0f, 0.0f, 1.0f};
     GLfloat light_Ks[]  = {0.0f, 1.0f, 1.0f, 1.0f};

     glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
     glLightfv(GL_LIGHT0, GL_AMBIENT, light_Ka);
     glLightfv(GL_LIGHT0, GL_DIFFUSE, light_Kd);
     glLightfv(GL_LIGHT0, GL_SPECULAR, light_Ks);

     // -------------------------------------------
     // Material parameters:

     GLfloat material_Ka[] = {0.0f, 0.0f, 0.0f, 1.0f};
     GLfloat material_Kd[] = {0.8f, 0.3f, 0.3f, 0.60f};
     GLfloat material_Ks[] = {0.8f, 0.8f, 0.0f, 1.0f};
     GLfloat material_Ke[] = {0.1f, 0.0f, 0.0f, 0.0f};
     GLfloat material_Se = 20.0f;

     glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_Ka);
     glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_Kd);
     glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_Ks);
     glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material_Ke);
     glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material_Se);
}

void skybox (void) 
{
//sets the skybox size
	int skyboxsize = 11000;
//load the correct shader change culling and draw the skybox with texture
	shader.init("shaderjusttex.vert","shaderjusttex.frag");
	shader.bind();
		glActiveTexture(GL_TEXTURE0);
		int texture_location = glGetUniformLocation(shader.id(), "texture_color");
		glUniform1i(texture_location, 0);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glScalef(-1,-1,-1);
	
		glBindTexture(GL_TEXTURE_2D, texture[4]);
		glBegin(GL_QUADS);
			// Front Face
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-skyboxsize, -skyboxsize,  skyboxsize);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( skyboxsize, -skyboxsize,  skyboxsize);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( skyboxsize,  skyboxsize,  skyboxsize);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-skyboxsize,  skyboxsize,  skyboxsize);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, texture[3]);
		glBegin(GL_QUADS);
			// Back Face
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-skyboxsize, -skyboxsize, -skyboxsize);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-skyboxsize,  skyboxsize, -skyboxsize);
			glTexCoord2f(0.0f, 1.0f); glVertex3f( skyboxsize,  skyboxsize, -skyboxsize);
			glTexCoord2f(0.0f, 0.0f); glVertex3f( skyboxsize, -skyboxsize, -skyboxsize);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, texture[5]);
		glBegin(GL_QUADS);
			// Top Face
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-skyboxsize,  skyboxsize, -skyboxsize);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-skyboxsize,  skyboxsize,  skyboxsize);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( skyboxsize,  skyboxsize,  skyboxsize);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( skyboxsize,  skyboxsize, -skyboxsize);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, texture[6]);
		glBegin(GL_QUADS);
			// Bottom Face
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-skyboxsize, -skyboxsize, -skyboxsize);
			glTexCoord2f(0.0f, 1.0f); glVertex3f( skyboxsize, -skyboxsize, -skyboxsize);
			glTexCoord2f(0.0f, 0.0f); glVertex3f( skyboxsize, -skyboxsize,  skyboxsize);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-skyboxsize, -skyboxsize,  skyboxsize);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, texture[2]);
		glBegin(GL_QUADS);
			// Right face
			glTexCoord2f(1.0f, 0.0f); glVertex3f( skyboxsize, -skyboxsize, -skyboxsize);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( skyboxsize,  skyboxsize, -skyboxsize);
			glTexCoord2f(0.0f, 1.0f); glVertex3f( skyboxsize,  skyboxsize,  skyboxsize);
			glTexCoord2f(0.0f, 0.0f); glVertex3f( skyboxsize, -skyboxsize,  skyboxsize);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glBegin(GL_QUADS);
			// Left Face
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-skyboxsize, -skyboxsize, -skyboxsize);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-skyboxsize, -skyboxsize,  skyboxsize);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-skyboxsize,  skyboxsize,  skyboxsize);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-skyboxsize,  skyboxsize, -skyboxsize);
		glEnd();
	shader.unbind();
}

void startTransform(float x,float y,float z, float angle, float rotx,float roty,float rotz,float scx,float scy,float scz)
{
	glPushMatrix();
		glTranslatef(x,y,z);
		glRotatef(angle,rotx,roty,rotz);
		glScalef(scx,scy,scz);
}

void endTransform()
{
	glPopMatrix();
}

void drawmodels()
{
//load correct shaders then bind texture and draw terrain
	shader.init("shader1.vert","shader1.frag");
	shader.bind();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture[7]);
	
		glScalef(-1,-1,-1);
		genTerrain.RenderHeightMap();
	shader.unbind();
//load correct shaders then bind texture and draw building		
	glCullFace(GL_BACK);
	glBindTexture(GL_TEXTURE_2D, texture[8]);
	shader.init("shader1.vert", "shader1.frag");
	shader.bind();
		startTransform(two._x, two._y, two._z, two._angle, two._rotx, two._roty, two._rotz, two._scx, two._scy, two._scz);
			two.makegeo(255,255,255);
		endTransform();	
	shader.unbind();
//load correct shaders then bind texture and draw statue
	glBindTexture(GL_TEXTURE_2D, texture[9]);
	shader.init("shader1.vert", "shader1.frag");
	shader.bind();
		startTransform(three._x, three._y, three._z, three._angle, three._rotx, three._roty, three._rotz, three._scx, three._scy, three._scz);
			three.makegeo(255,255,255);
		endTransform();
	shader.unbind();
//change the matterial for the next model
	 GLfloat material_Ka[] = {0.3f, 0.2f, 0.0f, 1.0f};
     GLfloat material_Kd[] = {0.8f, 0.3f, 0.3f, 0.60f};
     GLfloat material_Ks[] = {0.8f, 0.8f, 0.0f, 1.0f};
     GLfloat material_Ke[] = {0.0f, 0.0f, 0.0f, 0.0f};
     GLfloat material_Se = 20.0f;

     glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_Ka);
     glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_Kd);
     glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_Ks);
     glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material_Ke);
     glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material_Se);
//load correct shaders then bind texture and draw the barrel
	 glCullFace(GL_BACK);
	glBindTexture(GL_TEXTURE_2D, texture[0]);	
	shader.init("shader1.vert", "shader1.frag");
	shader.bind();
	//barrel one
		startTransform(one._x, one._y, one._z, one._angle, one._rotx, one._roty, one._rotz, one._scx, one._scy, one._scz);
			one.makegeo(255,255,255);
		endTransform();
	shader.unbind();
}


void generateTree()
{
//Generate the Tree
	gRoot = gRoot->generateNode(1, 0, 0, gNumDivisions, 1, 1);
}


void drawTree()
{
//Draw the Tree
	if(!genTree)
	{
		generateTree();
		//only generate once
		genTree = true;
	}
	//Draw 2 trees (can be ajusted)
	for(int i = 0; i < 1; i++)
	{
		for(int j = 0; j < 2; j++)
		{
			glPushMatrix();
				glScalef(-10.0f,-10.0f,-10.0f);
				glColor3f(0.0f, 1.0f, 0.0f);
				glTranslatef(50.0f + (j*100), -2.0f, 80.0f + (i*50)); 
				glBindTexture(GL_TEXTURE_2D, texture[0]);	
				shader.init("shaderjusttex.vert", "shaderjusttex.frag");
				shader.bind();
					gRoot->draw();
				shader.unbind();
			glPopMatrix;	
		}
	}
}

void setupMatrices(	float position_x,float position_y,float position_z, float lookAt_x,float lookAt_y,float lookAt_z)
{
//setup matricies for the camera
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45,RENDER_WIDTH/RENDER_HEIGHT,0.1,1000000);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(	position_x,		position_y,		position_z,
				lookAt_x,		lookAt_y,		lookAt_z,
				camera_up.x,	camera_up.y,	camera_up.z);
}

void display(void) 
{
//keeps speed of movement the same
	oldTime = currentTime;
	currentTime = glutGet(GLUT_ELAPSED_TIME);
    deltaTime = currentTime - oldTime;
	
	glClearColor (0.0,0.0,0.0,1.0);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();  
	
	setupMatrices(	camera_pos.x,					camera_pos.y,					camera_pos.z,
					camera_pos.x + camera_dir.x,	camera_pos.y + camera_dir.y,	camera_pos.z + camera_dir.z);
	setLighting();
	glEnable(GL_CULL_FACE);
	
	glCullFace(GL_FRONT);
//render the skybox
	skybox();
//render the tree
	drawTree();
//render the models
	drawmodels();
	glutSwapBuffers();
//for rotation
	angle += 1.10f;
}

void reshape (int w, int h) 
{
//adjusts the content when window changes size
	glViewport (0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective (45, (GLfloat)w / (GLfloat)h, 0.1, 1000000);
	glMatrixMode (GL_MODELVIEW);
	gluLookAt(	camera_pos.x,					camera_pos.y,					camera_pos.z,
				camera_pos.x + camera_dir.x,	camera_pos.y + camera_dir.y,	camera_pos.z + camera_dir.z,
				camera_up.x,					camera_up.y,					camera_up.z);
}

void processNormalKeys(unsigned char key, int x, int y) 
{
//process the keypresses
	switch(key)
	{
		case 27: { exit(0); break;}
		case 'l': { showlightpos = !showlightpos; break;}
		//Main_Camera
		case 'w': 
			{
				//glTranslatef(-512,-120,-512);
				camera_pos.y = camera_pos.y >= 5000?5000 : camera_pos.y;
				camera_pos.y = camera_pos.y <= (genTerrain.Height(512+camera_pos.x/10,512+camera_pos.z/10)*(10*0.8) -600) ? (genTerrain.Height(512+camera_pos.x/10,512+camera_pos.z/10)*(10*0.8)-600) : camera_pos.y;
				//if the camera is allowed to move
				if(cameraMove)
				{
					//move camera forwards
					camera_pos.x += camera_dir.x * deltaTime * speed;
					camera_pos.y += camera_dir.y * deltaTime * speed; 
					camera_pos.z += camera_dir.z * deltaTime * speed; 
				}
				else 
				{ 
					//if cameraMove if false, a model has been selected use the pointer to the model to start to minipulate it.
					if(translate) { object->_y += 10.0f; } 
					else 
					{ 
						object->_rotx = 0.0f; 
						object->_roty = 1.0f; 
						object->_rotz = 0.0f; 
						object->_angle += 10.0f; 
					} 
				}
				break;
			}
		case 's': 
			{ 
				camera_pos.y = camera_pos.y >= 1000?1000 : camera_pos.y;
				camera_pos.y = camera_pos.y <= (genTerrain.Height(512+camera_pos.x/10,512+camera_pos.z/10)*(10*0.8) -600) ? (genTerrain.Height(512+camera_pos.x/10,512+camera_pos.z/10)*(10*0.8)-600) : camera_pos.y;

				//if the camera is allowed to move
				if(cameraMove)
				{
					camera_pos.x -= camera_dir.x * deltaTime * speed; 
					camera_pos.y -= camera_dir.y * deltaTime * speed;
					camera_pos.z -= camera_dir.z * deltaTime * speed;
				}
				else 
				{ 
					if(translate) { object->_y -= 10.0f; } 
					else 
					{ 
						object->_rotx = 0.0f; 
						object->_roty = 1.0f; 
						object->_rotz = 0.0f; 
						object->_angle -= 10.0f; 
					}
				}
				break;
			}
		case 'd': 
			{ 
				//if the camera is allowed to move
				if(cameraMove)
				{
					camera_pos.x += camera_right.x * deltaTime * speed; 
					camera_pos.y += camera_right.y * deltaTime * speed;
					camera_pos.z += camera_right.z * deltaTime * speed;
				}
				else 
				{ 
					if(translate) { object->_x -= 10.0f; } 
					else 
					{ 
						object->_rotx = 1.0f; 
						object->_roty = 0.0f; 
						object->_rotz = 0.0f; 
						object->_angle -= 10.0f; 
					}
				}
				break;
			}
		case 'a': 
			{ 
				//if the camera is allowed to move
				if(cameraMove)
				{
					camera_pos.x -= camera_right.x * deltaTime * speed; 
					camera_pos.y -= camera_right.y * deltaTime * speed;
					camera_pos.z -= camera_right.z * deltaTime * speed;
				}
				else 
				{ 
					if(translate) { object->_x += 10.0f; } 
					else 
					{ 
						object->_rotx = 1.0f; 
						object->_roty = 0.0f; 
						object->_rotz = 0.0f; 
						object->_angle += 10.0f; 
					}
				}
				break;
			}
		case 'q': 
			{ 
				//if the camera is allowed to move
				if(cameraMove){}
				else 
				{ 
					if(translate) { object->_z += 10.0f; } 
					else 
					{ 
						object->_rotx = 0.0f; 
						object->_roty = 0.0f; 
						object->_rotz = 1.0f; 
						object->_angle += 10.0f; 
					}
				}
				break;
			}
		case 'e': 
			{ 
				//if the camera is allowed to move
				if(cameraMove){}
				else
				{ 
					if(translate) { object->_z -= 10.0f; } 
					else 
					{ 
						object->_rotx = 0.0f; 
						object->_roty = 0.0f; 
						object->_rotz = 1.0f; 
						object->_angle -= 10.0f; 
					}
				}
				break;
			}
		case 'z': 
			{ 
				//if the camera is allowed to move
				if(cameraMove){}
				else { translate = !translate; }
				break;
			}
		case 'x': 
			{ 
				//regenerate the trees
				genTree = false;
				break;
			}
		case '#': 
			{ 
				lxx = t_lxx; lx = t_lx;	 
				lyy = t_lyy; ly = t_ly;	  
				lzz = t_lzz; lz = t_lz;  
				break;
			}
		case 32:
			{
				if(cameraMove)
				{
					camera_pos.y+=10;
				}

				camera_pos.y = camera_pos.y >= 1000?1000 : camera_pos.y;
				camera_pos.y = camera_pos.y <= (genTerrain.Height(512+camera_pos.x/10,512+camera_pos.z/10)*(10*0.8) -600) ? (genTerrain.Height(512+camera_pos.x/10,512+camera_pos.z/10)*(10*0.8)-600) : camera_pos.y;

				break;
			}
	}
}

void camera(int x, int y)
{
	//if the mouse hasn't moved
	if (!mouseWarp)
	{	
		//if the player hasn't moved the mouse, no need to calculate view.
		float diffx = x - mouseX; 
		float diffy = y - mouseY; 
		if( diffx == 0 && diffy == 0) { return; }

		//centre the mouse so it doesn't run off of the screen.
		if(x != RENDER_WIDTH/2 || y != RENDER_HEIGHT/2)
		{
			glutWarpPointer((int)RENDER_WIDTH/2, (int)RENDER_HEIGHT/2); //warp mouse
			mouseWarp = true;
		}

		mouseX = x;
		mouseY = y;

		angle_Y += mouseSpeed * deltaTime * float(RENDER_WIDTH/2 - mouseX );
		angle_X += mouseSpeed * deltaTime * float(RENDER_HEIGHT/2 - mouseY );

		camera_dir.x = cos(angle_X) * sin(angle_Y);
		camera_dir.y = sin(angle_X);
		camera_dir.z = cos(angle_X) * cos(angle_Y);

		camera_right.x = sin(angle_Y - 3.14f/2.0f);
		camera_right.y = 0;
		camera_right.z = cos(angle_Y - 3.14f/2.0f);

		camera_up = camera_right.crossProduct(camera_dir);
		camera_up.y = -camera_up.y;
	}
	else
	{
		mouseWarp = false;
		mouseX = x;
		mouseY = y;
	}
}

void mouseMove(int x, int y)
{
//works camera and object selection
	camera(x,y);
	moveObject(*object, x, y);
}

void moveObject(Model &object, int x, int y)
{
//disable/enavbles the camera movement 
	if(&object == NULL)
	{
		cameraMove = true;
		return;
	}
	cameraMove = false;
}

void mouseClicks(int button, int state, int x, int y)
{
//picks at the mouse pos
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) 
	{
		pick(x, y);	
	}
}

void pick(int x, int y)
{
	//clear buffers and disable lighting ect, may effect color.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DITHER);
	glDisable(GL_LIGHTING);

	GLint viewport[4];
	GLubyte pixel[3];
	
	//get the view port
	glGetIntegerv(GL_VIEWPORT,viewport);

	//draw the models in the specific solid color
	startTransform(one._x, one._y, one._z, one._angle, one._rotx, one._roty, one._rotz, one._scx, one._scy, one._scz);
		one.makegeo(0,255,0);
	endTransform();

	startTransform(two._x, two._y, two._z, two._angle, two._rotx, two._roty, two._rotz, two._scx, two._scy, two._scz);
		two.makegeo(255,0,0);
	endTransform();

	startTransform(three._x, three._y, three._z, three._angle, three._rotx, three._roty, three._rotz, three._scx, three._scy, three._scz);
		three.makegeo(0,0,255);
	endTransform();

	//make sure to read back buffer pixels (default)
	glReadBuffer(GL_BACK);
	//read pixels at mouse x, y
	glReadPixels(x, viewport[3]-y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, (void *)pixel);

	printf("%d %d %d\n",pixel[0],pixel[1],pixel[2]);

	//check pixels, if a match is found set the pointer to that model
	if (pixel[0] == 0 && pixel[1] == 255 && pixel[2] == 0) { printf ("You picked the 1st Object");  object = &one;}
	else if (pixel[0] == 255 && pixel[1] == 0 && pixel[2] == 0) { printf ("You picked the 2nd Object"); object = &two; }
	else if (pixel[0] == 0 && pixel[1] == 0 && pixel[2] == 255) { printf ("You picked the 3rd Object"); object = &three; }
	else { printf("You didn't click anything!"); object = NULL;}

	printf ("\n");

	glEnable(GL_DITHER);
	glEnable(GL_LIGHTING);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

int main (int argc, char **argv) 
{
//load the models
	load.Load("Models\\Barel_blue.obj", one);
	load.Load("Models\\building.obj", two);
	load.Load("Models\\statue.obj", three);
//init the terrain
	genTerrain.init();
//glut init stuff
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); //set up the double buffering
	glutInitWindowSize((int)RENDER_WIDTH,(int)RENDER_HEIGHT);
	glutCreateWindow("Interactive TechDemo : Height Map Terrain : Lighting : Color Picking : Camera : Procedural Trees : Ground Collision");
	glutInitWindowPosition(100, 100);

	//glutSetCursor(GLUT_CURSOR_NONE);
	glutWarpPointer((int)RENDER_WIDTH/2, (int)RENDER_HEIGHT/2); //Set cursor to the centre of the screen
    
    glutDisplayFunc(display);
	glutIdleFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouseClicks);
    glutKeyboardFunc(processNormalKeys);
	glutMotionFunc(mouseMove);
	glutPassiveMotionFunc(mouseMove);
//run initilasation code
	init();
    
    
    glutMainLoop();
    
    return 0;
}

