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
	ks = 1.0f;
	kd = 1.0f;
	length = 10.0f;
}

Spring::Spring(Node * n1, Node * n2)
{
	node1 = n1;
	node2 = n2;
	ks = 1.0f;
	kd = 1.0f;
	length = 10.0f;
}

Spring::Spring(Node * n1, Node * n2, float unit)
{
	node1 = n1;
	node2 = n2;
	ks = 1.0f;
	kd = 1.0f;
	length = unit;
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
	length = l;
}

/*----------------------------*/
// GETTERS
/*----------------------------*/
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
	return length;
}

/*----------------------------*/
// OTHERS
/*----------------------------*/
