#ifndef OPENGL_GAME
#define OPENGL_GAME

//windows specific stuff
#ifdef WIN32
#include <windows.h>
#define GLEW_STATIC
#define GLUT_DISABLE_ATEXIT_HACK
#define GLAPI extern
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/GL.h>
#endif

#ifndef WIN32
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#endif

//other classes
#include "camera.h"
#include "dobj.h"

//everything else
#include <iostream>
#include <fstream>
#include <vector>

class Game
{
public:
	Game();
	~Game();

	bool init(int argv, char** argc);

	void keyboardFunc(char key, int x, int y);
	void keyboardLiftFunc(char key, int x, int y);
	void mouseMoved(int x, int y);
	void mouseEntered(int state);

	void reshape(int width, int height);

	//functions used for game's main loop
	void run();
	//decided what to do with user's inputs
	void keyHandler();
	//draws next frame
	void draw();


	//for trial and debugging
	void trialSettings();



private:


	//the games needs a camera. This also deals with perspective
	Camera _myCamera;

	//array for keys. True represents key is being pressed
	bool _key[256];



	//state variables
	//last mouse position
	int _xm, _ym;
	bool _jumped;


	//used for perspective
	int _xres, _yres;
	float _fov;
	float _near, _far;


	//boolean for game to run. Starts as false until game is initialized
	bool _gameIsRunning;

	//these will dictate framerate and framerate limiting
	bool _limitFramerate;
	int _targetFPS;
	float _lastTimeDrawn;

	//vector for holding objects in the game
	std::vector<DObject *> _myObjects;
};

#endif