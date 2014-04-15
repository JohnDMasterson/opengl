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
#include <iostream>
#include <vector>
#include <stdlib.h> // atof
#include <sstream>  // stringstream
#include <fstream> // ifstream
class DObject
{
public:

	//constructor and destructor
	DObject();
	DObject(const char* filename);
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
	void generateNormals();
	void generateVBO();
	void generateIBO();

	//shader related methods
	GLuint createShader(const char * filename, GLenum sType);
	bool createShaderProgram(const char* vertFile, const char* fragFile);

	void addVertice(float x, float y, float z);
	void addFace(std::vector< int * > face);







	void setLightColor(Vector3f color);
		void setLightAmbient(float intensity);
		void setLightDiffuse(float intensity);
		void setLightSpecular(float intensity);
		void setLightPosition(Vector3f position);

		void initLight(Vector3f sourcePosition, float aInt, float dInt, float sInt, Vector3f lColor);



private:



protected:

	struct VertNorm
	{
		float coords[6];
	};

	struct VTN
	{
		int vtn[3];
	};

	struct objFace
	{
		std::vector< int * > indices;
		bool textures;
		bool norms;
	};

	struct objFile
	{
		std::vector< float * > vertices;
		std::vector< float * > tcoords;
		std::vector< float * > norms;
		std::vector< objFace > faces;

		void print()
		{
			std::cout << "Verticies : " << vertices.size() << std::endl;
			for(int i = 0; i < vertices.size(); i++)
			{
				std::cout << "(" << vertices[i][0] << "," << vertices[i][1] << "," << vertices[i][2] << "," << vertices[i][3] << ")" << std::endl;
			}
			std::cout << std::endl;

			std::cout << "Normals : " << norms.size() << std::endl;
			for(int i = 0; i < norms.size(); i++)
			{
				std::cout << "(" << norms[i][0] << "," << norms[i][1] << "," << norms[i][2] << ")" << std::endl;
			}
			std::cout << std::endl;

			std::cout << "Texture Coordinates : " << tcoords.size() << std::endl;
			for(int i = 0; i < tcoords.size(); i++)
			{
				std::cout << "(" << tcoords[i][0] << "," << tcoords[i][1] << "," << tcoords[i][2] << ")" << std::endl;
			}
			std::cout << std::endl;

			std::cout << "Faces : " << faces.size() << std::endl;
			for(int i = 0; i < faces.size(); i++)
			{
				for(int j = 0; j < faces[i].indices.size(); j++)
				{
					std::cout << faces[i].indices[j][0] << "/";
					if(faces[i].textures)
						std::cout << faces[i].indices[j][1];
					std::cout << "/";
					if(faces[i].textures)
						std::cout << faces[i].indices[j][2];
					std::cout << "   ";
				}
				std::cout << std::endl;
			}
		}
	};


	//object properties
	bool _canCollide;
	Vector3f _position;

	//matricies for drawing
	Matrix4f _cameraPerspective;

	//vertices
	std::vector< VertNorm > _vertnorm;
	GLuint _VBO;

	//indices and normals
	std::vector< VTN > _faces;
	std::vector< int > _vertsInFace;

	GLsizei* count;
	GLvoid** indices;

	GLuint _IBO;

	//shader program
	GLuint _sp, _vs, _fs;

	//camera position
	Vector3f _cp;

	//reader for file parsing
	Reader _fileIn;








		GLuint _gWVP;
		GLuint _lcolor, _ambient, _diffuse, _specular, _lposition, _cposition;
		float _a2, _d2, _s2;
		Vector3f _lp, _lc;
};

#endif
