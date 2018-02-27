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
	tension = 0.85f;
	rest_len = tension * 1.0f;
}

Spring::Spring(Node * n1, Node * n2)
{
	node1 = n1;
	node2 = n2;
	ks = 200.0f;
	kd = 200.0f;
	tension = 0.85f;
	rest_len = tension * 1.0f;
}

Spring::Spring(Node * n1, Node * n2, float unit)
{
	node1 = n1;
	node2 = n2;
	ks = 200.0f;
	kd = 200.0f;
	tension = 0.85f;
	rest_len = tension * unit;
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

void Spring::setRestLen(float l)
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

float Spring::getRestLen()
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
	Vec3D dir = (1 / rest_len) * spring_vec;
	float vel1 = dotProduct(dir, node1->getVel());
	float vel2 = dotProduct(dir, node2->getVel());
	float spring_force = -ks * (rest_len - spring_len);
	float damping_force = -kd * (vel1 - vel2);
	Vec3D force = (spring_force - damping_force) * dir;
	return force;
}

void Spring::draw(GLuint shaderProgram)
{
	Vec3D pos1 = node1->getPos();
	Vec3D pos2 = 2 * node2->getPos() - pos1;

	float vertices[6] = {
		pos1.getX(), pos1.getY(), pos1.getZ(),
		pos2.getX(), pos2.getY(), pos2.getZ()
	};

	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), vertices, GL_STATIC_DRAW);

	GLint uniModel = glGetUniformLocation(shaderProgram, "model");

	glm::mat4 model;
	glm::vec3 size_v = util::vec3DtoGLM(Vec3D(.5,.5,.5));
	glm::vec3 pos_v = util::vec3DtoGLM(pos1);

	//build model mat specific to this spring
	model = glm::scale(model, size_v);
	model = glm::translate(model, pos_v);
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

	glDrawArrays(GL_LINES, 0, 2);
}
