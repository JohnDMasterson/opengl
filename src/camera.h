#ifndef CAMERA_GL
#define CAMERA_GL

#define PI_CONST 3.1415926

#include <GL/glew.h>		//GLEW
#include <GL/freeglut.h> 	//glut
#include <iostream>

#include "math3d.h"

class Camera
{
public:
	//Constructors
	Camera();

	//Pipeline Matricies
	Matrix4f getPerspective();
	Matrix4f getTranslation();
	Matrix4f getRotation();
	Matrix4f getCameraFinal();


	//methods for moving camera
	void moveForward(float distance);
	void moveRight(float distance);
	void moveUp(float distance);
	void rotateUp(float radians);
	void rotateRight(float radians);
	void rollRight(float radians);

	//methods to directly controls camera states
	bool moveTo(Vector3f & position);
	bool moveTo(float x, float y, float z);
	bool lookAt(Vector3f & target);
	bool lookAt(float x, float y, float z);
	bool setUp(Vector3f & up);
	bool setUp(float x, float y, float z);

	//perspective changing methods
	void reshape(int width, int height);
	void changeNear(float nearPoint);
	void changeFar(float farPoint);
	void changeFOV(float radians);

	void flush();

	Vector3f getPosition();


private:

	//perspective variables
	int _xres, _yres;
	float _near, _far;
	float _fov;


	//camera state variables
	Vector3f _position;
	Vector3f _lookat, _up, _right;


	//matricies for drawing
	Matrix4f _perspective;
	bool _pUpToDate;
	Matrix4f _rotation;
	bool _rUpToDate;
	Matrix4f _translation;
	bool _tUpToDate;

};


#endif
