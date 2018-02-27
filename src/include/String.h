#ifndef STRING_INCLUDED
#define STRING_INCLUDED

#include "Vec3D.h"
#include "Node.h"
#include "Spring.h"
#include "WorldObject.h"

class String
{
private:
	int num_nodes;
	float unit; //distance between nodes in rows/cols (also: length of spring)
	Vec3D center;
	float rest_len;

	int num_springs;
	Node ** nodes = NULL;
	Spring ** springs = NULL;

	int start_vertex_index;	//index where vertices start in modelData array for nodes
	int total_vertices;	//total num of vertices within modelData array for nodes

public:
	//CONSTRUCTORS AND DESTRUCTORS
	String();
	String(int num);
	String(int num, Vec3D pos);
	~String();

	//SETTERS
	void setVertexInfo(int start, int total);

	//GETTERS
	int getNumNodes();
	float getUnit();
	Node * getNode(int i);
	float getRestLen();

	//OTHERS
	void initNodes();
	void initSprings(); //must call initNodes first
	void update(float dt);
	void draw(GLuint shaderProgram, GLuint model_vbo, GLuint line_vbo);

};

#endif