#ifndef WORLDOBJ_INCLUDED
#define WORLDOBJ_INCLUDED

#include "Vec3D.h"
#include "Util.h"
#include "Camera.h"
#include "Material.h"

enum WOBJ_type
{
	SPHERE_WOBJ,
	QUAD_WOBJ,
	DEFAULT_WOBJ
};

class WorldObject
{
protected:
	Vec3D pos;
  	Vec3D vel;

	Material mat;
	Vec3D size;
	int start_vertex_index;	//index where vertices start in modelData array
	int total_vertices;	//total num of vertices within modelData array

public:
	//CONSTRUCTORS AND DESTRUCTORS
	WorldObject();
	WorldObject(Vec3D init_pos);
	virtual ~WorldObject();

	//SETTERS
	void setPos(Vec3D p);
	void setVel(Vec3D v);
	void setVertexInfo(int start, int total);
	void setMaterial(Material m);
	void setSize(Vec3D s);
	void setColor(Vec3D color); //sets ambient and diffuse to 'color'

	//GETTERS
	Vec3D getPos();
	Vec3D getVel();
	Material getMaterial();
	Vec3D getSize();

	//VIRTUAL
	virtual int getType();
	virtual bool collision(Vec3D p) = 0;
	virtual Vec3D getCollisionPos(Vec3D p) = 0;

	//OTHER
	void draw(GLuint shaderProgram); //shared draw function among WObjs

};

#endif
