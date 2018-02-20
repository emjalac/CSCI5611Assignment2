#ifdef __APPLE__
#include "include/Sphere.h"
#else
#include "Sphere.h"
#endif

using namespace std;


/*----------------------------*/
// CONSTRUCTORS AND DESTRUCTORS
/*----------------------------*/
Sphere::Sphere() : WorldObject() //call parent default
{
  radius = 1;
}

Sphere::Sphere(Vec3D init_pos) : WorldObject(init_pos) //call parent constructor
{
  radius = 1;
}

Sphere::Sphere(Vec3D init_pos, float r) : WorldObject(init_pos) //call parent constructor
{
  radius = r;
}

Sphere::~Sphere()
{
}

/*----------------------------*/
// SETTERS
/*----------------------------*/
void Sphere::setRadius(float r)
{
	radius = r;
}

/*----------------------------*/
// GETTERS
/*----------------------------*/
float Sphere::getRadius()
{
	return radius;
}

/*----------------------------*/
// OTHERS
/*----------------------------*/


/*----------------------------*/
// VIRTUALS
/*----------------------------*/
int Sphere::getType()
{
	return SPHERE_WOBJ;
}
