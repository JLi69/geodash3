#include "Engine.h"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>

bool Geodash3::Engine::Read(std::string path)
{
	this->m_path = path; //Set the path to be the specified path
	bool success; //Whether or not the file loaded
	this->m_level = Geodash3::LoadLevel(this->m_path, success); //Load the level
	return success;
}

void Geodash3::Engine::Write()
{
	std::vector<std::pair<glm::vec3, char>> objects[48];

	//Blocks
	for(auto block : this->m_level.blocks)
	{
		int tempX = (int)((block.position.x + 0.75f) / 0.5f),
			tempY = (int)((block.position.y + 1.75f) / 0.5f) * -1 + 11;

		//Characters to represent the different types of blocks
		char blockChars[] = { '#', '$', '%', '&' };

		objects[tempX * 12 + tempY].push_back({ block.position, blockChars[block.blockType] });		
	}

	//Spikes
	for(auto spike : this->m_level.spikes)
	{	
		int tempX = (int)((spike.position.x + 0.75f) / 0.5f),
			tempY = (int)((spike.position.y + 1.75f) / 0.5f) * -1 + 11;
		objects[tempX * 12 + tempY].push_back({ spike.position, '^' });
	}	

	//Sort the objects
	for(int i = 0; i < 48; i++)
	{
		std::sort(objects[i].begin(), objects[i].end(),
				[](std::pair<glm::vec3, char> a, std::pair<glm::vec3, char> b)
				{
					return a.first.z > b.first.z;
				});
	}

	//Open the save file
	std::ofstream saveFile(this->m_path, std::ios::trunc);

	if(!saveFile.is_open())
		std::cout << "Failed to open save file!\n";

	//Set the start of the level to be at z = 0
	float start = -9999.0f;
	for(int i = 0; i < 48; i++)
		for(auto obj : objects[i])
			start = std::max(obj.first.z, start);

	for(int i = 0; i < 48; i++)
		for(auto &obj : objects[i])
			obj.first.z -= start; 

	int currentZ = 0;

	//The lines that represent the file
	std::stringstream lines[48];

	for(int i = 0; i < 48; i++)
		lines[i] << '.';

	for(int i = 0; i < 48; i++)
	{
		for(auto obj : objects[i])
		{	
			int tempZ = (int)((-obj.first.z) / 0.5f) + 1;

			for(int j = 0; j < tempZ - currentZ - 1; j++)
				lines[i] << '.';
			lines[i] << obj.second;

			currentZ = tempZ;
		}
		currentZ = 0;
	}

	//Write to the file
	for(int i = 0; i < 48; i++)
	{
		saveFile << lines[i].str() << '\n';

		if(i % 12 == 11)
			saveFile << '\n';
	}

	saveFile.close(); //Close the save file
	
	//Write to the level path list
	if(this->m_path != this->m_levelPaths.at(0))
		return; //Don't write to the path list file if you are not making a new file
	std::ofstream levelListFile("saves/path-list.txt", std::ios::trunc);	
	if(!levelListFile.is_open())
	{
		std::cout << "Failed to open path-list.txt!\n";
		levelListFile.close();
		return;
	}
	//Output to the file	
	if(m_levelPaths.size() != 0)
	{
		for(auto path : m_levelPaths)
			levelListFile << path << '\n';
	}
	//Close the file	
	levelListFile.close();
}

void Geodash3::Engine::WritePath(std::string path)
{
	std::vector<std::pair<glm::vec3, char>> objects[48];

	//Blocks
	for(auto block : this->m_level.blocks)
	{
		int tempX = (int)((block.position.x + 0.75f) / 0.5f),
			tempY = (int)((block.position.y + 1.75f) / 0.5f) * -1 + 11;

		//Characters to represent the different types of blocks
		char blockChars[] = { '#', '$', '%', '&' };

		objects[tempX * 12 + tempY].push_back({ block.position, blockChars[block.blockType] });		
	}

	//Spikes
	for(auto spike : this->m_level.spikes)
	{	
		int tempX = (int)((spike.position.x + 0.75f) / 0.5f),
			tempY = (int)((spike.position.y + 1.75f) / 0.5f) * -1 + 11;
		objects[tempX * 12 + tempY].push_back({ spike.position, '^' });
	}	

	//Sort the objects
	for(int i = 0; i < 48; i++)
	{
		std::sort(objects[i].begin(), objects[i].end(),
				[](std::pair<glm::vec3, char> a, std::pair<glm::vec3, char> b)
				{
					return a.first.z > b.first.z;
				});
	}

	//Open the save file
	std::ofstream saveFile(path, std::ios::trunc);

	if(!saveFile.is_open())
		std::cout << "Failed to open save file!\n";

	//Set the start of the level to be at z = 0
	float start = -9999.0f;
	for(int i = 0; i < 48; i++)
		for(auto obj : objects[i])
			start = std::max(obj.first.z, start);

	for(int i = 0; i < 48; i++)
		for(auto &obj : objects[i])
			obj.first.z -= start; 

	int currentZ = 0;

	//The lines that represent the file
	std::stringstream lines[48];

	for(int i = 0; i < 48; i++)
		lines[i] << '.';

	for(int i = 0; i < 48; i++)
	{
		for(auto obj : objects[i])
		{	
			int tempZ = (int)((-obj.first.z) / 0.5f) + 1;

			for(int j = 0; j < tempZ - currentZ - 1; j++)
				lines[i] << '.';
			lines[i] << obj.second;

			currentZ = tempZ;
		}
		currentZ = 0;
	}

	//Write to the file
	for(int i = 0; i < 48; i++)
	{
		saveFile << lines[i].str() << '\n';

		if(i % 12 == 11)
			saveFile << '\n';
	}

	saveFile.close(); //Close the save file
	
	//Write to the level path list
	if(this->m_path != this->m_levelPaths.at(0))
		return; //Don't write to the path list file if you are not making a new file
	std::ofstream levelListFile("saves/path-list.txt", std::ios::trunc);	
	if(!levelListFile.is_open())
	{
		std::cout << "Failed to open path-list.txt!\n";
		levelListFile.close();
		return;
	}
	//Output to the file	
	if(m_levelPaths.size() != 0)
	{
		for(auto path : m_levelPaths)
			levelListFile << path << '\n';
	}
	//Close the file	
	levelListFile.close();
}

void Geodash3::Engine::Delete()
{
	m_levelPaths.erase(m_levelPaths.begin() + m_pathIndex);
	remove(m_path.c_str());
	
	std::ofstream levelListFile("saves/path-list.txt");
	for(int i = 1; i < m_levelPaths.size(); i++)
		levelListFile << m_levelPaths.at(i) << '\n';
	levelListFile.close();
}
