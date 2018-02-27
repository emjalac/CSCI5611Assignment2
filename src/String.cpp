#ifdef __APPLE__
#include "include/String.h"
#else
#include "String.h"
#endif

#include <iostream>

/*----------------------------*/
// CONSTRUCTORS AND DESTRUCTORS
/*----------------------------*/
String::String()
{
	num_nodes = 20; //must be even
	unit = 0.05f;
	center = Vec3D(0, 1, 0);
	rest_len = 0;

	start_vertex_index = 0;
	total_vertices = 0;

	num_springs = num_nodes - 1;
	nodes = new Node*[num_nodes];
	springs = new Spring*[num_springs];
}

String::String(int num)
{
	num_nodes = num; //must be even
	unit = 0.05f;
	center = Vec3D(0, 1, 0);
	rest_len = 0;

	start_vertex_index = 0;
	total_vertices = 0;

	num_springs = num_nodes - 1;
	nodes = new Node*[num_nodes];
	springs = new Spring*[num_springs];
}

String::String(int num, Vec3D pos)
{
	num_nodes = num; //must be even
	unit = 0.05f;
	center = pos;
	rest_len = 0;

	start_vertex_index = 0;
	total_vertices = 0;

	num_springs = num_nodes - 1;
	nodes = new Node*[num_nodes];
	springs = new Spring*[num_springs];
}

String::~String()
{
	for (int i = 0; i < num_nodes; i++)
	{
		delete nodes[i];
	}
}

/*----------------------------*/
// SETTERS
/*----------------------------*/
void String::setVertexInfo(int start, int total)
{
	start_vertex_index = start;
	total_vertices = total;
}

/*----------------------------*/
// GETTERS
/*----------------------------*/
int String::getNumNodes()
{
	return num_nodes;
}

float String::getUnit()
{
	return unit;
}

Node * String::getNode(int i)
{
	if (i < num_nodes && i >= 0) return nodes[i];
	return nodes[num_nodes/2];
}

float String::getRestLen()
{
	return rest_len;
}

/*----------------------------*/
// OTHERS
/*----------------------------*/
void String::initNodes()
{
	Vec3D temp_pos = Vec3D(center.getX(), center.getY(), center.getZ() - (num_nodes-1)/2 * unit);
	bool val;
	for (int i = 0; i < num_nodes; i++)
	{
		if (i == 0 || i == num_nodes - 1)
		{
			val = true; //nodes on ends are fixed
		}
		else
		{
			val = false; //nodes in middle are not
		}
		Vec3D pos = temp_pos + Vec3D(0, 0, i * unit);
		nodes[i] = new Node(pos, val);
		nodes[i]->setVertexInfo(start_vertex_index, total_vertices);
		nodes[i]->setColor(Vec3D(0, 0, 0));
	}
}

void String::initSprings()
{
	//IMPORTANT:
	//ASSUMES INITNODES HAS ALREADY BEEN CALLED

	//vertical springs:
	for (int i = 0; i < num_springs; i++)
	{
		springs[i] = new Spring(nodes[i], nodes[i + 1], unit);
		rest_len += springs[i]->getRestLen();
	}
}

void String::update(float dt)
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
			temp_vel1 = temp_vel1 - dt * (.5 * spr_force);
		}
		if (!(n2->isFixed()))
		{
			temp_vel2 = temp_vel2 + dt * (.5 * spr_force);
		}

		n1->setVel(temp_vel1);
		n2->setVel(temp_vel2);
	}
	//update node positions
	for (int i = 0; i < num_nodes; i++)
	{
		Node * cur_node = nodes[i];
		if (!(cur_node->isFixed()))
		{
			Vec3D temp_pos = cur_node->getPos() + dt * cur_node->getVel();
			cur_node->setPos(temp_pos);
		}
	}
}

void String::draw(GLuint shaderProgram, GLuint model_vbo, GLuint line_vbo)
{
	//Set vbo for nodes
	glBindBuffer(GL_ARRAY_BUFFER, model_vbo);
	//Define position attribute
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
	//Draw
	for (int i = 0; i < num_nodes; i++)
	{
		nodes[i]->draw(shaderProgram);
	}
	
	//Set vbo for springs
	glBindBuffer(GL_ARRAY_BUFFER, line_vbo);
	//Define position attribute
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	//Draw
	for (int i = 0; i < num_springs; i++)
	{
		springs[i]->draw(shaderProgram);
	}
}
