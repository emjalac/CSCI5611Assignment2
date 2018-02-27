#ifndef MUSICWORLD_INCLUDED
#define MUSICWORLD_INCLUDED

#include "glad.h"  //Include order can matter here

#ifdef __APPLE__
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#elif __linux__
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#else
#include <SDL.h>
#include <SDL_opengl.h>
#endif

#define GLM_FORCE_RADIANS
#ifdef __APPLE__
#include "../../ext/glm/glm.hpp"
#include "../../ext/glm/gtc/matrix_transform.hpp"
#include "../../ext/glm/gtc/type_ptr.hpp"
#else
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#endif

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>

#include "Vec3D.h"
#include "Camera.h"
#include "Util.h"
#include "String.h"

class MusicWorld{
private:
	int total_verts;
	float * modelData; 

	//modelData indices
	int CUBE_START = 0;
	int CUBE_VERTS = 0;
	int SPHERE_START = 0;
	int SPHERE_VERTS = 0;

	//VAO and VBO GLuints
	GLuint vao;
	GLuint model_vbo[1];
	GLuint line_vbo[1];

	//Shader and Texture GLuints
	GLuint shaderProgram;
	GLuint tex0;
	GLuint tex1;
	GLuint tex2;

	//what lives in this world
	int nodes_per_string;
	String * string1;
	String * string2;
	String * string3;
	String * string4;
	int node_index;
	bool audio;

public:
	//CONSTRUCTORS AND DESTRUCTORS
	MusicWorld();
	MusicWorld(int num);
	~MusicWorld();

	//SETTERS
	void setNodeIndex(int i);

	//GETTERS
	float getStringsLength();
	int getNodeIndex();
	int getNodesPerString();

	//OTHERS
	bool loadModelData();
	bool setupGraphics();
	void initStrings();
	void pluckString(int s); //s indicates which string to pluck
	void strikeString(int s); //s indicates which string to strike
	void updateObjs(float dt);
	float updateAudio();
	void draw(Camera * cam);

};

#endif
