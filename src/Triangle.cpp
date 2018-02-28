#ifdef __APPLE__
#include "include/Triangle.h"
#else
#include "Triangle.h"
#endif

using namespace std;

/*----------------------------*/
// CONSTRUCTORS AND DESTRUCTORS
/*----------------------------*/
Triangle::Triangle()
{
	v1 = new Node();
	v2 = new Node();
	v3 = new Node();
	normal = calculateNormal();
	mat = Material();
}

Triangle::Triangle(Node * _v1, Node * _v2, Node * _v3)
{
	v1 = _v1;
	v2 = _v2;
	v3 = _v3;
	normal = calculateNormal();
	mat = Material();
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

void Triangle::setColor(Vec3D color)
{
	glm::vec3 c = util::vec3DtoGLM(color);
	mat.setAmbient(c);
	mat.setDiffuse(c);
}

void Triangle::setSpecular(Vec3D color)
{
	glm::vec3 c = util::vec3DtoGLM(color);
	mat.setSpecular(c);
}

void Triangle::setNormal(Vec3D n)
{
	normal = n;
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

Vec3D Triangle::getNormal()
{
	return normal;
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
	Vec3D v2an = (vel.getMagnitude() * dotProduct(vel, n_star) / (2 * n_star.getMagnitude())) * n_star;
	//using density of air rho=1.225 and drag coefficient cd=1.0
	return -.5 * 1.225 * v2an;
}

void Triangle::updateNormal()
{
	normal = calculateNormal();
}

void Triangle::draw(GLuint shaderProgram, int i)
{
	Vec3D pos1 = v1->getPos();
	Vec3D pos2 = 2 * v2->getPos() - pos1;
	Vec3D pos3 = 2 * v3->getPos() - pos1;

	float t1, t2, t3, t4, t5, t6;
	int val;
	if (i)
	{
		t1 = 1.0;
		t2 = 0.0;
		t3 = 0.0;
		t4 = 1.0;
		t5 = 0.0;
		t6 = 0.0;
		val = -1;
	}
	else
	{
		t1 = 1.0;
		t2 = 1.0;
		t3 = 1.0;
		t4 = 0.0;
		t5 = 0.0;
		t6 = 1.0;
		val = 1;
	}

	float vertices[24] = {
		pos1.getX(), pos1.getY(), pos1.getZ(), t1, t2, val * normal.getX(), val * normal.getY(), val * normal.getZ(),
		pos2.getX(), pos2.getY(), pos2.getZ(), t3, t4, val * normal.getX(), val * normal.getY(), val * normal.getZ(),
		pos3.getX(), pos3.getY(), pos3.getZ(), t5, t6, val * normal.getX(), val * normal.getY(), val * normal.getZ()
	};

	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), vertices, GL_STATIC_DRAW);

	GLint uniModel = glGetUniformLocation(shaderProgram, "model");

	glm::mat4 model;
	glm::vec3 size_v = util::vec3DtoGLM(Vec3D(.5,.5,.5));
	glm::vec3 pos_v = util::vec3DtoGLM(pos1);

	//build model mat specific to this spring
	model = glm::scale(model, size_v);
	model = glm::translate(model, pos_v);
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

	glDrawArrays(GL_TRIANGLES, 0, 3);
}