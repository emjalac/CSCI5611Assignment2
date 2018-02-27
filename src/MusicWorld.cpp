#ifdef __APPLE__
#include "include/MusicWorld.h"
#else
#include "MusicWorld.h"
#endif

using namespace std;


/*----------------------------*/
// CONSTRUCTORS AND DESTRUCTORS
/*----------------------------*/
MusicWorld::MusicWorld()
{
	total_verts = 0;
	nodes_per_string = 20; //must be even
	string1 = new String(nodes_per_string, Vec3D(0,0,0));
	string2 = new String(nodes_per_string, Vec3D(.25,0,0));
	string3 = new String(nodes_per_string, Vec3D(.5,0,0));
	string4 = new String(nodes_per_string, Vec3D(.75,0,0));
	node_index = nodes_per_string/2;
	audio = true;
}

MusicWorld::MusicWorld(int num)
{
	total_verts = 0;
	nodes_per_string = num; //must be even
	string1 = new String(nodes_per_string, Vec3D(0,0,0));
	string2 = new String(nodes_per_string, Vec3D(.25,0,0));
	string3 = new String(nodes_per_string, Vec3D(.5,0,0));
	string4 = new String(nodes_per_string, Vec3D(.75,0,0));
	node_index = nodes_per_string/2;
	audio = true;
}

MusicWorld::~MusicWorld()
{
	delete[] modelData;
	string1->~String();
	string2->~String();
	string3->~String();
	string4->~String();
}

/*----------------------------*/
// SETTERS
/*----------------------------*/
void MusicWorld::setNodeIndex(int i)
{
	node_index = i;
}

/*----------------------------*/
// GETTERS
/*----------------------------*/
float MusicWorld::getStringsLength()
{
	//assumed all four strings have same length
	int num_nodes = string1->getNumNodes();
	float unit = string1->getUnit();
	return (num_nodes * unit);
}

int MusicWorld::getNodeIndex()
{
	return node_index;
}

int MusicWorld::getNodesPerString()
{
	return nodes_per_string;
}

/*----------------------------*/
// OTHERS
/*----------------------------*/
//load in all models and store data into the modelData array
bool MusicWorld::loadModelData()
{
	/////////////////////////////////
	//LOAD IN MODELS
	/////////////////////////////////
	//CUBE
	CUBE_START = 0;
	CUBE_VERTS = 0;
	#ifdef __APPLE__
	float* cubeData = util::loadModel("../models/cube.txt", CUBE_VERTS);
	#else
	float* cubeData = util::loadModel("models/cube.txt", CUBE_VERTS);
	#endif
	cout << "\nNumber of vertices in cube model : " << CUBE_VERTS << endl;
	total_verts += CUBE_VERTS;

	//SPHERE
	SPHERE_START = CUBE_VERTS;
	SPHERE_VERTS = 0;
	#ifdef __APPLE__
	float* sphereData = util::loadModel("../models/sphere.txt", SPHERE_VERTS);
	#else
	float* sphereData = util::loadModel("models/sphere.txt", SPHERE_VERTS);
	#endif
	cout << "\nNumber of vertices in sphere model : " << SPHERE_VERTS << endl;
	total_verts += SPHERE_VERTS;

	/////////////////////////////////
	//BUILD MODELDATA ARRAY
	/////////////////////////////////
	if (!(cubeData != nullptr && sphereData != nullptr))
	{
		cout << "ERROR. Failed to load model(s)" << endl;
		delete[] cubeData;
		delete[] sphereData;
		return false;
	}
	//each vertex has pos (3f) + norm (3) + texture coords (u,v) = 8 floats
	modelData = new float[total_verts * 8];

	//copy data into modelData array
	copy(cubeData, cubeData + CUBE_VERTS * 8, modelData);
	copy(sphereData, sphereData + SPHERE_VERTS * 8, modelData + (CUBE_VERTS * 8));
	delete[] cubeData;
	delete[] sphereData;
	return true;
}

