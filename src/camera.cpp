#include "camera.h"

using namespace std;
Camera::Camera()
{
	//setting up perspective
	_xres = 1;
	_yres = 1;
	_fov = PI_CONST / 4.0f;
	_near = 0.1f;
	_far = 1000.0f;

	_position = Vector3f(0.0f, 0.0f, 0.0f);
	_lookat = Vector3f(0.0f, 0.0f, -1.0f);
	_up = Vector3f(0.0f, 1.0f, 0.0f);
	_right = _up.cross(_lookat).getUnitVector();


	_pUpToDate = false;
	_rUpToDate = false;
	_tUpToDate = false;

}

Matrix4f Camera::getPerspective()
{
	if(!_pUpToDate)
	{
		Matrix4f temp;
		float invTanFOV = 1.0/tanf(_fov/2.0f);
		float ar = 1.0f*_xres/_yres;

		temp.points[0][0] = ar*invTanFOV;
		temp.points[1][1] = invTanFOV;
		temp.points[2][2] = (0.0f-_near-_far)/(_near-_far);	temp.points[2][3] = 2.0f*_near*_far/(_near-_far);
		temp.points[3][2] = 1;


		_perspective = temp;
		_pUpToDate = true;
	}

	return _perspective;
}

/*
*	Matrix to move camera to (0,0,0)
*/
Matrix4f Camera::getTranslation()
{
	if(!_tUpToDate)
	{
		Matrix4f temp;

		temp.makeIdentity();
		temp.points[0][3] = 0 - _position.x;
		temp.points[1][3] = 0 - _position.y;
		temp.points[2][3] = 0 - _position.z;
		_translation = temp;
		_tUpToDate = true;
	}
	return _translation;
}

/*
*	Matrix to rotate camera's lookat to (0,0,1)
*/
Matrix4f Camera::getRotation()
{
	if(!_rUpToDate)
	{
		//UVN or RUV method of arbitrary axis rotation
		Vector3f u = _up.cross(_lookat);	//vector to lookats right
		u.unitize();
		Vector3f v = _lookat.cross(u);				//true up compared to camera
		v.unitize();
		Vector3f n = _lookat.getUnitVector();


		Matrix4f temp(u,v,n);
		temp.makeTranspose();
		_rotation = temp;
		_rUpToDate = true;
	}

	return _rotation;
}

/*
*	Perspective*Rotation*Translation
*/
Matrix4f Camera::getCameraFinal()
{
	return getPerspective()*getRotation()*getTranslation();
}

/*
*	@param position, position to move camera to
*/
bool Camera::moveTo(Vector3f & position)
{
	_position = position;
	_tUpToDate = false;
	return true;
}

/*
*	@param	x, new x position
*	@param	y, new y position
*	@param	z, new z position
*/
bool Camera::moveTo(float x, float y, float z)
{
	Vector3f temp(x,y,z);
	return moveTo(temp);
}

/*
*	Simple movement forward and backwards
*	Positive moves the camera forward
*	Negative moves the camera backwards
*/
void Camera::moveForward(float distance)
{
	_position = _position + _lookat * distance;
	_tUpToDate = false;
}

/*
*	Simple Right and Left movement
*
*	Positive moves the camera left
*	Negative moves the camera right
*/
void Camera::moveRight(float distance)
{
	_position = _position + _right * distance;
	_tUpToDate = false;
}

/*
*	Simple movement up and down
*	Positive moves the camera up
*	Negative moves the camera down
*/
void Camera::moveUp(float distance)
{
	_position = _position + _up * distance;
	_tUpToDate = false;
}

/*
*	Simple method to look up or down
*	Positive looks updards
*	Negative looks downwards
*/
void Camera::rotateUp(float radians)
{

	_lookat = (_up * tanf(radians))+_lookat;
	_lookat.unitize();
	_up = _lookat.cross(_right).getUnitVector();

	_rUpToDate = false;

}

/*
*	Simple method to turn right or left
*	Positive turns right
*	Negative turns left
*/
void Camera::rotateRight(float radians)
{

	_lookat = (_right * tanf(radians))+_lookat;
	_lookat.unitize();
	_right = _up.cross(_lookat).getUnitVector();

	_rUpToDate = false;
}

/*
*	Simple method to roll right or left
*	Positive rolls right
*	Negative rolls left
*/
void Camera::rollRight(float radians)
{
	_up = (_right * tanf(radians))+_up;
	_up.unitize();
	_right = _up.cross(_lookat).getUnitVector();


	_rUpToDate = false;

}

/*
*	@param target, position where target is located
*
*	@return if lookAt != up
*/
bool Camera::lookAt(Vector3f & target)
{
	Vector3f temp = target-_position;
	temp.unitize();
	_lookat = temp;

	if(temp == _up || temp == _up * -1)
	{
		_up = _lookat.cross(_right).getUnitVector();
		_right = _up.cross(_lookat).getUnitVector();
	}
	else
	{
		_right = _up.cross(_lookat).getUnitVector();
		_up = _lookat.cross(_right).getUnitVector();
	}

	return true;
}

/*
*	@param	x, x position of target
*	@param	y, y position of target
*	@param	z, z position of target
*
*	@return if lookAt != up
*/
bool Camera::lookAt(float x, float y, float z)
{
	Vector3f temp(x,y,z);
	return lookAt(temp);
}

/*
*	@param up, vector that represents up
*
*	@return if lookAt != up
*/
bool Camera::setUp(Vector3f & up)
{
	up.unitize();
	_rUpToDate = false;
	if(up!=_lookat)
	{
		_up = up;
		return true;
	}
	return false;

}

/*
*	@param	x, x component of up
*	@param	y, y componont of up
*	@param	z, z componont of up
*
*	@return if lookAt != up
*/
bool Camera::setUp(float x, float y, float z)
{
	Vector3f temp(x,y,z);
	return setUp(temp);
}


void Camera::reshape(int width, int height)
{
	_xres = width;
	_yres = height;
	_pUpToDate = false;
}

void Camera::changeNear(float nearPoint)
{
	_near = nearPoint;
	_pUpToDate = false;
}

void Camera::changeFar(float farPoint)
{
	_far = farPoint;
	_pUpToDate = false;
}

void Camera::changeFOV(float radians)
{
	_fov = radians;
	_pUpToDate = false;
}

void Camera::flush()
{
	_pUpToDate = false;
	_tUpToDate = false;
	_rUpToDate = false;
	getPerspective();
	getTranslation();
	getRotation();
}


Vector3f Camera::getPosition()
{
	return _position;
}
