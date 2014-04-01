#ifndef MAIN_METHOD
#define MAIN_METHOD

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

//my other classes and methods
#include "game.h"

//console and other stuff
#include <iostream>

//main and initializing functions
int main(int argv, char** argc);
bool init(int argv, char** argc);

//functions for glut callbacks
void initCallbacks();
void reshape(int width, int height);
void keyboardFuncSpec(int key, int x, int y);
void keyboardFunc(unsigned char key, int x, int y);
void keyboardLiftFunc(unsigned char key, int x, int y);
void mouseMovement(int x, int y);
void mouseEntered(int state);
void draw();


//have to do this. I know this is not proper coding
Game * _myGame;

#endif