//
bool MusicWorld::setupGraphics()
{
	/////////////////////////////////
	//SETUP SHADERS
	/////////////////////////////////
	#ifdef __APPLE__
	shaderProgram = util::LoadShader("../Shaders/phongTex.vert", "../Shaders/phongTex.frag");
	#else
	shaderProgram = util::LoadShader("Shaders/phongTex.vert", "Shaders/phongTex.frag");
	#endif

	//load in textures
	#ifdef __APPLE__
	tex0 = util::LoadTexture("../textures/wood.bmp");
	tex1 = util::LoadTexture("../textures/grey_stones.bmp");
	tex2 = util::LoadTexture("../textures/uniformclouds.bmp");
	#else
	tex0 = util::LoadTexture("textures/wood.bmp");
	tex1 = util::LoadTexture("textures/grey_stones.bmp");
	tex2 = util::LoadTexture("textures/uniformclouds.bmp");
	#endif

	if (tex0 == -1 || tex1 == -1 || tex2 == -1 || shaderProgram == -1)
	{
		cout << "\nERROR. Failed to load texture(s)" << endl;
		printf(strerror(errno));
		return false;
	}

	/////////////////////////////////
	//BUILD VERTEX ARRAY OBJECT
	/////////////////////////////////
	//This stores the VBO and attribute mappings in one object
	glGenVertexArrays(2, &vao); //Create a VAO
	glBindVertexArray(vao); //Bind the above created VAO to the current context
	cout << "VAO bound to current context" << endl;

	/////////////////////////////////
	//BUILD VERTEX BUFFER OBJECTS
	/////////////////////////////////
	//Allocate memory on the graphics card to store geometry (vertex buffer object)
	glGenBuffers(1, model_vbo);  //Create 1 buffer called model_vbo
	glBindBuffer(GL_ARRAY_BUFFER, model_vbo[0]); //Set the buffer as the active array buffer (Only one buffer can be active at a time)
	glBufferData(GL_ARRAY_BUFFER, total_verts * 8 * sizeof(float), modelData, GL_STATIC_DRAW); //upload vertices to vbo
	cout << "First VBO set up with model data" << endl;

	//Tell OpenGL how to set fragment shader input
	//first 3 floats are position coords
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	//the specifics of position attribute will be specified prior to drawing depending on which vbo is being used
	glEnableVertexAttribArray(posAttrib);

	//next 2 floats are texture coords (u,v)
	GLint texAttrib = glGetAttribLocation(shaderProgram, "inTexcoord");
	glEnableVertexAttribArray(texAttrib);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

	//last 3 floats are normal coords
	GLint normAttrib = glGetAttribLocation(shaderProgram, "inNormal");
	glVertexAttribPointer(normAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(normAttrib);

	//Allocate memory for second vbo
	glGenBuffers(1, line_vbo);  //Create 1 buffer called line_vbo
	glBindBuffer(GL_ARRAY_BUFFER, line_vbo[0]); //Set the buffer as the active array buffer (Only one buffer can be active at a time)
	//vertex data for line_vbo will be set later and constantly updated to draw moving lines
	cout << "Second VBO set up" << endl;

	glBindVertexArray(0); //Unbind the VAO in case we want to create a new one

	glEnable(GL_DEPTH_TEST);
	return true;
}

//loops through WObj array and draws each
//also draws floor
void MusicWorld::draw(Camera * cam)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shaderProgram); //Set the active shader (only one can be used at a time)

	//vertex shader uniforms
	GLint uniView = glGetUniformLocation(shaderProgram, "view");
	GLint uniProj = glGetUniformLocation(shaderProgram, "proj");
	GLint uniTexID = glGetUniformLocation(shaderProgram, "texID");

	//build view matrix from Camera
	glm::mat4 view = glm::lookAt(
		util::vec3DtoGLM(cam->getPos()),
		util::vec3DtoGLM(cam->getPos() + cam->getDir()),  //Look at point
		util::vec3DtoGLM(cam->getUp()));

	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 proj = glm::perspective(3.14f / 4, 800.0f / 600.0f, 0.1f, 100.0f); //FOV, aspect, near, far
	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex0);
	glUniform1i(glGetUniformLocation(shaderProgram, "tex0"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex1);
	glUniform1i(glGetUniformLocation(shaderProgram, "tex1"), 1);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex2);
	glUniform1i(glGetUniformLocation(shaderProgram, "tex2"), 2);

	glBindVertexArray(vao);

	glUniform1i(uniTexID, -1); //Set texture ID to use (0 = wood texture, -1 = no texture)

	string1->draw(shaderProgram, model_vbo[0], line_vbo[0]);
	string2->draw(shaderProgram, model_vbo[0], line_vbo[0]);
	string3->draw(shaderProgram, model_vbo[0], line_vbo[0]);
	string4->draw(shaderProgram, model_vbo[0], line_vbo[0]);
}

