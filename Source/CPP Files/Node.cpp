#include "Node.h"

//http://www.idevgames.com/forums/thread-4059.html

Node::Node(void)
{
//default values
	this->mNext = NULL;
	this->gNumDivisions = 5;
	this->mLength = 1;
    this->mPitch = 0;
	this->mYaw = 0;
	this->mR = 1.0f;
	this->mG = 0.0f; 
	this->mB = 0.0f;
}

Node::~Node(void)
{
	//on destroy clean up tree (vectors containing tree parts)
    std::vector<Node*>::iterator itr;
    for (itr = mChildren.begin(); itr != mChildren.end(); ++itr)
	{
        delete (*itr);
	}
        
    if (mNext)
	{
        delete mNext;
	}
}
    
//a simply random function
float Node::randFloat(float min, float max)
{
    if (max <= min)
        return max;
    
    float newRand = (float)(rand() % (int)(max * 100));
    newRand *= 0.01f;
    newRand += min;

    return newRand;
}

//create the nodes/parts of the tree, taking into account the specific parameters
Node* Node::generateNode(float length, float pitch, float yaw, int division, int numChildren, int generation)
{
    Node *newNode = new Node;
    newNode->mLength = length;
    newNode->mPitch = pitch;
    newNode->mYaw = yaw;
    newNode->mR = randFloat(.3, 1);
    newNode->mG = randFloat(.3, 1);
    newNode->mB = randFloat(.3, 1);

    if (division > 0)
    {
		//generate main branch
        newNode->mNext = generateNode(randFloat(MIN_LENGTH, MAX_LENGTH), randFloat(MIN_PITCH, MAX_PITCH), randFloat(0.0f, 360.0f), division - 1, numChildren, generation);
		if (generation <= MAX_GENERATIONS)
        {
            for (int i = 0; i < numChildren; i++)
            {
				//generate child branches
				newNode->mChildren.push_back(generateNode(randFloat(MIN_LENGTH / (float)generation, MAX_LENGTH / (float)generation), randFloat(MIN_PITCH, MAX_PITCH) + randFloat(MIN_CHILD_PITCH_ADJUST, MAX_CHILD_PITCH_ADJUST), randFloat(0.0f, 360.0f), gNumDivisions / (generation + 1), numChildren, generation + 1));
			}
        }
    }
    return newNode;
}
// draw the tree (a line), altering the yaw, pitch and translation each time.
void Node::draw()
{
	glPushMatrix();
		glRotatef(mYaw, 0, 1, 0);
		glRotatef(mPitch, 0, 0, 1);

        glLineWidth(4);
		//collect vbo coords
		glBegin(GL_LINES);
			glColor3f(mR, mG, mB);
			glVertex3f(0, 0, 0);
			glVertex3f(0, mLength, 0);
		glEnd();
		glLineWidth(1);

		glTranslatef(0, mLength, 0);
        
		//reiterate through the draw function until every part has been drawn
		if (!mChildren.empty())
		{
			std::vector<Node*>::iterator itr;
			for (itr = mChildren.begin(); itr != mChildren.end(); ++itr)
			{
				glPushMatrix();
					(*itr)->draw();
				glPopMatrix();
			}
		}
        //move onto the next set of children (parts)
		if (mNext)
		{
			mNext->draw();
		}
	glPopMatrix();
}