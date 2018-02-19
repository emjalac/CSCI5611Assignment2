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
	float length;

public:
	//CONSTRUCTORS AND DESTRUCTORS
	Spring();
	Spring(Node * n1, Node * n2);
	Spring(Node * n1, Node * n2, float unit);
	~Spring();

	//SETTERS
	void setKs(float k);
	void setKd(float k);
	void setLength(float l);

	//GETTERS
	float getKs();
	float getKd();
	float getLength();

	//OTHERS

};

#endif