void MusicWorld::initStrings()
{
	string1->setVertexInfo(SPHERE_START, SPHERE_VERTS);
	string1->initNodes();
	string1->initSprings();
	string2->setVertexInfo(SPHERE_START, SPHERE_VERTS);
	string2->initNodes();
	string2->initSprings();
	string3->setVertexInfo(SPHERE_START, SPHERE_VERTS);
	string3->initNodes();
	string3->initSprings();
	string4->setVertexInfo(SPHERE_START, SPHERE_VERTS);
	string4->initNodes();
	string4->initSprings();
}

void MusicWorld::pluckString(int s) //s indicates which string to pluck
{
	String * cur_string;
	Node * cur_node;
	switch (s)
	{
	case 1:
		cur_string = string1;
		break;
	case 2:
		cur_string = string2;
		break;
	case 3:
		cur_string = string3;
		break;
	case 4:
		cur_string = string4;
		break;
	default:
		cur_string = string1;
		break;
	}//END polling switch
	float strength = 0.1f;
	double d = strength * cur_string->getRestLen(); //scale pluck with string size
	for (int i = 0; i < 5; i++)
	{
		int which_node = node_index + i - 2;
		if (which_node > 0 && which_node < nodes_per_string-1) //a nonfixed node
		{
			cur_node = cur_string->getNode(which_node);
			if (i == 2) cur_node->setPos(cur_node->getPos() + Vec3D(0,d,0));
			else cur_node->setPos(cur_node->getPos() + Vec3D(0,d/2,0));
		}
	}
}	

void MusicWorld::strikeString(int s) //s indicates which string to strike
{
	String * cur_string;
	Node * cur_node;
	switch (s)
	{
	case 1:
		cur_string = string1;
		break;
	case 2:
		cur_string = string2;
		break;
	case 3:
		cur_string = string3;
		break;
	case 4:
		cur_string = string4;
		break;
	default:
		cur_string = string1;
		break;
	}//END polling switch
	float strength = 0.1f;
	double d = strength * 20; //scale pluck with string size
	for (int i = 0; i < 5; i++)
	{
		int which_node = node_index + i - 2;
		if (which_node > 0 && which_node < nodes_per_string-1) //a nonfixed node
		{
			cur_node = cur_string->getNode(which_node);
			if (i == 2) cur_node->setVel(cur_node->getVel() + Vec3D(0,d,0));
			else cur_node->setVel(cur_node->getVel() + Vec3D(0,d/2,0));
		}
	}
}

void MusicWorld::updateObjs(float dt)
{
	dt = .0001; //override given dt

	//update string positions
	string1->update(dt);
	string2->update(dt);
	string3->update(dt);
	string4->update(dt);
}

float MusicWorld::updateAudio()
{
	float val = 0;

	if (audio)
	{
		float mid_yvel = string1->getNode(nodes_per_string/2)->getVel().getY();
		float end1_yvel = string1->getNode(nodes_per_string/2 + 7)->getVel().getY();
		float end2_yvel = string1->getNode(nodes_per_string/2 - 7)->getVel().getY();
		val = .5*mid_yvel + .25*end1_yvel + .25*end2_yvel;
	}

	return val;
}

/*----------------------------*/
// PRIVATE FUNCTIONS
/*----------------------------*/
