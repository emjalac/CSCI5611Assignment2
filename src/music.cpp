//////////////////////////////////
//CSCI 5611 HW2 - Physically-Based Simulation
//Emma Lacroix - lacro058
//Sound simulation
//////////////////////////////////

#ifdef __APPLE__
#include "include/glad.h"
#else
#include "glad.h"
#endif  //Include order can matter here

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
#include "../ext/glm/glm.hpp"
#include "../ext/glm/gtc/matrix_transform.hpp"
#include "../ext/glm/gtc/type_ptr.hpp"
#else
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#endif

#include <cerrno>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>

//MY CLASSES
#ifdef __APPLE__
#include "include/Util.h"
#include "include/MusicWorld.h"
#else
#include "Util.h"
#include "MusicWorld.h"
#endif

using namespace std;

/*=============================*/
// Global Default Parameters
/*=============================*/
bool fullscreen = false;
int screen_width = 800;
int screen_height = 600;

//shader globals
#ifdef __APPLE__
string vertFile = "../Shaders/phong.vert";
string fragFile = "../Shaders/phong.frag";
#else
string vertFile = "Shaders/phong.vert";
string fragFile = "Shaders/phong.frag";
#endif

//audio globals
int SamplesPerSecond = 48000;
short ToneVolume = 2000;
unsigned RunningSampleIndex = 0;
const int buffSize = 1 * 104 * 1024;
float soundBuff[buffSize];
int lastS = 0;
static int lastP = 0;

//other globals
const float mouse_speed = 0.05f;
const float step_size = 0.15f;
const int update_count = 2000;
int string_index = 1; //default: pluck/strike string1

/*=============================*/
// Helper Functions
/*=============================*/
void onKeyDown(SDL_KeyboardEvent & event, Camera* cam, MusicWorld* myWorld, float dt);
void audioCallback(void*, Uint8*, int);
void mouseMove(SDL_MouseMotionEvent & event, Camera * cam, float horizontal_angle, float vertical_angle);

