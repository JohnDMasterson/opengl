#include "dobj.h"


using namespace std;

DObject::DObject()
{
}

DObject::~DObject()
{
}

DObject::DObject(const char* filename)
{
	ifstream file(filename);
	string temp;


	objFile thisObj;
	while(getline(file,temp))
	{
		stringstream ss;
		ss.str(temp);
		string word;
		ss >> word;

		if(word == "v")
		{
			//This line is a vertice then

			int dim = 0;
			float * vert = new float[4];
			while(ss.good() && dim < 4)
			{	
				ss >> word;
				vert[dim] = atof(word.c_str());
				dim++;
			}
			while(dim < 4)
			{
				vert[dim] = 0.0f;
				dim++;
			}

			if(dim == 3 || dim == 4)
			{
				thisObj.vertices.push_back(vert);
			}
		}else if(word == "f")
		{
			//This line is a face
			//faces holds all ints for every face
			word = ss.peek();
			string part = word;
			objFace thisFace;

			//tests for what type of face this is 
			int temp;
			int a = sscanf(part.c_str(), " %i ", &temp);
			int b = sscanf(part.c_str(), " %i / %i ", &temp, &temp);
			int c = sscanf(part.c_str(), " %i // %i ", &temp, &temp);
			int d = sscanf(part.c_str(), " %i / %i / %i ", &temp, &temp, &temp);
			if(a >= b && a>=c && a>=d)
			{
				thisFace.textures = false;
				thisFace.norms = false;
			}else if (b >=a && b >= c && b >= d)
			{
				thisFace.textures = true;
				thisFace.norms = false;
			}else if (c >=a && c >= b && c >= d)
			{
				thisFace.textures = false;
				thisFace.norms = true;
			}else if (d >=a && d >= b && d >= c)
			{
				thisFace.textures = true;
				thisFace.norms = true;
			}else
			{
				break;
			}

			while(ss.good())
			{
				ss >> word;
				int * tripple = new int[3];
				if(!thisFace.textures && !thisFace.norms)
				{
					// of form v/t
					int v = 0;
					sscanf(word.c_str(), " %i ", &v);
					tripple[0] = v;
					tripple[1] = 0;
					tripple[2] = 0;
					thisFace.indices.push_back(tripple);
				}else if(thisFace.textures && !thisFace.norms)
				{
					// of form v/t
					int v = 0, t = 0;
					sscanf(word.c_str(), " %i / %i ", &v, &t);
					tripple[0] = v;
					tripple[1] = t;
					tripple[2] = 0;
					thisFace.indices.push_back(tripple);
				}else if(!thisFace.textures && thisFace.norms)
				{
					// of form v/t
					int v = 0, n = 0;
					sscanf(word.c_str(), " %i // %i ", &v, &n);
					tripple[0] = v;
					tripple[1] = 0;
					tripple[2] = n;
					thisFace.indices.push_back(tripple);
				}else if(thisFace.textures && thisFace.norms)
				{
					// of form v/t
					int v = 0, t = 0, n = 0;
					sscanf(word.c_str(), " %i / %i / %i ", &v, &t, &n);
					tripple[0] = v;
					tripple[1] = t;
					tripple[2] = n;
					thisFace.indices.push_back(tripple);
				}
			}
			thisObj.faces.push_back(thisFace);
		}else if(word == "vn")
		{
			float * tripple = new float[3];
			int dim = 0;
			while(ss.good() && dim < 3)
			{
				ss >> word;
				tripple[dim] = atof(word.c_str());
				dim++;
			}
			while(dim < 3)
			{
				tripple[dim] = 0.0f;
			}
			thisObj.norms.push_back(tripple);
		}else if(word == "vt")
		{
			float * tripple = new float[3];
			int dim = 0;
			while(ss.good() && dim < 3)
			{
				ss >> word;
				tripple[dim] = atof(word.c_str());
				dim++;
			}
			while(dim < 3)
			{
				tripple[dim] = 0.0f;
			}
			thisObj.tcoords.push_back(tripple);
		}
	}







	addVertice(0.0f, 0.0f, 0.0f);
	//adds vertices to object after loading
	for(int i = 0; i < thisObj.vertices.size(); i++)
	{
		addVertice(thisObj.vertices[i][0],thisObj.vertices[i][1],thisObj.vertices[i][2]);
	}

	for(int i = 0; i < thisObj.faces.size(); i++)
	{
		addFace(thisObj.faces[i].indices);
	}

}


