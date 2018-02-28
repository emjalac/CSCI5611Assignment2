#ifndef TRIANGLE_INCLUDED
#define TRIANGLE_INCLUDED

#include "Vec3D.h"
#include "Node.h"

class Triangle
{
private:
	Node * v1;
	Node * v2;
	Node * v3;

public:
	//CONSTRUCTORS AND DESTRUCTORS
	Triangle();
	Triangle(Node * _v1, Node * _v2, Node * _v3);
	~Triangle();

	//SETTERS
	void setV1(Node * v);
	void setV2(Node * v);
	void setV3(Node * v);

	//GETTERS
	Node * getV1();
	Node * getV2();
	Node * getV3();

	//OTHERS
	Vec3D calculateNormal();
	Vec3D calculateVelocity();
	Vec3D calculateDrag();

};

#endif