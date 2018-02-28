#ifndef TRIANGLE_INCLUDED
#define TRIANGLE_INCLUDED

#include "Vec3D.h"
#include "Node.h"
#include "Material.h"

class Triangle
{
private:
	Node * v1;
	Node * v2;
	Node * v3;
	Vec3D normal;

	//for drawing
	Material mat;

public:
	//CONSTRUCTORS AND DESTRUCTORS
	Triangle();
	Triangle(Node * _v1, Node * _v2, Node * _v3);
	~Triangle();

	//SETTERS
	void setV1(Node * v);
	void setV2(Node * v);
	void setV3(Node * v);
	void setColor(Vec3D color); //sets ambient and diffuse to 'color'
	void setSpecular(Vec3D color); //sets specular to 'color'
	void setNormal(Vec3D n);

	//GETTERS
	Node * getV1();
	Node * getV2();
	Node * getV3();
	Vec3D getNormal();

	//OTHERS
	Vec3D calculateNormal();
	Vec3D calculateVelocity(Vec3D wind);
	Vec3D calculateDrag(Vec3D wind);
	void updateNormal();
	void draw(GLuint shaderProgram, int i);

};

#endif