/*==============================================================*/
//							  MAIN
/*==============================================================*/
int main(int argc, char *argv[]) {
	/////////////////////////////////
	//INITIALIZE SDL WINDOW
	/////////////////////////////////
	SDL_GLContext context;
	SDL_Window* window = util::initSDL(context, screen_width, screen_height);

	if (window == NULL)
	{
		cout << "ERROR: initSDL() failed." << endl;
		SDL_GL_DeleteContext(context);
		SDL_Quit();
		exit(0);
	}

	MusicWorld * myWorld = new MusicWorld(20); //must be even (number of nodes per string)

	/////////////////////////////////
	//LOAD MODEL DATA INTO WORLD
	/////////////////////////////////
	if (!myWorld->loadModelData())
	{
		cout << "ERROR. Unable to load model data." << endl;
		//Clean up
		myWorld->~MusicWorld();
		SDL_GL_DeleteContext(context);
		SDL_Quit();
		exit(0);
	}

	/////////////////////////////////
	//INITIALIZE STRINGS
	/////////////////////////////////
	myWorld->initStrings();

	/////////////////////////////////
	//INITIALIZE AUDIO
	/////////////////////////////////
	SDL_Init(SDL_INIT_AUDIO);
	SDL_AudioSpec desiredSpec;

	desiredSpec.freq = SamplesPerSecond;
	desiredSpec.format = AUDIO_S16SYS;
	desiredSpec.channels = 1;
	desiredSpec.samples = 2048;
	desiredSpec.callback = audioCallback;

	SDL_AudioSpec obtainedSpec;

	//Check the following for errors
	//device, iscapture, desired, obtained, allowedchanges?
	//https://wiki.libsdl.org/SDL_OpenAudioDevice
	SDL_OpenAudioDevice(0, 0, &desiredSpec, &obtainedSpec, 0); //CHECK THIS

	//unpause audio
	SDL_PauseAudio(0);

	for (int i = 0; i < buffSize; i++){
	   soundBuff[i] = 0;
	}

	/////////////////////////////////
	//SETUP CAMERA
	/////////////////////////////////
	Camera* cam = new Camera();
	cam->setDir(Vec3D(0, 0, 1));					//look along +z
	cam->setPos(Vec3D(0, 0, 0));					//start
	cam->setUp(Vec3D(0, 1, 0));						//map is in xz plane
	cam->setRight(Vec3D(1, 0, 0));					//look along +z

	/////////////////////////////////
	//SETUP MOUSE INITIAL STATE
	/////////////////////////////////
	float horizontal_angle = 0.0f;
	float vertical_angle = 0.0f;

	/////////////////////////////////
	//VAO + VBO + SHADERS + TEXTURES
	/////////////////////////////////
	if (!myWorld->setupGraphics())
	{
		cout << "ERROR. Unable to setup Graphics." << endl;
		//Clean Up
		SDL_GL_DeleteContext(context);
		SDL_Quit();
		myWorld->~MusicWorld();
		cam->~Camera();
	}

	/*===========================================================================================
	* EVENT LOOP (Loop forever processing each event as fast as possible)
	* List of keycodes: https://wiki.libsdl.org/SDL_Keycode - You can catch many special keys
	* Scancode refers to a keyboard position, keycode refers to the letter (e.g., EU keyboards)
	===========================================================================================*/
	SDL_Event windowEvent;
	bool quit = false;
	bool mouse_active = false;
	bool recentering = true;
	float last_time = SDL_GetTicks(),	delta_time = 0,	new_time = 0;
	float sound_val;

	//FPS calculations
	float framecount = 0;
	float fps = 0, last_fps_print = 0.0;

	while (!quit)
	{
		while (SDL_PollEvent(&windowEvent)) {
			switch (windowEvent.type) //event type -- key up or down
			{
			case SDL_QUIT:
				quit = true; //Exit event loop
				break;
			case SDL_KEYDOWN:
				//check for escape or fullscreen before checking other commands
				if (windowEvent.key.keysym.sym == SDLK_ESCAPE) quit = true; //Exit event loop
				else if (windowEvent.key.keysym.sym == SDLK_f) fullscreen = !fullscreen;
				onKeyDown(windowEvent.key, cam, myWorld, delta_time);
				break;
			case SDL_MOUSEMOTION:
				if (recentering)
				{
					SDL_WarpMouseInWindow(window, screen_width / 2, screen_height / 2);
					mouse_active = true;
				}
				else if (mouse_active && !recentering)
				{
					mouseMove(windowEvent.motion, cam, horizontal_angle, vertical_angle);
					// recentering = true;
				}
			default:
				break;
			}//END polling switch
			SDL_SetWindowFullscreen(window, fullscreen ? SDL_WINDOW_FULLSCREEN : 0); //Set to full screen
		}//END polling while

		if (mouse_active)
		{
			recentering = false;
		}

		//draw entire world
		myWorld->draw(cam);

		//delta_time is in seconds so convert ticks (ms) by * 1000
		new_time = SDL_GetTicks();
		delta_time = (new_time - last_time) / 1000.0;
		last_time = new_time;
		//printf("dt is %f\n", delta_time);

		if ((new_time - last_fps_print) / 1000.0 >= 1.0) //only print every 1+ seconds
		{
			fps = framecount;
			last_fps_print = new_time;
			printf("FPS: %f\n", fps);
			framecount = 0;
		}

		for (int i = 0; i < update_count; i++)
		{
			if (lastS - lastP <= 2 * 2048 && lastS + update_count < buffSize)
			{
				sound_val = myWorld->updateAudio();
				SDL_LockAudio();
				soundBuff[lastS++] = sound_val;
				SDL_UnlockAudio();
			}
			myWorld->updateObjs(delta_time);
		}

		SDL_GL_SwapWindow(window);
		framecount++;
	}//END looping While

	//Clean Up
	SDL_GL_DeleteContext(context);
	SDL_Quit();
	myWorld->~MusicWorld();
	cam->~Camera();

	return 0;
}//END MAIN

