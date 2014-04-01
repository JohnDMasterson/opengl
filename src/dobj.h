#ifndef	DOBJECT
#define DOBJECT

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


//my files to include
#include "math3d.h"
#include "readfile.h"


//other includes
#include <vector>

class DObject
{
public:

	//constructor and destructor
	DObject();
	~DObject();

	//drawing methods
	void draw();
	Matrix4f getTranslationMatrix();

	//collision detection
	bool canCollide();
	void setCanCollide(bool canCollide);

	//position getting and setting
	void setPosition(Vector3f position);
	void setPosition(float x, float y, float z);
	Vector3f getPosition();

	//camera stuff
	void setCameraPerspective(Matrix4f cameraPerspective);
	void setCameraPosition(Vector3f position);


	//vertice methods
	void addVertices(float * vertices, int numVertices);
	void addIndexArray(int * indices, int numIndices);
	void generateNormals();
	void generateVBO();
	void generateIBO();

	void printVerticesToTerminal();

	//shader related methods
	GLuint createShader(const char * filename, GLenum sType);
	bool createShaderProgram(const char* vertFile, const char* fragFile);

private:



protected:

	struct VertNorm
	{
		float coords[6];
	};

	//object properties
	bool _canCollide;
	Vector3f _position;

	//matricies for drawing
	Matrix4f _cameraPerspective;

	//vertice variables
	VertNorm * _vertices;
	int _numVertices;
	GLuint _VBO;

	//indices and normals
	std::vector< std::pair<int *, int> > _indices;
	std::vector< std::pair<GLuint, int> > _IBO;

	//shader program
	GLuint _sp, _vs, _fs;

	//camera position
	Vector3f _cp;

	//reader for file parsing
	Reader _fileIn;
};

#endif
