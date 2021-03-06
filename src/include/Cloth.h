#ifndef CLOTH_INCLUDED
#define CLOTH_INCLUDED

#include "Vec3D.h"
#include "Node.h"
#include "Spring.h"
#include "Triangle.h"
#include "WorldObject.h"

class Cloth
{
private:
	int num_rows;
	int num_cols;
	float unit; //distance between nodes in rows/cols (also: length of spring)
	Vec3D center;
	float tension; //ratio between goal length and actual length of springs in string

	int num_nodes;
	int num_springs;
	int num_triangles;
	Node *** nodes = NULL;
	Spring ** springs = NULL;
	Triangle ** triangles = NULL;

	bool drag;

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
	void setDrag(bool b);

	//GETTERS
	bool getDrag();

	//OTHERS
	void initNodes();
	void initSprings(); //must call initNodes first
	void initTriangles(); //must call initNodes first
	void updateTriangleNormals();
	void fixNodes();
	void releaseNodes();
	void update(WorldObject ** wobjs, int num_wobjs, Vec3D g_force, Vec3D wind, float dt);
	void drawNodes(GLuint shaderProgram, GLuint model_vbo, GLuint line_vbo);
	void drawTriangles(GLuint shaderProgram, GLuint model_vbo, GLuint line_vbo);

};

#endif