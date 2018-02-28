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
	num_triangles = (num_rows-1) * (num_cols-1) * 2;
	nodes = new Node**[num_rows];
	for (int i = 0; i < num_rows; i++)
	{
		nodes[i] = new Node*[num_cols];
	}
	springs = new Spring*[num_springs];
	triangles = new Triangle*[num_triangles];

	bool drag = false;
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
	num_triangles = (num_rows-1) * (num_cols-1) * 2;
	nodes = new Node**[num_rows];
	for (int i = 0; i < num_rows; i++)
	{
		nodes[i] = new Node*[num_cols];
	}
	springs = new Spring*[num_springs];
	triangles = new Triangle*[num_triangles];

	bool drag = false;
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
	num_triangles = (num_rows-1) * (num_cols-1) * 2;
	nodes = new Node**[num_rows];
	for (int i = 0; i < num_rows; i++)
	{
		nodes[i] = new Node*[num_cols];
	}
	springs = new Spring*[num_springs];
	triangles = new Triangle*[num_triangles];

	bool drag = false;
}

Cloth::~Cloth()
{
	for (int i = 0; i < num_rows; i++)
	{
		for (int j = 0; j < num_cols; j++)
		{
			delete nodes[i][j];
		}
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

void Cloth::setDrag(bool b)
{
	drag = b;
}

/*----------------------------*/
// GETTERS
/*----------------------------*/
bool Cloth::getDrag()
{
	return drag;
}

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
			val = true; //used to be false but starting with nodes not fixed blows up the system, oops
		}
		for (int j = 0; j < num_cols; j++)
		{
			Vec3D pos = temp_pos + Vec3D(j * unit, 0, i * unit);
			nodes[i][j] = new Node(pos, val);
			nodes[i][j]->setVertexInfo(start_vertex_index, total_vertices);
			nodes[i][j]->setColor(Vec3D(0.5, 0, 1));
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
			springs[i * num_cols + j] = new Spring(nodes[i][j], nodes[i + 1][j], unit);
		}
	}
	//horizontal springs:
	int num_vert_springs = (num_rows-1) * num_cols;
	for (int i = 0; i < num_rows; i++)
	{
		for (int j = 0; j < num_cols-1; j++)
		{
			springs[num_vert_springs + i * (num_cols-1) + j] = new Spring(nodes[i][j], nodes[i][j + 1], unit);
		}
	}
}

void Cloth::initTriangles()
{
	//IMPORTANT:
	//ASSUMES INITNODES HAS ALREADY BEEN CALLED
	
	for (int i = 0; i < num_rows-1; i++)
	{
		for (int j = 0; j < num_cols-1; j++)
		{
			triangles[i * 2 * (num_cols-1) + 2 * j] = new Triangle(nodes[i][j],nodes[i+1][j],nodes[i][j+1]);
			triangles[i * 2 * (num_cols-1) + 2 * j + 1] = new Triangle(nodes[i+1][j],nodes[i][j+1],nodes[i+1][j+1]);
			triangles[i * 2 * (num_cols-1) + 2 * j]->setColor(Vec3D(0,0,1));
			triangles[i * 2 * (num_cols-1) + 2 * j + 1]->setColor(Vec3D(0,0,1));
			// triangles[i * 2 * (num_cols-1) + 2 * j]->setSpecular(Vec3D(1,1,1));
			// triangles[i * 2 * (num_cols-1) + 2 * j + 1]->setSpecular(Vec3D(1,1,1));
		}
	}
}

void Cloth::updateTriangleNormals()
{
	for (int i = 0; i < num_triangles; i++)
	{
		triangles[i]->updateNormal();
	}
}

void Cloth::fixNodes()
{
	for (int i = 0; i < num_rows; i++)
	{
		for (int j = 0; j < num_cols; j++)
		{
			nodes[i][j]->fix();
		}
	}
}

void Cloth::releaseNodes()
{
	for (int i = 1; i < num_rows; i++)
	{
		for (int j = 0; j < num_cols; j++)
		{
			nodes[i][j]->release();
		}
	}
}

