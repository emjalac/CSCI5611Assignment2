#ifndef CLOTH_INCLUDED
#define CLOTH_INCLUDED

#include "Vec3D.h"
#include "Node.h"
#include "Spring.h"

class Cloth
{
private:
	int num_rows;
	int num_cols;
	float unit; //distance between nodes in rows/cols (also: length of spring)
	Vec3D center;
	float tension; //ratio between goal length and actual length of springs in string

	Node ** nodes = NULL;
	Spring ** springs = NULL;

	int start_vertex_index;	//index where vertices start in modelData array for nodes
	int total_vertices;	//total num of vertices within modelData array for nodes

public:
	//CONSTRUCTORS AND DESTRUCTORS
	Cloth();
	Cloth(int rows, int cols);
	Cloth(int rows, int cols, Vec3D pos);
	~Cloth();

	//SETTERS
	void setVertexInfo(int start, int total);

	//GETTERS

	//OTHERS
	void initNodes();
	void initSprings(); //must call initNodes first
	void fixNodes();
	void releaseNodes();
	void draw(GLuint shaderProgram);

};

#endif