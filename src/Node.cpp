#ifdef __APPLE__
#include "include/Node.h"
#else
#include "Node.h"
#endif

#include <iostream>

/*----------------------------*/
// CONSTRUCTORS AND DESTRUCTORS
/*----------------------------*/
Node::Node()
{
	pos = Vec3D();
	vel = Vec3D();
	mass = 1.0f;
	fixed = true;
	size = Vec3D(.02, .02, .02);
	start_vertex_index = 0;
	total_vertices = 0;
	mat = Material();
}

Node::Node(Vec3D p)
{
	pos = p;
	vel = Vec3D();
	mass = 1.0f;
	fixed = false;
	size = Vec3D(.02, .02, .02);
	start_vertex_index = 0;
	total_vertices = 0;
	mat = Material();
}

Node::Node(Vec3D p, bool f)
{
	pos = p;
	vel = Vec3D();
	mass = 1.0f;
	fixed = f;
	size = Vec3D(.02, .02, .02);
	start_vertex_index = 0;
	total_vertices = 0;
	mat = Material();
}

Node::~Node()
{
}

/*----------------------------*/
// SETTERS
/*----------------------------*/
void Node::setPos(Vec3D p)
{
	pos = p;
}

void Node::setVel(Vec3D v)
{
	vel = v;
}

void Node::setMass(float m)
{
	mass = m;
}

void Node::setVertexInfo(int start, int total)
{
  start_vertex_index = start;
  total_vertices = total;
}

void Node::setMaterial(Material m)
{
	mat = m;
}

void Node::setSize(Vec3D s)
{
	size = s;
}

void Node::setColor(Vec3D color)
{
	glm::vec3 c = util::vec3DtoGLM(color);
	mat.setAmbient(c);
	mat.setDiffuse(c);
}

/*----------------------------*/
// GETTERS
/*----------------------------*/
Vec3D Node::getPos()
{
	return pos;
}

Vec3D Node::getVel()
{
	return vel;
}

float Node::getMass()
{
	return mass;
}

Material Node::getMaterial()
{
	return mat;
}

Vec3D Node::getSize()
{
	return size;
}

/*----------------------------*/
// OTHERS
/*----------------------------*/
void Node::fix()
{
	fixed = true;
}

void Node::release()
{
	fixed = false;
}

void Node::draw(GLuint shaderProgram)
{
	GLint uniModel = glGetUniformLocation(shaderProgram, "model");

	glm::mat4 model;
	glm::vec3 size_v = util::vec3DtoGLM(size);
	glm::vec3 pos_v = util::vec3DtoGLM(pos);

	//build model mat specific to this WObj
	model = glm::translate(model, pos_v);
	model = glm::scale(model, size_v);
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

	//fragment shader uniforms (from Material)
	GLint uniform_ka = glGetUniformLocation(shaderProgram, "ka");
	GLint uniform_kd = glGetUniformLocation(shaderProgram, "kd");
	GLint uniform_ks = glGetUniformLocation(shaderProgram, "ks");
	GLint uniform_s = glGetUniformLocation(shaderProgram, "s");

	glm::vec3 mat_AMB = mat.getAmbient();
	glUniform3f(uniform_ka, mat_AMB[0], mat_AMB[1], mat_AMB[2]);

	glm::vec3 mat_DIF = mat.getDiffuse();
	glUniform3f(uniform_kd, mat_DIF[0], mat_DIF[1], mat_DIF[2]);

	glm::vec3 mat_SPEC = mat.getSpecular();
	glUniform3f(uniform_ks, mat_SPEC[0], mat_SPEC[1], mat_SPEC[2]);

	glUniform1f(uniform_s, mat.getNS());

	//starts at an offset of start_vertex_index
	//(Primitive Type, Start Vertex, End Vertex)
	glDrawArrays(GL_TRIANGLES, start_vertex_index, total_vertices);
}
