#include "Engine.h"

void Geodash3::Engine::m_HandleKeyInput(GLFWwindow* win, int key, int scancode, int action, int mods)
{
	if(action == GLFW_PRESS)
	{
		switch(key)
		{
		case GLFW_KEY_A:
			this->m_playerCube.movement.x = -3.0f;
			break;
		case GLFW_KEY_D:
			this->m_playerCube.movement.x = 3.0f;
			break;
		//JUMP
		case GLFW_KEY_SPACE:
			if (this->m_playerCube.canJump)
			{
				this->m_playerCube.position.y += 0.1f;
				this->m_playerCube.movement.y = 8.0f;
			}
			break;
		}
	}
	else if(action == GLFW_RELEASE)
	{
		switch(key)
		{
		case GLFW_KEY_A:
			this->m_playerCube.movement.x = 0.0f;
			break;
		case GLFW_KEY_D:
			this->m_playerCube.movement.x = 0.0f;
			break;
		}
	}
}
