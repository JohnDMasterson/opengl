#ifndef READ_F
#define READ_F

//for reading shaders form text file
#include <fstream>
#include <iostream>
#include <string>

class Reader
{
	public:
		Reader();
		std::string read(const char* filename);

	private:
		std::ifstream file;

};

#endif