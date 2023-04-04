#include "Engine.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdio>

void Geodash3::Engine::Export()
{
	bool found = false;	
	std::stringstream exportPath;
	exportPath << "res/levels" << m_path.substr(5);
	//Check to see if the level is already included	
	std::ifstream levelListIn("res/levels/level-list.txt");
	if(levelListIn.is_open())
	{
		std::string line;
		while(std::getline(levelListIn, line) && !(found = (line == exportPath.str())));
	}
	//Not found
	else if(!levelListIn.is_open())
		std::cout << "res/levels/level-list.txt not found!\n";
	levelListIn.close();

	//Add the level path to the list
	if(!found)
	{
		std::ofstream levelListOut("res/levels/level-list.txt", std::ios_base::app);
		levelListOut << exportPath.str() << '\n';
		levelListOut.close();
	}

	WritePath(exportPath.str());
}

void Geodash3::Engine::UnExport()
{
	std::vector<std::string> levels;

	std::stringstream exportPath;
	exportPath << "res/levels" << m_path.substr(5);
	//Check to see if the level is already included	
	std::ifstream levelListIn("res/levels/level-list.txt");
	if(levelListIn.is_open())
	{
		std::string line;
		while(std::getline(levelListIn, line))
			if(exportPath.str() != line)
				levels.push_back(line);
	}
	//Not found
	else if(!levelListIn.is_open())
		std::cout << "res/levels/level-list.txt not found!\n";
	levelListIn.close();

	//Remove the path from the list		
	std::ofstream levelListOut("res/levels/level-list.txt");
	for(auto levelPath : levels)
	{
		levelListOut << levelPath << '\n';
		std::cout << levelPath << '\n';	
	}	
	levelListOut.close();

	//Delete the file
	if(remove(exportPath.str().c_str()) == 0)
		std::cout << "Deleted " << exportPath.str() << '\n';
	else
		std::cout << "Did not delete " << exportPath.str() << '\n';
}
