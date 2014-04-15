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
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800,800);
	_xres = _yres = 800;
	glutCreateWindow("Cross Compile Trial 1");


	glutSetCursor(GLUT_CURSOR_NONE);

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
void Game::keyboardLiftFunc(char key, int x, int y)
{
	_key[key] = false;
}


/*
*	This is the main loop for the engine. 
*	I am using this instead of glutMainLoop() because I think that this will enable me to be more precise and do more
*	I lied. At the moment, my loop has issues, so I am using glutMainLoop till I can figure it out better
*
*/
void Game::run()
{
	draw();
	
}

void Game::keyHandler()
{
	
	if(_key['w'])
	{
		_myCamera.moveForward(0.05f);
	}
	if(_key['s'])
	{
		_myCamera.moveForward(-0.05f);
	}
	if(_key['d'])
	{
		_myCamera.moveRight(0.05f);
	}
	if(_key['a'])
	{
		_myCamera.moveRight(-0.05f);
	}
	if(_key[27])
	{
		glutLeaveMainLoop();
	}
	if(_key['q'])
	{
		_myCamera.rotateRight(-0.05f);
	}
	if(_key['e'])
	{
		_myCamera.rotateRight(0.05f);
	}


}

void Game::draw()
{

	if(glutGet(GLUT_ELAPSED_TIME)-_lastTimeDrawn >= 10/6)
	{

	keyHandler();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Matrix4f cpm = _myCamera.getCameraFinal();



	_myObjects.at(0)->setCameraPosition(_myCamera.getPosition());
	_myObjects.at(0)->setCameraPerspective(cpm);
	_myObjects.at(0)->draw();

	_lastTimeDrawn = glutGet(GLUT_ELAPSED_TIME);
	glutSwapBuffers();
	}

}

void Game::trialSettings()
{

	DObject * tri = new DObject("./models/teapot.obj");

	tri->generateNormals();
	tri->generateVBO();
	tri->generateIBO();



	tri->createShaderProgram("./shaders/default.vert", "./shaders/green.frag");

	tri->setCameraPosition( _myCamera.getPosition());
	tri->setLightAmbient(0.3f);
	tri->setLightPosition(Vector3f(10.0f, 10.0f, 0.0f));
	tri->setLightDiffuse(0.3f);
	tri->setLightColor(Vector3f(1.0f, 1.0f, 1.0f));
	tri->setLightSpecular(0.0f);



	_myObjects.push_back(tri);



	_myCamera.lookAt(1.0f, 0.0f, 1.0f);
	_myCamera.moveTo(0.0f, 0.0f, -1.0f);
	_myCamera.flush();



}








/*
*	Handle for when a mouse is moved
*/
void Game::mouseMoved(int x, int y)
{
	float dx = 1.0f*x-_xm;
	float dy = 1.0f*y-_ym;

	if(!_jumped)
	{
		if( abs(dx) > 1)
		{
			_myCamera.rollRight(dx/200);
		}
		if( abs(dy) > 1)
		{
			_myCamera.rotateUp(dy/200);
		}
	}
	_jumped = false;

	if( _xres-x < _xres/20 || _xres-x > 19*_xres/20 || _yres-y < _yres/20 || _yres-y > 19*_yres/20)
	{
		_jumped = true;
		_ym = _yres/2;
		_xm = _xres/2;
		glutWarpPointer(_xm,_ym);
	}
	else
	{
		_xm = x;
		_ym = y;
	}

}

/*
*	Forces mouse to stay in frame
*
*/
void Game::mouseEntered(int state)
{
		_jumped = true;
		_xm = _xres/2;
		_ym = _yres/2; 
		glutWarpPointer(_xm,_ym);
}




/*
*	Updates resolution of world for perspective
*/
void Game::reshape(int width, int height)
{
	_xres = width;
	_yres = height;
	_myCamera.reshape(_yres, _xres);
}