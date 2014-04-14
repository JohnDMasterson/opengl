#include "spherebounded.h"


using namespace std;

SBObject::SBObject()
{
	setPosition(Vector3f(0.0f, 0.0f, 0.0f));
}

SBObject::SBObject(const char* filename)
{
	ifstream file(filename);
	string temp;
	while(getline(file,temp))
	{
		stringstream ss;
		ss.str(temp);
		string word;
		ss >> word;

		bool texture = false;
		bool normal = false;
		if(word == "v")
		{
			//This line is a vertice then
			cout << "word!" << endl;
			ss >> word;
			int dim = 0;
			float vert[4];
			while(ss.good() && dim < 4)
			{
				vert[dim] = atof(word.c_str());
				ss >> word;
				dim++;
			}
			
			//vert holds up to 4 dimensions
			//dim tells you how many dimensions were included
		}else if(word == "f")
		{
			cout << "face" << endl;
			//This line is a face
			//faces holds all ints for every face
			vector<int> faces;
			while(ss.good())
			{
				if(texture && !normal)
				{
					// of form v/t
					int v = 0, t = 0;
					sscanf(word.c_str(), " %i / %i ", &v, &t);
					faces.push_back(v);
					faces.push_back(t);
				}else if (!texture && normal)
				{
					// of form v/ /n
					int v = 0, n = 0;
					if( sscanf(word.c_str(), " %i / / %i ", &v, &n) < 2)
					{
						// tries hidden int on inside
						int temp;
						sscanf(word.c_str(), " %i / %i / %i ", &v, &temp, &n);
					}
					faces.push_back(v);
					faces.push_back(n);
				}else if( texture && normal)
				{
					// of form v/t/n
					int v = 0, t = 0, n = 0;
					sscanf(word.c_str(), " %i / %i / %i ", &v, &t, &n);
					faces.push_back(v);
					faces.push_back(t);
					faces.push_back(n);
				}else
				{
					// of form v
					int v = 0;
					sscanf(word.c_str(), " %i ", &v);
					faces.push_back(v);
				}
				ss >> word;
			}

		}
	}
}

SBObject::~SBObject()
{
	//delete program when object is deleted
	glDeleteProgram(_sp);

}


void SBObject::draw()
{
	//bind VBO buffer
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);

	//enable locations
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE, 6*sizeof(float), 0);
	glVertexAttribPointer(1,3,GL_FLOAT, GL_FALSE, 6*sizeof(float), (const GLvoid *)12);

	//enable shader
	glUseProgram( _sp );


	glUniform3f(_lcolor, _lc.x, _lc.y, _lc.z);
	glUniform3f(_cposition, _cp.x, _cp.y, _cp.z);
	glUniform1f(_ambient, _a2);
	glUniform1f(_diffuse, _d2);
	glUniform1f(_specular, _s2);
	glUniform3f(_lposition, _lp.x, _lp.y, _lp.z);


	//load matrices 
	Matrix4f fTemp = _cameraPerspective * getTranslationMatrix();
	glUniformMatrix4fv(_gWVP, 1, GL_TRUE, &fTemp.points[0][0]);

	//draw object

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	vector< pair<GLuint, int > >::iterator iboIt;
	for(iboIt = _IBO.begin(); iboIt != _IBO.end(); iboIt++)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboIt->first );
		glDrawElements(GL_TRIANGLE_STRIP, iboIt->second , GL_UNSIGNED_INT, 0);
	}

	//disable shader
	glUseProgram(0);

	//disable locations
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	//disable buffers
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	glBindBuffer(GL_ARRAY_BUFFER,0);
		
}

/*
*	Creates a shader from the current file an returns it
*
*	@input filename, file to read GLSL code from
*	@input sType, the type of shader to create
*
*	@return GLuint, shader that was created
*/
GLuint SBObject::createShader(const char * filename, GLenum sType)
{

	GLint success;
	GLuint retu = glCreateShader(sType);

	if(retu == 0)
	{
		cout << "error creating shader" << endl;
		exit(0);
	}

	string temp = _fileIn.read(filename);
	const GLchar* stext[1];
	GLint slen[1];
	stext[0] = temp.c_str();
	slen[0] = temp.length();

	// binds data and compiles
	glShaderSource(retu, 1, stext, slen);
	glCompileShader(retu);

	//cehcks for errors
    glGetShaderiv(retu, GL_COMPILE_STATUS, &success);
    if (!success) 
    {
        GLchar InfoLog[1024];
        glGetShaderInfoLog(retu, 1024, NULL, InfoLog);
        fprintf(stderr, "Error compiling shader:'%s'\n", InfoLog);
        cout << stext[0] << endl;
        exit(1);
    }

    return retu;
}

void SBObject::init()
{


}

/*
*	return index of the created shader program
*/
bool SBObject::createShaderProgram(const char* vertFile, const char* fragFile)
{
	_sp = glCreateProgram();

	//program didn't initialize properly
	if(_sp == 0)
		return false;

	_vs = createShader(vertFile, GL_VERTEX_SHADER);
	glAttachShader(_sp,_vs);

	_fs = createShader(fragFile, GL_FRAGMENT_SHADER);
	glAttachShader(_sp,_fs);

	glLinkProgram(_sp);
	glValidateProgram(_sp);


	//detach and delete after linking
	glDetachShader(_sp, _vs);
	glDeleteShader(_vs);
	glDetachShader(_sp, _fs);
	glDeleteShader(_fs);

	//GLSL variables
	_gWVP = glGetUniformLocation(_sp, "gWVP");
    _lcolor = glGetUniformLocation(_sp, "sunlight.color");
    _ambient = glGetUniformLocation(_sp, "sunlight.ambient");
    _diffuse = glGetUniformLocation(_sp, "sunlight.diffuse");
    _specular = glGetUniformLocation(_sp, "sunlight.specular");
    _lposition= glGetUniformLocation(_sp, "sunlight.position");
    _cposition = glGetUniformLocation(_sp, "sunlight.camera");




	return true;
}

