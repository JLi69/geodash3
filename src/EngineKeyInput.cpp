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
			this->m_playerCube.isJumping = true;	
			break;
		//Toggle RGB background
		case GLFW_KEY_T:
			this->m_rgbEnabled = !this->m_rgbEnabled;
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
		//Stop jumping
		case GLFW_KEY_SPACE:
			this->m_playerCube.isJumping = false;
			break;
		}
	}
}