void DObject::setCanCollide(bool canCollide)
{
	_canCollide = canCollide;
}

bool DObject::canCollide()
{
	return _canCollide;
}




void DObject::setPosition(Vector3f position)
{
	_position = position;
}
void DObject::setPosition(float x, float y, float z)
{
	Vector3f temp(x,y,z);
	setPosition(temp);
}
Vector3f DObject::getPosition()
{
	return _position;
}





Matrix4f DObject::getTranslationMatrix()
{
	Matrix4f retu;
	retu.makeIdentity();

	retu.points[0][3] = 0 - _position.x;
	retu.points[1][3] = 0 - _position.y;
	retu.points[2][3] = 0 - _position.z;

	return retu;
}






void DObject::setCameraPerspective(Matrix4f cameraPerspective)
{
	_cameraPerspective = cameraPerspective;
}


void DObject::setCameraPosition(Vector3f position)
{
	_cp = position;
}


void DObject::addVertice(float x, float y, float z)
{
	VertNorm temp;
	temp.coords[0] = x;
	temp.coords[1] = y;
	temp.coords[2] = z;
	temp.coords[3] = 0.0f;
	temp.coords[4] = 0.0f;
	temp.coords[5] = 0.0f;

	_vertnorm.push_back(temp);
}

void DObject::addFace(vector< int * > face)
{
	for(int i = 0; i < face.size(); i++)
	{
		VTN temp;
		temp.vtn[0] = face[i][0];
		temp.vtn[1] = face[i][1];
		temp.vtn[2] = face[i][2];
		_faces.push_back(temp);
	}
	_vertsInFace.push_back(face.size());
}

/*
*	finds all normals
*	assumes usage of triangles
*	
*/
void DObject::generateNormals()
{

	//resets all normals to (0,0,0)
	for (int i = 0; i < _vertnorm.size(); i++)
	{
		_vertnorm[i].coords[3] = 0;
		_vertnorm[i].coords[4] = 0;
		_vertnorm[i].coords[5] = 0;
	}


	//incriments through all faces
	int last = 0;
	count = new GLsizei[_vertsInFace.size()];
	indices = new GLvoid*[_vertsInFace.size()];
	for (int i = 0; i < _vertsInFace.size(); i++)
	{
		for(int j = 0; j<_vertsInFace[i]; j++)
		{
			//finds index in _vertnorm

			int prevIndex;
			if(j == 0)
				prevIndex = last+_vertsInFace[i];
			else
				prevIndex = last+j-1;

			prevIndex = _faces[prevIndex].vtn[0];

			int nextIndex;
			if(j == _vertsInFace[i]-1)
				nextIndex = last;
			else
				nextIndex = last+j;

			nextIndex = _faces[nextIndex].vtn[0];

			int thisIndex = _faces[j].vtn[0];


			//gets the current, previous, and next point from indices
			Vector3f a(_vertnorm[thisIndex].coords[0],_vertnorm[thisIndex].coords[1],_vertnorm[thisIndex].coords[2]);
			Vector3f b(_vertnorm[prevIndex].coords[0],_vertnorm[prevIndex].coords[1],_vertnorm[prevIndex].coords[2]);
			Vector3f c(_vertnorm[nextIndex].coords[0],_vertnorm[nextIndex].coords[1],_vertnorm[nextIndex].coords[2]);

			//gets two vectors from the points
			Vector3f ab = a - b;
			Vector3f ac = a - c;

			//finds normal of the plane
			Vector3f abcNorm = ab.cross(ac);
			abcNorm.unitize();

			//adds the normal to the points normal list
			_vertnorm[thisIndex].coords[3] += abcNorm.x;
			_vertnorm[thisIndex].coords[4] += abcNorm.y;
			_vertnorm[thisIndex].coords[5] += abcNorm.z;
		}
		count[i] = _vertsInFace[i];

		int temp = 0;
		for(int j = 0; j < i; j++)
			temp += _vertsInFace[j];
		indices[i] = (GLvoid*) (temp*sizeof(GLuint));
	}

	//normalizes all of the vertex normals
	for (int i = 0; i < _vertnorm.size(); i++)
	{
		Vector3f temp;
		temp.x = _vertnorm[i].coords[3];
		temp.y = _vertnorm[i].coords[4];
		temp.z = _vertnorm[i].coords[5];

		temp.unitize();


		_vertnorm[i].coords[3] = temp.x;
		_vertnorm[i].coords[4] = temp.y;
		_vertnorm[i].coords[5] = temp.z;
	}

}