void Cloth::releaseAllNodes()
{
	for (int i = 0; i < num_rows; i++)
	{
		for (int j = 0; j < num_cols; j++)
		{
			nodes[i][j]->release();
		}
	}
}

void Cloth::update(WorldObject ** wobjs, int num_wobjs, Vec3D g_force, float dt)
{
	//update node velocities with spring forces + gravity
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
			temp_vel1 = temp_vel1 - dt * (.5 * spr_force) + dt * g_force;
		}
		if (!(n2->isFixed()))
		{
			temp_vel2 = temp_vel2 + dt * (.5 * spr_force) + dt * g_force;
		}

		n1->setVel(temp_vel1);
		n2->setVel(temp_vel2);
	}

	//update node velocities with drag forces
	if (drag)
	{
		for (int i = 0; i < num_triangles; i++)
		{
			//printf("%i\n", i);
			Triangle * cur_triangle = triangles[i];
			Vec3D drag_force = cur_triangle->calculateDrag();


			Node * n1 = cur_triangle->getV1();
			Node * n2 = cur_triangle->getV2();
			Node * n3 = cur_triangle->getV3();
			Vec3D temp_vel1 = n1->getVel();
			Vec3D temp_vel2 = n2->getVel();
			Vec3D temp_vel3 = n3->getVel();

			if (!(n1->isFixed()))
			{
				temp_vel1 = temp_vel1 + dt * (1/3 * 5000 * drag_force);
			}
			if (!(n2->isFixed()))
			{
				temp_vel2 = temp_vel2 + dt * (1/3 * 5000 * drag_force);
			}
			if (!(n3->isFixed()))
			{
				temp_vel3 = temp_vel3 + dt * (1/3 * 5000 * drag_force);
			}

			n1->setVel(temp_vel1);
			n2->setVel(temp_vel2);
			n3->setVel(temp_vel3);
		}
	}
	
	//update node positions
	bool collided = false;
	Vec3D collision_pos;
	Vec3D n; //normal to collision
	for (int i = 0; i < num_rows; i++)
	{
		for (int j = 0; j < num_cols; j++)
		{
			Node * cur_node = nodes[i][j];
			if (!(cur_node->isFixed()))
			{
				Vec3D temp_pos = cur_node->getPos() + dt * cur_node->getVel();
				for (int k = 0; k < num_wobjs; k++)
				{
					if (wobjs[k]->collision(temp_pos))
					{
						collided = true;
						collision_pos = wobjs[k]->getCollisionPos(temp_pos);
						n = temp_pos - wobjs[k]->getPos();
					}
				}
				if (collided)
				{
					cur_node->setPos(collision_pos);
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

	//update triangle normals
	updateTriangleNormals();
}

void Cloth::draw(GLuint shaderProgram, GLuint model_vbo, GLuint line_vbo)
{
	// //Set vbo for nodes
	// glBindBuffer(GL_ARRAY_BUFFER, model_vbo);
	// //Define position attribute
	// GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	// glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
	// //Draw
	// for (int i = 0; i < num_rows; i++)
	// {
	// 	for (int j = 0; j < num_cols; j++)
	// 	{
	// 		nodes[i][j]->draw(shaderProgram);
	// 	}
	// }
	
	// //Set vbo for springs
	// glBindBuffer(GL_ARRAY_BUFFER, line_vbo);
	// //Define position attribute
	// glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	// //Draw
	// for (int i = 0; i < num_springs; i++)
	// {
	// 	springs[i]->draw(shaderProgram);
	// }

	//Use same vbo for triangles
	//Uncomment the following if only drawing triangles:
	glBindBuffer(GL_ARRAY_BUFFER, line_vbo);
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
	//next 2 floats are texture coords (u,v)
	GLint texAttrib = glGetAttribLocation(shaderProgram, "inTexcoord");
	glEnableVertexAttribArray(texAttrib);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	//last 3 floats are normal coords
	GLint normAttrib = glGetAttribLocation(shaderProgram, "inNormal");
	glVertexAttribPointer(normAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(normAttrib);
	//Draw
	for (int i = 0; i < num_triangles; i++)
	{
		triangles[i]->draw(shaderProgram, i%2);
	}
}
