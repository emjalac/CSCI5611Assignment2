#ifdef __APPLE__
#include "include/Triangle.h"
#else
#include "Triangle.h"
#endif

#include <iostream>

/*----------------------------*/
// CONSTRUCTORS AND DESTRUCTORS
/*----------------------------*/
Triangle::Triangle()
{
	v1 = new Node();
	v2 = new Node();
	v3 = new Node();
}

Triangle::Triangle(Node * _v1, Node * _v2, Node * _v3)
{
	v1 = _v1;
	v2 = _v2;
	v3 = _v3;
}

Triangle::~Triangle()
{
}

/*----------------------------*/
// SETTERS
/*----------------------------*/
void Triangle::setV1(Node * v)
{
	v1 = v;
}

void Triangle::setV2(Node * v)
{
	v2 = v;
}

void Triangle::setV3(Node * v)
{
	v3 = v;
}

/*----------------------------*/
// GETTERS
/*----------------------------*/
Node * Triangle::getV1()
{
	return v1;
}

Node * Triangle::getV2()
{
	return v2;
}

Node * Triangle::getV3()
{
	return v3;
}

/*----------------------------*/
// OTHERS
/*----------------------------*/
Vec3D Triangle::calculateNormal()
{
	Vec3D n = cross(v2->getPos() - v1->getPos(), v3->getPos() - v1->getPos());
	float mag = n.getMagnitude();
	return 1/mag * n;
}

Vec3D Triangle::calculateVelocity()
{
	return 1/3 * (v1->getVel()+v2->getVel()+v3->getVel()); 
	//subtract v_air from this quantity if there is wind (add later)
}

Vec3D Triangle::calculateDrag()
{
	//using optimized formula from class slides
	Vec3D vel = calculateVelocity();
	Vec3D n_star = cross(v2->getPos() - v1->getPos(), v3->getPos() - v1->getPos());
	Vec3D van = (vel.getMagnitude() * dotProduct(vel, n_star) / (2 * n_star.getMagnitude())) * n_star;
	//using density of air rho=1.225 and drag coefficient cd=1.0
	return -.5 * 1.225 * van;
}