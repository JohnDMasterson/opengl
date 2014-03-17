#include "game.h"


using namespace std;
Game::Game()
{

	//sets keys to not being pressed
	for (int i = 0; i < 256; i++)
	{
		_key[i] = false;
	}

	_gameIsRunning = false;

}

Game::~Game()
{

}

/*
*	Takes parameters from main method for initialization
*	
*	Returns false if something didn't initialize correctly
*	Returns true if everything was initialized alight
*/
bool Game::init(int argv, char** argc)
{
	//initializes glut
	glutInit(&argv, argc);
	glutCreateWindow("Cross Compile Trial 1");

	//ANTI-ALIASING
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);

	//Depth and 3D modeling
	glEnable(GL_DEPTH_TEST);

	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);




	//initializes glew
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		return false;
	}


	//since glut and glew initialized fine, the main loop can run now
	_gameIsRunning = true;

	//returns true if everything was initialized alight
	return true;
}

void Game::keyboardFunc(char key, int x, int y)
{
	_key[key] = true;
}


/*
*	This is the main loop for the engine. 
*	I am using this instead of glutMainLoop() because I think that this will enable me to be more precise and do more
*	I lied. At the moment, my loop has issues, so I am using glutMainLoop till I can figure it out better
*
*/
void Game::run()
{
	keyHandler();
	draw();
	
}

void Game::keyHandler()
{
	if (_key[27])
	{
		glutLeaveMainLoop();
	}


}

void Game::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_myObjects.at(0)->draw();

	glutSwapBuffers();
}

void Game::trialSettings()
{
	DObject * tri = new DObject();

	float verts[] = {
		0.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f
	};

	int asdf = 10000000;
	float * absurd = new float[3* asdf];
	for (int i = 0; i < asdf; i++)
	{
		absurd[3 * i] = 0.0f;
		absurd[3 * i+1] = 0.0f;
		absurd[3 * i+2] = 0.0f;
	}

	tri->addVertices(absurd, asdf);
	
	delete[] absurd;


	int inds[] = {
		0,	1,	2
	};

	tri->addIndexArray(inds, 3);

	tri->generateNormals();
	tri->generateVBO();
	tri->generateIBO();


	tri->createShaderProgram("./shaders/default.vert", "./shaders/green.frag");

	_myObjects.push_back(tri);
}