/*--------------------------------------------------------------*/
// onKeyDown : determine which key was pressed and how to edit
//				current translation or rotation parameters
/*--------------------------------------------------------------*/
void onKeyDown(SDL_KeyboardEvent & event, Camera* cam, MusicWorld* myWorld, float dt)
{
	Vec3D pos = cam->getPos();
	Vec3D dir = cam->getDir();
	Vec3D right = cam->getRight();
	Vec3D up = cam->getUp();

	//temps to be modified in switch
	Vec3D temp_pos = pos;
	Vec3D temp_dir = dir;
	Vec3D temp_right = right;

	switch (event.keysym.sym)
	{
	/////////////////////////////////
	//TRANSLATION WITH WASD
	/////////////////////////////////
	case SDLK_w:
		temp_pos = pos + (step_size*dir);
		break;
	case SDLK_s:
		temp_pos = pos - (step_size*dir);
		break;
	case SDLK_d:
		temp_pos = pos + (step_size*right);
		break;
	case SDLK_a:
		temp_pos = pos - (step_size*right);
		break;
	/////////////////////////////////
	//PLUCK/STRIKE STRINGS
	/////////////////////////////////
	case SDLK_j:
		myWorld->pluckString(string_index);
		break;
	case SDLK_k:
		myWorld->strikeString(string_index);
		break;
	/////////////////////////////////
	//CHANGE STRING INDEX WITH #S
	/////////////////////////////////
	case SDLK_1:
	case SDLK_KP_1:
		string_index = 1;
		break;
	case SDLK_2:
	case SDLK_KP_2:
		string_index = 2;
		break;
	case SDLK_3:
	case SDLK_KP_3:
		string_index = 3;
		break;
	case SDLK_4:
	case SDLK_KP_4:
		string_index = 4;
		break;
	/////////////////////////////////
	//CHANGE NODE INDEX WITH +/-
	/////////////////////////////////
	case SDLK_EQUALS:
		if (myWorld->getNodeIndex() < myWorld->getNodesPerString()-1)
		{
			myWorld->setNodeIndex(myWorld->getNodeIndex()+1);
		}
		break;
	case SDLK_MINUS:
		if (myWorld->getNodeIndex() > 0)
		{
			myWorld->setNodeIndex(myWorld->getNodeIndex()-1);
		}
		break;
	default:
		break;
	}//END switch key press

	cam->setDir(temp_dir);
	cam->setRight(temp_right);
	cam->setPos(temp_pos);
	return;
}//END onKeyDown

/*--------------------------------------------------------------*/
// audioCallback : 
/*--------------------------------------------------------------*/

void audioCallback(void * _beeper, Uint8 * _stream, int _len)
{
	short * stream = (short *) _stream;
	int len = _len / 2;

	int i;
	for (i = 0; i < len && (lastP < lastS); i++)
	{
		if (lastS > buffSize) exit(0);
		lastP++;
		double amp = ToneVolume * 2 * soundBuff[lastP]; //set amplitude
		amp = amp > 3*ToneVolume ? 3*ToneVolume : amp < -3*ToneVolume ? -3*ToneVolume : amp; //clamp amplitude
		stream[i] = amp;
	}
	for (; i < len; i++) //if we missed a frame...
	{
		stream[i] = stream[i-1]; //repeat last tone
	}
	printf("Buffer size is %d\n", lastS-lastP);
	if (lastS + len > buffSize)
	{
		if (lastS > lastP) //looping
		{
			memcpy(&soundBuff[0], &soundBuff[lastP], sizeof(float)*(lastS-lastP));
			lastS = lastS-lastP;
		}
		else
		{
			lastS = 0;
		}
		lastP = 0;
		memset(&soundBuff[lastS], 0, sizeof(float)*(buffSize-lastS));
	}
}

/*--------------------------------------------------------------*/
// mouseMove : change the view accordingly when the mouse moves!
/*--------------------------------------------------------------*/

void mouseMove(SDL_MouseMotionEvent & event, Camera * cam, float horizontal_angle, float vertical_angle)
{
	Vec3D dir = cam->getDir();
	Vec3D right = cam->getRight();
	Vec3D up = cam->getUp();

	//temps to be modified
	Vec3D temp_dir = dir;
	Vec3D temp_right = right;
	Vec3D temp_up = up;

	horizontal_angle += mouse_speed * step_size * float(screen_width / 2 - event.x);
	vertical_angle += mouse_speed * step_size * float(screen_height / 2 - event.y);

	temp_dir = dir + (Vec3D(cos(vertical_angle) * sin(horizontal_angle), sin(vertical_angle), cos(vertical_angle) * cos(horizontal_angle)));
	temp_right = right + (Vec3D(sin(horizontal_angle - 3.14f / 2.0f), 0, cos(horizontal_angle - 3.14f / 2.0f)));
	temp_up = cross(temp_dir, -1 * temp_right);

	cam->setDir(temp_dir);
	cam->setRight(temp_right);
	cam->setUp(temp_up);
}
