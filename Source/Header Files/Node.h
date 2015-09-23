#include <glew.h>
#include <GL/gl.h>
#include <glut.h>
#include <vector>
#include "Model.h"

//http://www.idevgames.com/forums/thread-4059.html

class Node
{
	public:
		Node(void);
		~Node(void);
		
		//Define parameters for generating trees
		#define		MAX_GENERATIONS				5
		#define		MIN_LENGTH					2.0f
		#define		MAX_LENGTH					3.0f
		#define		MIN_PITCH					-2.0f
		#define		MAX_PITCH					2.0f
		#define     MIN_CHILD_PITCH_ADJUST		20.0f
		#define     MAX_CHILD_PITCH_ADJUST		30.0f

		int gNumDivisions;
		float mLength;
		float mPitch, mYaw;
		float mR, mG, mB;
		Node *mNext;
		std::vector<Node*> mChildren;

		float randFloat(float min, float max);
		Node* generateNode(float length, float pitch, float yaw, int division, int numChildren, int generation);
		void draw();
};