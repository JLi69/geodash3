#include "level.h"
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include "../File/OpenFile.h"
#include <algorithm>

Geodash3::Level Geodash3::LoadLevel(std::string levelFilePath)
{
	bool success;
	std::ifstream levelFile = Geodash3::OpenFile(levelFilePath, success); //Open the file that contains the level data

	Geodash3::Level loaded; //Level to be loaded
	loaded.levelEnd = -32.0f;

	//Read the level file
	//levels are 4 wide, 80 long, and 12 high
	std::string line;
	int currentLine = 0,
		end = 0;
	//Read the file
	while(std::getline(levelFile, line))
	{
		//Ignore empty lines
		if(line.size() == 0)
			continue;
		if((int)line[0] == 13)
			continue;

		end = line.size() > end ? line.size() : end;

		//Read the line
		for(int i = 0; i < line.size(); i++)
		{
			switch(line[i])
			{//Block 1
			case '#':
				loaded.blocks.push_back(Geodash3::Block(glm::vec3(floorf((float)currentLine / 12.0f) * 0.5f - 0.75f,
																  -1.75f + 0.5f * (float)(11 - currentLine % 12),
																  -(float)i * 0.5f - 32.0f)));
				break;
			//Block 2
			case '$':
				loaded.blocks.push_back(Geodash3::Block(glm::vec3(floorf((float)currentLine / 12.0f) * 0.5f - 0.75f,
																  -1.75f + 0.5f * (float)(11 - currentLine % 12),
																  -(float)i * 0.5f - 32.0f)));
				loaded.blocks.at(loaded.blocks.size() - 1).blockType = 1;	
				break;
			//Block 3
			case '%':
				loaded.blocks.push_back(Geodash3::Block(glm::vec3(floorf((float)currentLine / 12.0f) * 0.5f - 0.75f,
																  -1.75f + 0.5f * (float)(11 - currentLine % 12),
																  -(float)i * 0.5f - 32.0f)));
				loaded.blocks.at(loaded.blocks.size() - 1).blockType = 2;	
				break;
			//Spike
			case '^':
				loaded.spikes.push_back(Geodash3::Spike(glm::vec3(glm::floor((float)currentLine / 12.0f) * 0.5f - 0.75f,
														-1.75f + 0.5f * (float)(11 - currentLine % 12),
														-(float)i * 0.5f - 32.0f)));
				break;
			//Bouncy block
			case '&':
				loaded.blocks.push_back(Geodash3::Block(glm::vec3(floorf((float)currentLine / 12.0f) * 0.5f - 0.75f,
																  -1.75f + 0.5f * (float)(11 - currentLine % 12),
																  -(float)i * 0.5f - 32.0f)));
				loaded.blocks.at(loaded.blocks.size() - 1).blockType = 3;	
				break;
			//Nothing
			default:
				break;
			}
		}

		currentLine++;
	}

	loaded.levelEnd -= (float)end * 0.5f;
	loaded.levelLength = -loaded.levelEnd - 1.0f;

	//Close the file
	levelFile.close();

	//Sort the loaded level's blocks based on block type
	std::sort(loaded.blocks.begin(), loaded.blocks.end(),
			[](Block a, Block b)
			{
				return a.blockType > b.blockType;	
			});

	return loaded;
}
