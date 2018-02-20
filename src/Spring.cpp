#ifdef __APPLE__
#include "include/Spring.h"
#else
#include "Spring.h"
#endif

#include <cmath>
#include <iostream>

/*----------------------------*/
// CONSTRUCTORS AND DESTRUCTORS
/*----------------------------*/
Spring::Spring()
{
	node1 = new Node();
	node2 = new Node();
	ks = 200.0f;
	kd = 200.0f;
	rest_len = 1.0f;
}

Spring::Spring(Node * n1, Node * n2)
{
	node1 = n1;
	node2 = n2;
	ks = 200.0f;
	kd = 200.0f;
	rest_len = 1.0f;
}

Spring::Spring(Node * n1, Node * n2, float unit)
{
	node1 = n1;
	node2 = n2;
	ks = 200.0f;
	kd = 200.0f;
	rest_len = unit;
}

Spring::~Spring()
{
}

/*----------------------------*/
// SETTERS
/*----------------------------*/
void Spring::setKs(float k)
{
	ks = k;
}

void Spring::setKd(float k)
{
	kd = k;
}

void Spring::setLength(float l)
{
	rest_len = l;
}

/*----------------------------*/
// GETTERS
/*----------------------------*/
Node * Spring::getNode1()
{
	return node1;
}

Node * Spring::getNode2()
{
	return node2;
}

float Spring::getKs()
{
	return ks;
}

float Spring::getKd()
{
	return kd;
}

float Spring::getLength()
{
	return rest_len;
}

/*----------------------------*/
// OTHERS
/*----------------------------*/
Vec3D Spring::calculateForce()
{
	Vec3D spring_vec = node1->getPos() - node2->getPos();
	float spring_len = spring_vec.getMagnitude();
	//printf("spring length is %f\n", spring_len);
	Vec3D dir = (1 / spring_len) * spring_vec;
	float vel1 = dotProduct(dir, node1->getVel());
	float vel2 = dotProduct(dir, node2->getVel());
	float spring_force = -ks * (rest_len - spring_len);
	float damping_force = -kd * (vel1 - vel2);
	Vec3D force = (spring_force - damping_force) * dir;
	return force;
}