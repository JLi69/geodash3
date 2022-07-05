#include "Engine.h"
#include <iostream>

void Geodash3::Engine::m_Update()
{
	//Update the player
	this->m_playerCube.Update(this->m_secondsToDrawFrame);

	//Update the level	
	if(this->m_level.levelEnd >= -1.0f)
	{
		this->m_currentLevel++; //Once the player passed a level, load another one
		//Load a new level	
		if(this->m_currentLevel < this->m_resetLevels.size())
			this->m_level = Geodash3::LoadLevel(this->m_resetLevels.at(this->m_currentLevel));	
	}	
	//Player beat the game
	if(this->m_currentLevel >= this->m_resetLevels.size())
	{
		//this->m_resetLevels.at(this->m_resetLevels.size() - 1).blocks.clear();
		//this->m_resetLevels.at(this->m_resetLevels.size() - 1).spikes.clear();
		this->m_currentLevel = this->m_resetLevels.size() - 1;
		//Player cube runs off into the sunset
		this->m_playerCube.movement.z = -20.0f;
	}

	//Update the blocks
	bool hit = false; //Store if the player collided with a block

	this->m_level.levelEnd += 5.0f * this->m_secondsToDrawFrame;

	for(auto &block : this->m_level.blocks)
	{
		if(block.position.z > 1.0f)
			continue;

		//Check for collision
		if(Geodash3::Collider::Colliding(this->m_playerCube.getCollider(), block.getCollider()))
		{
			hit = true;
			this->m_playerCube.canJump = true;			
			this->m_playerCube.movement.y = 0.0f;
			this->m_playerCube.rotation.x = 0.0f;	
			
			//Player hits the cube head on	
			if(this->m_playerCube.position.z <= block.position.z + block.dimensions.z + this->m_playerCube.dimensions.z &&
				this->m_playerCube.position.y > block.position.y - block.dimensions.y &&
				this->m_playerCube.position.y < block.position.y + block.dimensions.y)
			{
				//Reset the player and level
				this->m_playerCube = Geodash3::Player(glm::vec3(0.0f, -1.8f, -4.5f));
				this->m_level = Geodash3::LoadLevel(this->m_resetLevels.at(this->m_currentLevel));
				return;
			}	
			//Player hit bottom of cube
			if(this->m_playerCube.position.y < block.position.y)
			{
				this->m_playerCube.position.y = block.position.y - block.dimensions.y - this->m_playerCube.dimensions.y - 0.05f;
				block.Update(this->m_secondsToDrawFrame);
				continue;
			}
			//Player fell on top of the cube
			if (this->m_playerCube.falling)
			{	
				this->m_playerCube.position.y = block.position.y + this->m_playerCube.dimensions.y + block.dimensions.y;	
				//Check if it is a bouncy cube
				if(block.blockType == 3)
				{
					this->m_playerCube.bouncing = true;
					this->m_playerCube.position.y += 0.2f;	
				}		
			}

			this->m_playerCube.falling = false;
		}

		block.Update(this->m_secondsToDrawFrame);
	}
	

	//Update the spikes
	for(auto &spike : this->m_level.spikes)
	{
		if(spike.position.z > 1.0f)
			continue;

		spike.Update(this->m_secondsToDrawFrame);

		//Check for collisions
		for(int i = 0; i < 4; i++)
		{
			//Check for collision
			if(Geodash3::Collider::Colliding(this->m_playerCube.getCollider(), spike.colliders[i]))
			{
				//Reset the player and level
				this->m_playerCube = Geodash3::Player(glm::vec3(0.0f, -1.8f, -4.5f));
				this->m_level = Geodash3::LoadLevel(this->m_resetLevels.at(this->m_currentLevel));	
				return;
			}
		}	
	}	

	//Check if the player collided with a block
	if(!hit && this->m_playerCube.position.y > -1.8f)
		this->m_playerCube.falling = true;
	//If the player didn't collide with a block and isn't on the ground,
	//then that means that they cannot jump
	if(this->m_playerCube.falling)
		this->m_playerCube.canJump = false;
}
