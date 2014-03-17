#ifndef MATH3D
#define MATH3D

#include <math.h>

#include <iostream>


/*
*	Quick Vertex Struct
*
*/
struct Vector3f
{
	float x, y, z;

	Vector3f();
	Vector3f(float myX, float myY, float myZ);

	
	Vector3f getUnitVector() const;
	Vector3f cross(Vector3f const &other) const;
	float dot(Vector3f const &other) const;
	float length() const;
	void unitize();

	Vector3f operator+(Vector3f const &other) const;
	Vector3f operator-(Vector3f const &other) const;
	bool operator==(Vector3f const &other) const;
	bool operator!=(Vector3f const &other) const;
	Vector3f operator=(Vector3f const &other);
	Vector3f operator*(float const &scalor);

};

/*
*	Quick Matrix struct for easier multiplication
*
*
*/
struct Matrix4f
{
	float points[4][4];

	Matrix4f();
	Matrix4f(Vector3f &A, Vector3f &B, Vector3f &C);

	void makeIdentity();
	void clear();
	void makeTranspose();

	Matrix4f operator*(const Matrix4f &other) const;
	Vector3f operator*(const Vector3f &vec3) const;

	void print();
};

#endif
