#include "Engine.h"

#include <iostream>

void Geodash3::Engine::m_Display()
{
	//RGB background
	static float rgb[3] = { 1.0f, 0.0f, 0.0f };
	static int index = 0;
	if(this->m_rgbEnabled)
	{
		rgb[index] -= 1.0f / 16.0f * this->m_secondsToDrawFrame;	
		rgb[(index + 1) % 3] += 1.0f / 8.0f * this->m_secondsToDrawFrame;
		//Cycle through the colors
		if(rgb[index] <= 0.0f)
		{
			rgb[index] = 0.0f;
			index++;
			index %= 3;
		}
		GL_CALL(glClearColor(rgb[0], rgb[1], rgb[2], 1.0f));
	}
	//RGB background is off
	else
	{
		GL_CALL(glClearColor(0.0f, 0.8f, 1.0f, 1.0f));
	}

	//Clear the depth buffer bit and the screen
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
	GL_CALL(glClear(GL_DEPTH_BUFFER_BIT));

	//Enable the cube model
	GL_CALL(m_cube.Enable());
	GL_CALL(glUseProgram(m_basic3D.GetId()));
	//Set the texture coordinates for the cube
	GL_CALL(this->m_cubeCoords.Enable());

	//Draw the ground
	GL_CALL(glFrontFace(GL_CW));
	GL_CALL(this->m_ground.ActivateTexture(GL_TEXTURE0));
	GL_CALL(glUniformMatrix4fv(m_basic3D.GetUniformLocation("u_PerspectiveMat"), 1, false, glm::value_ptr(m_perspectiveMat)));
	m_modelViewMat = m_rotationMatrix * m_viewMatrix * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -3.0f, -6.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1000.0f));
	GL_CALL(glUniformMatrix4fv(m_basic3D.GetUniformLocation("u_ModelViewMat"), 1, false, glm::value_ptr(m_modelViewMat)));
	//[GROUND WILL BE TEXTURED] - DELETE LATER!!!
	GL_CALL(glUniform4f(m_basic3D.GetUniformLocation("u_Color"), 0.0f, 1.0f, 0.0f, 1.0f));
	GL_CALL(glDrawArrays(GL_TRIANGLES, 0, 36));

	//Draw the player
	GL_CALL(this->m_player.ActivateTexture(GL_TEXTURE0));

	m_modelViewMat = m_rotationMatrix * 
					 m_viewMatrix * 
					 glm::translate(glm::mat4(1.0f), this->m_playerCube.position) *
					 glm::scale(glm::mat4(1.0f), this->m_playerCube.dimensions) *
					 glm::rotate(glm::mat4(1.0f), this->m_playerCube.rotation.z, glm::vec3(0.0f, 0.0f, 1.0f)) *
					 glm::rotate(glm::mat4(1.0f), this->m_playerCube.rotation.y, glm::vec3(0.0f, 1.0f, 0.0f)) *
					 glm::rotate(glm::mat4(1.0f), this->m_playerCube.rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	GL_CALL(glUniformMatrix4fv(m_basic3D.GetUniformLocation("u_ModelViewMat"), 1, false, glm::value_ptr(m_modelViewMat)));
	//[PLAYER WILL BE TEXTURED] - DELETE LATER!!!
	GL_CALL(glUniform4f(m_basic3D.GetUniformLocation("u_Color"), 1.0f, 1.0f, 0.0f, 1.0f));
	GL_CALL(glDrawArrays(GL_TRIANGLES, 0, 36));

	//Display the block
	//GL_CALL(this->m_brick.ActivateTexture(GL_TEXTURE0));
	GL_CALL(glUseProgram(this->m_shaded3D.GetId()));
	GL_CALL(glUniformMatrix4fv(this->m_shaded3D.GetUniformLocation("u_PerspectiveMat"), 1, false, glm::value_ptr(this->m_perspectiveMat)));
	for(auto block : this->m_levels.at(this->m_currentLevel).blocks)
	{
		m_modelViewMat = m_rotationMatrix * 
					 m_viewMatrix * 
					 glm::translate(glm::mat4(1.0f), block.position) *
					 glm::scale(glm::mat4(1.0f), block.dimensions) *
					 glm::rotate(glm::mat4(1.0f), block.rotation.z, glm::vec3(0.0f, 0.0f, 1.0f)) *
					 glm::rotate(glm::mat4(1.0f), block.rotation.y, glm::vec3(0.0f, 1.0f, 0.0f)) *
					 glm::rotate(glm::mat4(1.0f), block.rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		GL_CALL(glUniformMatrix4fv(m_shaded3D.GetUniformLocation("u_ModelViewMat"), 1, false, glm::value_ptr(m_modelViewMat)));	
		
		GL_CALL(glUniform4f(m_shaded3D.GetUniformLocation("u_Color"), 0.6f, 0.6f, 0.6f, 1.0f));
		GL_CALL(glDrawArrays(GL_TRIANGLES, 0, 36));
	}

	//Display the spikes
	GL_CALL(this->m_pyrCoords.Enable());
	GL_CALL(glFrontFace(GL_CCW));
	GL_CALL(this->m_pyramid.Enable());
	GL_CALL(glUseProgram(m_basicPyramid3D.GetId()));
	GL_CALL(glUniformMatrix4fv(m_basicPyramid3D.GetUniformLocation("u_PerspectiveMat"), 1, false, glm::value_ptr(this->m_perspectiveMat)));

	for(auto spike : this->m_levels.at(this->m_currentLevel).spikes)
	{
		m_modelViewMat = m_rotationMatrix *
						 m_viewMatrix *
						 glm::translate(glm::mat4(1.0f), spike.position) *
						 glm::scale(glm::mat4(1.0f), spike.dimensions) *
						 glm::rotate(glm::mat4(1.0f), spike.rotation.z, glm::vec3(0.0f, 0.0f, 1.0f)) *
						 glm::rotate(glm::mat4(1.0f), spike.rotation.y, glm::vec3(0.0f, 1.0f, 0.0f)) *
						 glm::rotate(glm::mat4(1.0f), spike.rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		GL_CALL(glUniformMatrix4fv(m_basicPyramid3D.GetUniformLocation("u_ModelViewMat"), 1, false, glm::value_ptr(m_modelViewMat)));
		GL_CALL(glUniform4f(m_basicPyramid3D.GetUniformLocation("u_Color"), 1.0f, 0.0f, 0.0f, 1.0f));
		GL_CALL(glDrawArrays(GL_TRIANGLES, 0, 18));
	}

	//Draw the progress bar
	GL_CALL(glFrontFace(GL_CW));
	GL_CALL(this->m_rect.Enable());
	GL_CALL(glUseProgram(m_progressShader.GetId()));
	GL_CALL(glUniformMatrix4fv(m_progressShader.GetUniformLocation("u_PerspectiveMat"), 1, false, glm::value_ptr(this->m_perspectiveMat)));
	GL_CALL(glUniform1f(m_progressShader.GetUniformLocation("u_percentage"), 1.0f - (-this->m_levels.at(this->m_currentLevel).levelEnd - 1.0f) / (this->m_levels.at(this->m_currentLevel).levelLength - 26.0f)));
	m_modelViewMat = m_rotationMatrix *
					 m_viewMatrix *
					 glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.057f, -0.12f)) *
					 glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.0015f, 1.0f));
	GL_CALL(glUniformMatrix4fv(m_progressShader.GetUniformLocation("u_ModelViewMat"), 1, false, glm::value_ptr(m_modelViewMat)));
	GL_CALL(glDrawArrays(GL_TRIANGLES, 0, 6));

	//GLFW stuff
	glfwSwapBuffers(m_gameWindow);
	glfwPollEvents();
}
