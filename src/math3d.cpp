#include "math3d.h"


/*
*	Vector3f methods
*
*/

Vector3f::Vector3f()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

Vector3f::Vector3f(float myX, float myY, float myZ)
{
	x = myX;
	y = myY;
	z = myZ;
}

Vector3f Vector3f::getUnitVector() const
{
	Vector3f retu;
	float length = this->length();
	retu.x = x/length;
	retu.y = y/length;
	retu.z = z/length;
	return retu;
}

Vector3f Vector3f::cross(Vector3f const &other) const
{
	Vector3f retu;
	retu.x = y*other.z - z*other.y;
	retu.y = z*other.x - x*other.z;
	retu.z = x*other.y - y*other.x;
	return retu;
}

float Vector3f::dot(Vector3f const &other) const
{
	float d;
	d = x*other.x + y*other.y * z*other.z;
	return d;

}

float Vector3f::length() const
{
	return sqrt( pow(x,2) + pow(y,2) + pow(z,2));
}

void Vector3f::unitize()
{
	float length = this->length();
	if(length != 0)
	{
		x /= length;
		y /= length;
		z /= length;
	}
	else
	{
		x = y = z = 0;
	}
}

Vector3f Vector3f::operator+(Vector3f const &other) const
{
	Vector3f retu;
	retu.x = x + other.x;
	retu.y = y + other.y;
	retu.z = z + other.z;
	return retu;
}

Vector3f Vector3f::operator-(Vector3f const &other) const
{
	Vector3f retu;
	retu.x = x - other.x;
	retu.y = y - other.y;
	retu.z = z - other.z;
	return retu;
}

bool Vector3f::operator==(Vector3f const &other) const
{
	return ((x == other.x) && (y == other.y) && (z == other.z));
}

bool Vector3f::operator!=(Vector3f const &other) const
{
	return !(*this==other);
}

Vector3f Vector3f::operator=(Vector3f const &other)
{
	x = other.x;
	y = other.y;
	z = other.z;
	return *this;
}

Vector3f Vector3f::operator*(float const &scalor)
{
	Vector3f retu;
	retu.x = x*scalor;
	retu.y = y*scalor;
	retu.z = z*scalor;
	return retu;
}



/*
*	Matrix4f methods
*
*/


Matrix4f::Matrix4f()
{
	clear();
}

Matrix4f::Matrix4f(Vector3f &A, Vector3f &B, Vector3f &C)
{
	points[0][0] = A.x;	points[0][1] = B.x;	points[0][2] = C.x;	points[3][0] = 0;
	points[1][0] = A.y;	points[1][1] = B.y;	points[1][2] = C.y;	points[3][1] = 0;
	points[2][0] = A.z;	points[2][1] = B.z;	points[2][2] = C.z;	points[3][2] = 0;
	points[3][0] = 0;	points[3][1] = 0;	points[3][2] = 0;	points[3][3] = 1;
}

void Matrix4f::makeIdentity()
{
	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
			if(i == j)
				points[i][j] = 1.0f;
			else
				points[i][j] = 0.0f;
}


void Matrix4f::clear()
{
	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
			points[i][j] = 0.0f;
}

void Matrix4f::makeTranspose()
{
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < i; j++)
		{
			float temp = points[i][j];
			points[i][j] = points[j][i];
			points[j][i] = temp;
		}
	}
}

Matrix4f Matrix4f::operator*(const Matrix4f &other) const
{
	Matrix4f retu;
	retu.clear();
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			for(int k = 0; k < 4; k++)
			{
				retu.points[i][k] += points[i][j] * other.points[j][k]; 
			}
		}
	}
	return retu;
}

Vector3f Matrix4f::operator*(const Vector3f &vec3) const
{
	Vector3f retu;
	retu.x = (vec3.x*points[0][0] + vec3.y*points[1][0] + vec3.z*points[2][0]);
	retu.y = (vec3.x*points[0][1] + vec3.y*points[1][1] + vec3.z*points[2][1]);
	retu.z = (vec3.x*points[0][2] + vec3.y*points[1][2] + vec3.z*points[2][2]);
	return retu;
}

void Matrix4f::print()
{
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			std::cout << points[i][j] << "   " ;
		}
		std::cout << std::endl;
	}

}