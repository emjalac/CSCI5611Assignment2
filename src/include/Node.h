#ifndef NODE_INCLUDED
#define NODE_INCLUDED

#include "Vec3D.h"
#include "Material.h"
#include "Util.h"

class Node
{
private:
	Vec3D pos;
	Vec3D vel;
	float mass;
	bool fixed;

	//for drawing
	Material mat;
	Vec3D size;
	int start_vertex_index;	//index where vertices start in modelData array
	int total_vertices;	//total num of vertices within modelData array

public:
	//CONSTRUCTORS AND DESTRUCTORS
	Node();
	Node(Vec3D p);
	Node(Vec3D p, bool f);
	~Node();

	//SETTERS
	void setPos(Vec3D p);
	void setVel(Vec3D v);
	void setMass(float m);
	void setVertexInfo(int start, int total);
	void setMaterial(Material m);
	void setSize(Vec3D s);
	void setColor(Vec3D color); //sets ambient and diffuse to 'color'

	//GETTERS
	Vec3D getPos();
	Vec3D getVel();
	float getMass();
	bool isFixed();
	Material getMaterial();
	Vec3D getSize();

	//OTHERS
	void fix();
	void release();
	void draw(GLuint shaderProgram);

};

#endif