void SBObject::addVertices(float * vertices, int arraySize)
{
	for (int i = 0; i < arraySize; i++)
	{
		float temp = vertices[i];
		_vertices.push_back(temp);
	}
}

void SBObject::addIndiceArray(int * indices, int arraySize)
{
	vector<int> iTemp;
	for(int i = 0; i < arraySize; i++)
	{
		int temp = indices[i];
		iTemp.push_back(temp);
	}
	_indices.push_back(iTemp);
}

/*
*	Finds normals, creates array for vertices, 
*	buffers vertices
*/
void SBObject::createBuffers()
{

	//total number of vertices needed is 2 * vertices
	//1 set for position, 1 set for normals
	int icount = 2 * _vertices.size();
	float * vertnorm = new float[icount];

	//initializes the first 3 in each pair of 6 to be positions
	for(int i = 0; i < icount; i+=6)
	{
		vertnorm[i] = _vertices[i/2];
		vertnorm[i+1] = _vertices[i/2+1];
		vertnorm[i+2] = _vertices[i/2+2];
		vertnorm[i+3] = 0;
		vertnorm[i+4] = 0;
		vertnorm[i+5] = 0;

	}

	//calculate normals and buffer indices into _IBO
	vector< vector <int> >::iterator it;
	for(it = _indices.begin(); it != _indices.end(); it++)
	{
		int * indices = new int[it->size()];
		cout << sizeof(*indices) << endl;
		cout << it->size() << " * " << sizeof(int) << endl;
		//loop for IBOs
		for(int i = 0; i < it->size(); i++)
		{
			indices[i] = (*it)[i];
		}
		GLuint iTemp;
		glGenBuffers(1, &iTemp );
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iTemp);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4*it->size(), indices, GL_STATIC_DRAW);
	
		_IBO.push_back(pair<GLuint, int > (iTemp, it->size()));
	

		int correct = 1;
		//loop for vertex normals
		for(int i = 0; i < it->size()-2; i++)
		{
			//indexa/b/c are the index where the vertice is stored
			//I multiply it by 3 because there are 3 numbers to a vertice
			int indexa = 3*indices[i];
			Vector3f a(_vertices[indexa], _vertices[indexa+1], _vertices[indexa+2]);

			int indexb = 3*indices[i+1];
			Vector3f b(_vertices[indexb], _vertices[indexb+1], _vertices[indexb+2]);

			int indexc = 3*indices[i+2];
			Vector3f c(_vertices[indexc], _vertices[indexc+1], _vertices[indexc+2]);

			//my two vectors made from vertices
			b = a-b;
			c = a-c;

			//cross them to find normal
			a = b.cross(c)* correct;
			correct *= -1;
			a.unitize();

			//add them to corresponding entry in vertnorm array
			//noramlize at the very end
			vertnorm[2*indexa+3] += a.x;
			vertnorm[2*indexa+4] += a.y;
			vertnorm[2*indexa+5] += a.z;

			vertnorm[2*indexb+3] += a.x;
			vertnorm[2*indexb+4] += a.y;
			vertnorm[2*indexb+5] += a.z;

			vertnorm[2*indexc+3] += a.x;
			vertnorm[2*indexc+4] += a.y;
			vertnorm[2*indexc+5] += a.z;

		}
		//this iterates through every IBO

		delete[] indices;

	}
	Vector3f up(0.0f, 1.0f, 0.0f);
	//normalizing all normals

	for(int i = 0; i < icount; i+=6)
	{
		Vector3f vntemp(vertnorm[i+3],vertnorm[i+4],vertnorm[i+5]);
		vntemp.unitize();

		vertnorm[i+3] = vntemp.x;
		vertnorm[i+4] = vntemp.y;
		vertnorm[i+5] = vntemp.z;
	}


	glGenBuffers(1, &_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, icount, vertnorm, GL_STATIC_DRAW);
	


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);





}

void SBObject::setRadius(float radius)
{
	_radius = radius;
}

float SBObject::getRadius()
{
	return _radius;
}












void SBObject::setLightColor(Vector3f color)
{
	_lc = color;
}

void SBObject::setLightAmbient(float intensity)
{
	_a2 = intensity;
}

void SBObject::setLightDiffuse(float intensity)
{
	_d2 = intensity;
}

void SBObject::setLightSpecular(float intensity)
{
	_s2 = intensity;
}

void SBObject::setLightPosition(Vector3f position)
{
	_lp = position;
}

void SBObject::initLight(Vector3f position, float aInt, float dInt, float sInt, Vector3f color)
{
	glUniform3f(_lposition, position.x, position.y, position.z);
	glUniform1f(_specular, aInt);
	glUniform1f(_diffuse, dInt);
	glUniform1f(_specular, sInt);
	glUniform3f(_lcolor, color.x, color.y, color.z);
}
