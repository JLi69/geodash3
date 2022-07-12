#include "Engine.h"

void Geodash3::Engine::m_HandleMouseInput(GLFWwindow *win, int button, int action, int mods) 
{
	if(this->m_paused && !this->m_menu && button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		if(this->m_quitButton.MouseHovering(this->m_gameWindow, this->m_mouseX, this->m_mouseY))	
			this->m_quitButton.Click();
		if(this->m_gotoMenuButton.MouseHovering(this->m_gameWindow, this->m_mouseX, this->m_mouseY))
		{	
			this->m_gotoMenuButton.Click();
			m_soundSrc.Play(m_soundIds["click"]);	
		}	
	}
	else if(this->m_menu && button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		if(this->m_playButton.MouseHovering(this->m_gameWindow, this->m_mouseX, this->m_mouseY))
		{	
			this->m_playButton.Click();
			m_soundSrc.Play(m_soundIds["click"]);	
		}	
		if(this->m_prevButton.MouseHovering(this->m_gameWindow, this->m_mouseX, this->m_mouseY))
		{	
			this->m_prevButton.Click();
			m_soundSrc.Play(m_soundIds["click"]);	
		}	
		if(this->m_nextButton.MouseHovering(this->m_gameWindow, this->m_mouseX, this->m_mouseY))
		{	
			this->m_nextButton.Click();
			m_soundSrc.Play(m_soundIds["click"]);		
		}	
	}
}