void DObject::generateVBO()
{
	VertNorm * temp = new VertNorm[_vertnorm.size()];
	for(int i = 0; i < _vertnorm.size(); i++)
	{
		temp[i] = _vertnorm[i];
	}

	glGenBuffers(1, &_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, _vertnorm.size() * sizeof(VertNorm), temp, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void DObject::generateIBO()
{
	int * temp = new int[_faces.size()];
	for(int i = 0; i < _faces.size(); i++)
	{
		temp[i] = _faces[i].vtn[0];
	}
	glGenBuffers(1, &_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _faces.size() * sizeof(int), temp, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}




void DObject::draw()
{

	//bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _IBO);
	glUseProgram(_sp);

	//enable vertices
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertNorm), 0);
	//enable normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertNorm), (const GLvoid*)12);



	glUniform3f(_lcolor, _lc.x, _lc.y, _lc.z);
	glUniform3f(_cposition, _cp.x, _cp.y, _cp.z);
	glUniform1f(_ambient, _a2);
	glUniform1f(_diffuse, _d2);
	glUniform1f(_specular, _s2);
	glUniform3f(_lposition, _lp.x, _lp.y, _lp.z);


	//load matrices 
	Matrix4f fTemp = _cameraPerspective * getTranslationMatrix();
	glUniformMatrix4fv(_gWVP, 1, GL_TRUE, &fTemp.points[0][0]);






	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glMultiDrawElements(GL_POLYGON, count , GL_UNSIGNED_INT, (const GLvoid** )indices, _vertsInFace.size());

	//disable shader
	glUseProgram(0);

	//disable locations
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	//bind null buffers to avoid modifying them
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}



/*
*	Creates a shader from the current file an returns it
*
*	@input filename, file to read GLSL code from
*	@input sType, the type of shader to create
*
*	@return GLuint, shader that was created
*/
GLuint DObject::createShader(const char * filename, GLenum sType)
{

	GLint success;
	GLuint retu = glCreateShader(sType);

	if (retu == 0)
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

/*
*	return index of the created shader program
*/
bool DObject::createShaderProgram(const char* vertFile, const char* fragFile)
{
	_sp = glCreateProgram();

	//program didn't initialize properly
	if (_sp == 0)
	{
		cout << "failed to create shader program" << endl;
		return false;
	}

	_vs = createShader(vertFile, GL_VERTEX_SHADER);
	glAttachShader(_sp, _vs);

	_fs = createShader(fragFile, GL_FRAGMENT_SHADER);
	glAttachShader(_sp, _fs);

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








void DObject::setLightColor(Vector3f color)
{
	_lc = color;
}

void DObject::setLightAmbient(float intensity)
{
	_a2 = intensity;
}

void DObject::setLightDiffuse(float intensity)
{
	_d2 = intensity;
}

void DObject::setLightSpecular(float intensity)
{
	_s2 = intensity;
}

void DObject::setLightPosition(Vector3f position)
{
	_lp = position;
}

void DObject::initLight(Vector3f position, float aInt, float dInt, float sInt, Vector3f color)
{
	glUniform3f(_lposition, position.x, position.y, position.z);
	glUniform1f(_specular, aInt);
	glUniform1f(_diffuse, dInt);
	glUniform1f(_specular, sInt);
	glUniform3f(_lcolor, color.x, color.y, color.z);
}