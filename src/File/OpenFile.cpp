#include "OpenFile.h"
#include <cstdlib>
#include <sstream>
#include <iostream>

std::ifstream Geodash3::OpenFile(std::string path, bool &success)
{
	//Attempt to open the file in the directory the program was run in
	std::ifstream file(path);	

	//If you can't open it, output an error message:
	if(!file.is_open())
	{
		std::cout << "Failed to open file: " << path << '\n';
		success = false; //Failed to open file
	}
	else
		success = true; //Opened file
	
	return file;
}
