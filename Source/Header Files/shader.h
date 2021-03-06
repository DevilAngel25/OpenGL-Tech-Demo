#pragma once
#include <string>
#include <stdlib.h>
#include <glew.h>
#include <GL\GL.h>
#include <glut.h>
#include <glext.h>

class Shader {
public:
	Shader();
	Shader(const char *vsFile, const char *fsFile);
	~Shader();
	
    void init(const char *vsFile, const char *fsFile);
	void bind();
	void unbind();
	unsigned int id();
	
private:
	unsigned int shader_id;
	unsigned int shader_vp;
	unsigned int shader_fp;
};
