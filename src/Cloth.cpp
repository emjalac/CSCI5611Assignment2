#ifdef __APPLE__
#include "include/Cloth.h"
#else
#include "Cloth.h"
#endif

#include <iostream>

/*----------------------------*/
// CONSTRUCTORS AND DESTRUCTORS
/*----------------------------*/
Cloth::Cloth()
{
	num_rows = 10;
	num_cols = 10;
	unit = 0.1f;
	center = Vec3D(0, 1, 0);
	tension = 0.9f; //ratio between goal length and actual length of springs in row/col

	start_vertex_index = 0;
	total_vertices = 0;

	nodes = new Node*[num_rows * num_cols];
	springs = new Spring*[(num_rows-1) * num_cols + (num_cols-1) * num_rows];
}

Cloth::Cloth(int rows, int cols)
{
	num_rows = rows;
	num_cols = cols;
	unit = 1.0f;
	center = Vec3D(0, 1, 0);
	tension = 0.9f; //ratio between goal length and actual length of springs in row/col

	start_vertex_index = 0;
	total_vertices = 0;

	nodes = new Node*[num_rows * num_cols];
	springs = new Spring*[(num_rows-1) * num_cols + (num_cols-1) * num_rows];
}

Cloth::Cloth(int rows, int cols, Vec3D pos)
{
	num_rows = rows;
	num_cols = cols;
	unit = 1.0f;
	center = pos;
	tension = 0.9f; //ratio between goal length and actual length of springs in row/col

	start_vertex_index = 0;
	total_vertices = 0;

	nodes = new Node*[num_rows * num_cols];
	springs = new Spring*[(num_rows-1) * num_cols + (num_cols-1) * num_rows];
}

Cloth::~Cloth()
{
	int num_nodes = num_rows * num_cols;
	int num_springs = (num_rows-1) * num_cols + (num_cols-1) * num_rows;
	for (int i = 0; i < num_nodes; i++)
	{
		delete nodes[i];
	}
	for (int i = 0; i < num_springs; i++)
	{
		delete springs[i];
	}
}

/*----------------------------*/
// SETTERS
/*----------------------------*/
void Cloth::setVertexInfo(int start, int total)
{
  start_vertex_index = start;
  total_vertices = total;
}

/*----------------------------*/
// GETTERS
/*----------------------------*/


/*----------------------------*/
// OTHERS
/*----------------------------*/
void Cloth::initNodes()
{
	Vec3D temp_pos = Vec3D(center.getX() - (num_cols-1)/2 * unit, center.getY(), center.getZ() - (num_rows-1)/2 * unit);
	for (int i = 0; i < num_rows; i++)
	{
		for (int j = 0; j < num_cols; j++)
		{
			Vec3D pos = temp_pos + Vec3D(j * unit, 0, i * unit);
			if (i == 0)
			{
				nodes[i * num_cols + j] = new Node(pos, true);
				nodes[i * num_cols + j]->setVertexInfo(start_vertex_index, total_vertices);
				nodes[i * num_cols + j]->setColor(Vec3D(0.5, 0, 1));
			}
			else
			{
				nodes[i * num_cols + j] = new Node(pos, false);
				nodes[i * num_cols + j]->setVertexInfo(start_vertex_index, total_vertices);
				nodes[i * num_cols + j]->setColor(Vec3D(0.5, 0, 1));
			}
		}
	}

}

void Cloth::initSprings()
{
	//assumes nodes have already been initialized!!!
	//vertical springs:
	for (int i = 0; i < num_rows-1; i++)
	{
		for (int j = 0; j < num_cols; j++)
		{
			springs[i * num_cols + j] = new Spring(nodes[i * num_cols + j], nodes[(i + 1) * num_cols + j], unit);
		}
	}
	//horizontal springs:
	int num_vert_springs = (num_rows-1) * num_cols;
	for (int i = 0; i < num_rows; i++)
	{
		for (int j = 0; j < num_cols-1; j++)
		{
			springs[num_vert_springs + i * num_cols + j] = new Spring(nodes[i * num_cols + j], nodes[i * num_cols + (j + 1)], unit);
		}
	}
}

void Cloth::fixNodes()
{
	int num_nodes = num_rows * num_cols;
	for (int i = 0; i < num_nodes; i++)
	{
		nodes[i]->fix();
	}
}

void Cloth::releaseNodes()
{
	int num_nodes = num_rows * num_cols;
	for (int i = 0; i < num_nodes; i++)
	{
		nodes[i]->release();
	}
}

void Cloth::draw(GLuint shaderProgram)
{
	int num_nodes = num_rows * num_cols;
	for (int i = 0; i < num_nodes; i++)
	{
		nodes[i]->draw(shaderProgram);
	}
}
