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

	num_nodes = num_rows * num_cols;
	num_springs = (num_rows-1) * num_cols + (num_cols-1) * num_rows;
	nodes = new Node*[num_nodes];
	springs = new Spring*[num_springs];
}

Cloth::Cloth(int rows, int cols)
{
	num_rows = rows;
	num_cols = cols;
	unit = 0.1f;
	center = Vec3D(0, 1, 0);
	tension = 0.9f; //ratio between goal length and actual length of springs in row/col

	start_vertex_index = 0;
	total_vertices = 0;

	num_nodes = num_rows * num_cols;
	num_springs = (num_rows-1) * num_cols + (num_cols-1) * num_rows;
	nodes = new Node*[num_nodes];
	springs = new Spring*[num_springs];
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

	num_nodes = num_rows * num_cols;
	num_springs = (num_rows-1) * num_cols + (num_cols-1) * num_rows;
	nodes = new Node*[num_nodes];
	springs = new Spring*[num_springs];
}

Cloth::~Cloth()
{
	for (int i = 0; i < num_nodes; i++)
	{
		delete nodes[i];
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
	bool val;
	for (int i = 0; i < num_rows; i++)
	{
		if (i == 0)
		{
			val = true;
		}
		else
		{
			val = false;
		}
		for (int j = 0; j < num_cols; j++)
		{
			Vec3D pos = temp_pos + Vec3D(j * unit, 0, i * unit);
			nodes[i * num_cols + j] = new Node(pos, val);
			nodes[i * num_cols + j]->setVertexInfo(start_vertex_index, total_vertices);
			nodes[i * num_cols + j]->setColor(Vec3D(0.5, 0, 1));
		}
	}

}

void Cloth::initSprings()
{
	//IMPORTANT:
	//ASSUMES INITNODES HAS ALREADY BEEN CALLED

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
			springs[num_vert_springs + i * (num_cols-1) + j] = new Spring(nodes[i * num_cols + j], nodes[i * num_cols + (j + 1)], unit);
		}
	}
}

void Cloth::fixNodes()
{
	for (int i = 0; i < num_nodes; i++)
	{
		nodes[i]->fix();
	}
}

void Cloth::releaseNodes()
{
	for (int i = num_cols; i < num_nodes; i++)
	{
		nodes[i]->release();
	}
}

void Cloth::releaseAllNodes()
{
	for (int i = 0; i < num_nodes; i++)
	{
		nodes[i]->release();
	}
}

void Cloth::update(WorldObject ** wobjs, int num_wobjs, Vec3D g_force, float dt)
{
	//update node velocities
	for (int i = 0; i < num_springs; i++)
	{
		Spring * cur_spring = springs[i];
		Vec3D spr_force = cur_spring->calculateForce();

		Node * n1 = cur_spring->getNode1();
		Node * n2 = cur_spring->getNode2();
		Vec3D temp_vel1 = n1->getVel();
		Vec3D temp_vel2 = n2->getVel();

		if (!(n1->isFixed()))
		{
			temp_vel1 = temp_vel1 - dt * spr_force + dt * g_force;
		}
		if (!(n2->isFixed()))
		{
			temp_vel2 = temp_vel2 + dt * spr_force + dt * g_force;
		}

		n1->setVel(temp_vel1);
		n2->setVel(temp_vel2);
	}
	//update node positions
	bool collided = false;
	Vec3D n; //normal to collision
	for (int i = 0; i < num_nodes; i++)
	{
		Node * cur_node = nodes[i];
		if (!(cur_node->isFixed()))
		{
			Vec3D temp_pos = cur_node->getPos() + dt * cur_node->getVel();
			for (int j = 0; j < num_wobjs; j++)
			{
				if (wobjs[j]->collision(temp_pos))
				{
					collided = true;
					n = temp_pos - wobjs[j]->getPos();
				}
			}
			if (collided)
			{
				cur_node->setVel(0.1 * n);
			}
			else
			{
				cur_node->setPos(temp_pos);
			}
		}
		collided = false;
	}
}

void Cloth::draw(GLuint shaderProgram)
{
	for (int i = 0; i < num_nodes; i++)
	{
		nodes[i]->draw(shaderProgram);
	}
}
