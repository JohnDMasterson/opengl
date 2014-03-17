#include "main.h"

using namespace std;

/*
*	Main method. Creates an instance of a game, sets the callbacks to the game's
*	and runs the game's main loop. 
*	
*	returns 1 if there are errors while initializing glew
*	returns 0 if the program terminates sucessfully
*/
int main(int argv, char ** argc)
{
	init(argv, argc);
	initCallbacks();

	_myGame->trialSettings();
	_myGame->trialSettings();
	_myGame->trialSettings();
	_myGame->trialSettings();
	_myGame->trialSettings();
	_myGame->trialSettings();
	_myGame->trialSettings();
	_myGame->trialSettings();
	_myGame->trialSettings();
	_myGame->trialSettings();
	_myGame->trialSettings();
	_myGame->trialSettings();
	_myGame->trialSettings();
	_myGame->trialSettings();

	glutMainLoop();

	return 0;
}

bool init(int argv, char ** argc)
{
	_myGame = new Game();
	bool initOk = _myGame->init(argv, argc);


	return initOk;
}

void initCallbacks()
{
	glutDisplayFunc(draw);
	glutIdleFunc(draw);
	glutKeyboardFunc(keyboardFunc);
	glutKeyboardUpFunc(keyboardLiftFunc);
	glutSpecialFunc(keyboardFuncSpec);
	glutReshapeFunc(reshape);
	glutPassiveMotionFunc(mouseMovement);
	glutEntryFunc(mouseEntered);
}

void draw()
{
	_myGame->run();
}

void keyboardFunc(unsigned char key, int x, int y)
{
	_myGame->keyboardFunc(key, x, y);

}

void keyboardLiftFunc(unsigned char key, int x, int y)
{

}

void keyboardFuncSpec(int key, int x, int y)
{

}

void keyboardLiftFuncSpec(int key, int x, int y)
{

}

void reshape(int width, int height)
{

}

void mouseMovement(int x, int y)
{

}

void mouseEntered(int state)
{

}
