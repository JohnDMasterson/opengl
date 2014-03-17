#include "readfile.h"
using namespace std;

/*
*	default constructor for reader
*	really nothing special
*/
Reader::Reader()
{
}
/*
*	reads a file and returns text from it
*	used to read shader files
*
*	@param filename, name of file to read
*
*	@return text from file
*
*/
string Reader::read(const char* filename){
	string retu,temp;
	file.open(filename);
	while(getline(file,temp))
	{
		retu += temp;
		retu.push_back('\n');
	}
	file.close();
	return retu;
}