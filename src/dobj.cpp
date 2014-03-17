#include "dobj.h"


using namespace std;

DObject::DObject()
{
	_numVertices = 0;
}

DObject::~DObject()
{
	// deletes all vertices
	delete[] _vertices;

	//deletes all indices
	vector< pair< int*, int> >::iterator it;
	for (it = _indices.begin(); it != _indices.end(); it++)
	{
		delete[] it->first;
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










void DObject::addVertices(float * vertices, int numVertices)
{
	int totalVerts = _numVertices + numVertices;
	VertNorm * newList = new VertNorm[totalVerts];
	for (int i = 0; i < _numVertices; i++)
	{
		newList[i] = _vertices[i];
	}
	for (int i = 0; i < numVertices; i++)
	{
		newList[i + _numVertices].coords[0] = vertices[3*i];
		newList[i + _numVertices].coords[1] = vertices[3*i+1];
		newList[i + _numVertices].coords[2] = vertices[3*i+2];
	}

	delete[] _vertices;
	_vertices = newList;
	_numVertices = totalVerts;
}


/*
*	adds a new entry to the index array.
*	vertex normals should be calculated separately after all index arrays and vertiecs are added
*/
void DObject::addIndexArray(int * indices, int numIndices)
{
	//creates a new indice array and adds it to the back of the indice vector
	int * indicesToAdd = new int[numIndices];
	for (int i = 0; i < numIndices; i++)
	{
		indicesToAdd[i] = indices[i];
	}

	_indices.push_back(pair<int *, int>(indicesToAdd, numIndices));
}

/*
*	finds all normals
*	assumes usage of triangles
*	
*/
void DObject::generateNormals()
{
	//resets all normals to (0,0,0)
	for (int i = 0; i < _numVertices; i++)
	{
		_vertices[i].coords[3] = 0;
		_vertices[i].coords[4] = 0;
		_vertices[i].coords[5] = 0;
	}

	//incriments through all index arrays
	vector< pair< int*, int> >::iterator it;
	for (it = _indices.begin(); it != _indices.end(); it++)
	{
		int * temp = it->first;
		//increments through all drawn triangles and adds their normal to the vertice normal
		for (int i = 0; i < it->second-2; i+=3)
		{
			//gets the three points in the triangle
			Vector3f a(_vertices[temp[i]].coords[0], _vertices[temp[i]].coords[1], _vertices[temp[i]].coords[2]);
			Vector3f b(_vertices[temp[i+1]].coords[0], _vertices[temp[+1]].coords[1], _vertices[temp[i+1]].coords[2]);
			Vector3f c(_vertices[temp[i+2]].coords[0], _vertices[temp[i+2]].coords[1], _vertices[temp[i+2]].coords[2]);

			//gets two vectors from the points
			Vector3f ab = a - b;
			Vector3f ac = a - c;

			//finds normal of the plane
			Vector3f abcNorm = ab.cross(ac);
			abcNorm.unitize();

			//adds the normal to the points normal list
			_vertices[temp[i]].coords[3] += abcNorm.x;
			_vertices[temp[i]].coords[4] += abcNorm.y;
			_vertices[temp[i]].coords[5] += abcNorm.z;

			_vertices[temp[i+1]].coords[3] += abcNorm.x;
			_vertices[temp[i+1]].coords[4] += abcNorm.y;
			_vertices[temp[i+1]].coords[5] += abcNorm.z;

			_vertices[temp[i+2]].coords[3] += abcNorm.x;
			_vertices[temp[i+2]].coords[4] += abcNorm.y;
			_vertices[temp[i+2]].coords[5] += abcNorm.z;
		}

	}

	//normalizes all of the vertex normals
	for (int i = 0; i < _numVertices; i++)
	{
		Vector3f temp;
		temp.x = _vertices[i].coords[3];
		temp.y = _vertices[i].coords[4];
		temp.z = _vertices[i].coords[5];

		temp.unitize();


		_vertices[i].coords[3] = temp.x;
		_vertices[i].coords[4] = temp.y;
		_vertices[i].coords[5] = temp.z;
	}
}

void DObject::generateVBO()
{
	glGenBuffers(1, &_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, _numVertices * sizeof(VertNorm), _vertices, GL_STATIC_DRAW);
	cout << _numVertices * sizeof(VertNorm) << endl;
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void DObject::generateIBO()
{
	vector< pair< int*, int> >::iterator it;
	for (it = _indices.begin(); it != _indices.end(); it++)
	{
		GLuint iTemp;
		glGenBuffers(1, &iTemp);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iTemp);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, it->second * sizeof(it->first), it->first, GL_STATIC_DRAW);
		cout << it->second * sizeof(it->first) << endl;
		_IBO.push_back( pair<GLuint,int>(iTemp,it->second));
	}


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}




void DObject::draw()
{

	//bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);

	glUseProgram(_sp);

	//enable vertices
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertNorm), 0);
	//enable normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertNorm), (const GLvoid*)12);


	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//cycle through all IBOs and draw
	vector< pair<GLuint,int> >::iterator it;
	for (it = _IBO.begin(); it != _IBO.end(); it++)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, it->first);
		glDrawElements(GL_TRIANGLES, it->second , GL_UNSIGNED_INT, 0);
	}


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


	return true;
}

void DObject::printVerticesToTerminal()
{
	for (int i = 0; i < _numVertices; i++)
	{
		cout << "Vertice: ( " << _vertices[i].coords[0] << "," << _vertices[i].coords[1] << "," << _vertices[i].coords[2] << ")" << endl;
		cout << "Normal: ( " << _vertices[i].coords[3] << "," << _vertices[i].coords[4] << "," << _vertices[i].coords[5] << ")" << endl;
		cout << endl;
	}
}