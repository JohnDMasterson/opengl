#ifndef SPHERE_BOUNDED
#define SPHERE_BOUNDED


//windows specific stuff
#ifdef WIN32
#include <windows.h>
#define GLEW_STATIC
#define GLUT_DISABLE_ATEXIT_HACK
#define GLAPI extern
#endif

//opengl and window stuff
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/GL.h>


//my files to include
#include "dobj.h"
#include "math3d.h"
#include "readfile.h"


//other includes
#include <iostream>
#include <vector>



class SBObject : public DObject
{
	public:
		//constructors and destructors
		SBObject();
		~SBObject();

		void init();

		//virtual overrides
		void draw();

		//shader related methods
		GLuint createShader(const char * filename, GLenum sType);
		bool createShaderProgram(const char* vertFile, const char* fragFile);

		//vertex specific methods
		void addVertices(float * vertices, int arraySize);
		void addIndiceArray(int * indices, int arraySize);
		void createBuffers();

		void setLightColor(Vector3f color);
		void setLightAmbient(float intensity);
		void setLightDiffuse(float intensity);
		void setLightSpecular(float intensity);
		void setLightPosition(Vector3f position);

		void initLight(Vector3f sourcePosition, float aInt, float dInt, float sInt, Vector3f lColor);


		//sphere specific methods
		void setRadius(float radius);
		float getRadius();

	private:


		//other stuff
		float _radius;


		//vertex stuff
		std::vector<float> _vertices;
		GLuint _VBO;

		std::vector< std::vector <int> > _indices;
		std::vector<float> _normals;
		std::vector< std::pair<GLuint, int > > _IBO;




		//shader stuff
		GLuint _sp, _vs, _fs;

		GLuint _gWVP;
		GLuint _lcolor, _ambient, _diffuse, _specular, _lposition, _cposition;
		float _a2, _d2, _s2;
		Vector3f _lp, _lc;
		Reader _fileIn;

};

#endif	//SPHERE_BOUNDED
