#ifndef SPRING_INCLUDED
#define SPRING_INCLUDED

#include "Node.h"

class Spring
{
private:
	Node * node1;
	Node * node2;
	float ks; //spring force constant
	float kd; //spring damping constant
	float rest_len;
	float tension;

public:
	//CONSTRUCTORS AND DESTRUCTORS
	Spring();
	Spring(Node * n1, Node * n2);
	Spring(Node * n1, Node * n2, float unit);
	~Spring();

	//SETTERS
	void setKs(float k);
	void setKd(float k);
	void setRestLen(float l);

	//GETTERS
	Node * getNode1();
	Node * getNode2();
	float getKs();
	float getKd();
	float getRestLen();

	//OTHERS
	Vec3D calculateForce();
	void draw(GLuint shaderprogram);
};

#endif