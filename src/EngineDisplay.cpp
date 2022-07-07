#include "Engine.h"
#include <iostream>

void Geodash3::Engine::m_Display()
{
	//Update the view matrix
	if(this->m_playerCube.position.y > -0.4f)
		this->m_viewMatrix = glm::mat4(1.0f) * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.8f - (this->m_playerCube.position.y + 0.4f), 1.5f)); 
	else
		this->m_viewMatrix = glm::mat4(1.0f) * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.8f, 1.5f));

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
			rgb[(index + 1) % 3] = 1.0f;
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

	GL_CALL(glFrontFace(GL_CW));	

	//Enable the cube model
	GL_CALL(m_cube.Enable());
	
	GL_CALL(glUseProgram(m_basic3D.GetId()));
	GL_CALL(glUniformMatrix4fv(m_basic3D.GetUniformLocation("u_PerspectiveMat"), 1, false, glm::value_ptr(m_perspectiveMat)));	

	//Set the texture coordinates for the cube
	GL_CALL(this->m_cubeCoords.Enable());

	//Draw the ground
	GL_CALL(glUniform1i(m_basic3D.GetUniformLocation("u_outline"), true));
	//Ground texture
	GL_CALL(this->m_ground.ActivateTexture(GL_TEXTURE0));
	m_modelViewMat = glm::mat4(1.0f);
	if(this->m_menu)
		m_modelViewMat *= glm::rotate(glm::mat4(1.0f), -3.14f / 4.0f, glm::vec3(0.0f, 1.0f, 0.5f));
	m_modelViewMat *= m_rotationMatrix *
					  m_viewMatrix *
					  glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -3.0f, -6.0f) + (this->m_menu ? glm::vec3(-2.5f, 0.6f, 0.0f) : glm::vec3(0.0f, 0.0f, 0.0f))) * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 256.0f));			
	GL_CALL(glUniformMatrix4fv(m_basic3D.GetUniformLocation("u_ModelViewMat"), 1, false, glm::value_ptr(m_modelViewMat)));	
	GL_CALL(glDrawArrays(GL_TRIANGLES, 0, 36));

	//Set the texture coordinates for the cube
	GL_CALL(this->m_cubeCoordsTop.Enable());
	//Draw the player
	//Don't draw the player on the main menu screen
	if(!this->m_menu)
	{
		//Don't draw the player if dead
		if(!this->m_playerCube.dead)
		{
			//Player texture
			GL_CALL(this->m_player.ActivateTexture(GL_TEXTURE0));
			m_modelViewMat = m_rotationMatrix * 
			   				m_viewMatrix * 
			   				glm::translate(glm::mat4(1.0f), this->m_playerCube.position) *
			   				glm::scale(glm::mat4(1.0f), this->m_playerCube.dimensions) *
			   				glm::rotate(glm::mat4(1.0f), this->m_playerCube.rotation.z, glm::vec3(0.0f, 0.0f, 1.0f)) *
			   				glm::rotate(glm::mat4(1.0f), this->m_playerCube.rotation.y, glm::vec3(0.0f, 1.0f, 0.0f)) *
			   				glm::rotate(glm::mat4(1.0f), this->m_playerCube.rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
			GL_CALL(glUniformMatrix4fv(m_basic3D.GetUniformLocation("u_ModelViewMat"), 1, false, glm::value_ptr(m_modelViewMat)));	
			GL_CALL(glDrawArrays(GL_TRIANGLES, 0, 36));
		}
		//Draw death particles if the player is dead
		else
		{
			GL_CALL(this->m_player.ActivateTexture(GL_TEXTURE0));	
			for(auto deathPart : this->m_deathParticles)
			{	
				if(deathPart.distTravelled <= 3.0f)
				{
					m_modelViewMat = m_rotationMatrix *
									 m_viewMatrix *
									 glm::translate(glm::mat4(1.0f), deathPart.position) *
									 glm::scale(glm::mat4(1.0f), deathPart.dimensions);
					GL_CALL(glUniformMatrix4fv(m_basic3D.GetUniformLocation("u_ModelViewMat"), 1, false, glm::value_ptr(m_modelViewMat)));
					GL_CALL(glDrawArrays(GL_TRIANGLES, 0, 36));	
				}	
			}
		}
	}

	//Display the block	
	GL_CALL(glUseProgram(this->m_shaded3D.GetId()));	
	GL_CALL(glUniformMatrix4fv(this->m_shaded3D.GetUniformLocation("u_PerspectiveMat"), 1, false, glm::value_ptr(this->m_perspectiveMat)));	

	this->m_currentBlockType = -1;
	for(auto block : this->m_level.blocks)
	{
		if((block.position.z < -64.0f || block.position.z > 1.0f) && !this->m_menu)
			continue;
		
		//Set the texture but only change the texture
		//when we hit a new block so that we minimize
		//OpenGL operations
		if(block.blockType != this->m_currentBlockType)
		{
			this->m_currentBlockType = block.blockType;
			GL_CALL(this->m_blocks[this->m_currentBlockType].ActivateTexture(GL_TEXTURE0));
		}	

		m_modelViewMat = glm::mat4(1.0f);
		if(m_menu)
			m_modelViewMat *= glm::rotate(glm::mat4(1.0f), -3.14f / 4.0f, glm::vec3(0.0f, 1.0f, 0.5f));
		m_modelViewMat *= m_rotationMatrix * 
					 m_viewMatrix * 
					 glm::translate(glm::mat4(1.0f), block.position + (this->m_menu ? glm::vec3(-2.5f, 0.6f, 30.0f) : glm::vec3(0.0f, 0.0f, 0.0f))) *
					 glm::scale(glm::mat4(1.0f), block.dimensions);	

		GL_CALL(glUniformMatrix4fv(m_shaded3D.GetUniformLocation("u_ModelViewMat"), 1, false, glm::value_ptr(m_modelViewMat)));		
		GL_CALL(glDrawArrays(GL_TRIANGLES, 0, 36));
	}

	//Display the spikes
	GL_CALL(this->m_pyrCoords.Enable());
	GL_CALL(glFrontFace(GL_CCW));
	
	GL_CALL(this->m_pyramid.Enable());	

	GL_CALL(glUseProgram(m_basicPyramid3D.GetId()));
	GL_CALL(glUniformMatrix4fv(m_basicPyramid3D.GetUniformLocation("u_PerspectiveMat"), 1, false, glm::value_ptr(this->m_perspectiveMat)));
	
	for(auto spike : this->m_level.spikes)
	{
		if((spike.position.z < -64.0f || spike.position.z > 1.0f) && !this->m_menu)
			continue;

		m_modelViewMat = glm::mat4(1.0f);
		if(m_menu)
			m_modelViewMat *= glm::rotate(glm::mat4(1.0f), -3.14f / 4.0f, glm::vec3(0.0f, 1.0f, 0.5f));
		m_modelViewMat *= m_rotationMatrix *
						 m_viewMatrix *
						 glm::translate(glm::mat4(1.0f), spike.position + (this->m_menu ? glm::vec3(-2.5f, 0.6f, 30.0f) : glm::vec3(0.0f, 0.0f, 0.0f))) *
						 glm::scale(glm::mat4(1.0f), spike.dimensions);	

		GL_CALL(glUniformMatrix4fv(m_basicPyramid3D.GetUniformLocation("u_ModelViewMat"), 1, false, glm::value_ptr(m_modelViewMat)));	
		GL_CALL(glDrawArrays(GL_TRIANGLES, 0, 18));
	}	

	GL_CALL(glFrontFace(GL_CW));
	GL_CALL(this->m_rect.Enable());
	//Draw the progress bar
	if(!this->m_menu)
	{	
		GL_CALL(glUseProgram(m_progressShader.GetId()));
		GL_CALL(glUniformMatrix4fv(m_progressShader.GetUniformLocation("u_PerspectiveMat"), 1, false, glm::value_ptr(this->m_perspectiveMat)));
		GL_CALL(glUniform1f(m_progressShader.GetUniformLocation("u_percentage"), 1.0f - (-this->m_level.levelEnd - 1.0f) / (this->m_level.levelLength - 26.0f)));
		m_modelViewMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.059f, -0.12f)) *
		   				glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.0015f, 1.0f));
		GL_CALL(glUniformMatrix4fv(m_progressShader.GetUniformLocation("u_ModelViewMat"), 1, false, glm::value_ptr(m_modelViewMat)));
		GL_CALL(glDrawArrays(GL_TRIANGLES, 0, 6));
	}

	GL_CALL(this->m_rectCoords.Enable());
	//Draw the level select menu
	if(this->m_menu)
	{
		//Draw the title
		GL_CALL(this->m_title.ActivateTexture(GL_TEXTURE0));
		GL_CALL(glUseProgram(m_basic3D.GetId()));
		GL_CALL(glUniform1i(m_basic3D.GetUniformLocation("u_outline"), false));
		m_modelViewMat = glm::translate(glm::mat4(1.0f), glm::vec3(-0.06f, 0.05f, -0.12f)) *
						 glm::scale(glm::mat4(1.0f), glm::vec3(0.05f, 0.05f * 320.0f / 1920.0f, 0.1f));
		GL_CALL(glUniformMatrix4fv(m_basic3D.GetUniformLocation("u_ModelViewMat"), 1, false, glm::value_ptr(m_modelViewMat)));
		GL_CALL(glDrawArrays(GL_TRIANGLES, 0, 6));

		//Display the buttons
		//Play button
		GL_CALL(glUseProgram(this->m_buttonShader.GetId()));
		GL_CALL(glUniformMatrix4fv(m_buttonShader.GetUniformLocation("u_PerspectiveMat"), 1, false, glm::value_ptr(this->m_perspectiveMat)));	

		GL_CALL(this->m_playButton.ActivateButtonTex());
		this->m_playButton.SetModelViewMat(this->m_modelViewMat);
		GL_CALL(glUniformMatrix4fv(this->m_buttonShader.GetUniformLocation("u_ModelViewMat"), 1, false, glm::value_ptr(m_modelViewMat)));
		GL_CALL(glUniform1i(this->m_buttonShader.GetUniformLocation("u_hovering"), this->m_playButton.MouseHovering(this->m_gameWindow, this->m_mouseX, this->m_mouseY)));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		//Prev button		
		GL_CALL(this->m_prevButton.ActivateButtonTex());
		this->m_prevButton.SetModelViewMat(this->m_modelViewMat);
		GL_CALL(glUniformMatrix4fv(this->m_buttonShader.GetUniformLocation("u_ModelViewMat"), 1, false, glm::value_ptr(m_modelViewMat)));
		GL_CALL(glUniform1i(this->m_buttonShader.GetUniformLocation("u_hovering"), this->m_prevButton.MouseHovering(this->m_gameWindow, this->m_mouseX, this->m_mouseY)));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		//Next button	
		GL_CALL(this->m_nextButton.ActivateButtonTex());
		this->m_nextButton.SetModelViewMat(this->m_modelViewMat);
		GL_CALL(glUniformMatrix4fv(this->m_buttonShader.GetUniformLocation("u_ModelViewMat"), 1, false, glm::value_ptr(m_modelViewMat)));
		GL_CALL(glUniform1i(this->m_buttonShader.GetUniformLocation("u_hovering"), this->m_nextButton.MouseHovering(this->m_gameWindow, this->m_mouseX, this->m_mouseY)));
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	//Draw the pause screen	
	else if(this->m_paused)
	{	
		GL_CALL(this->m_pauseScreen.ActivateTexture(GL_TEXTURE0));
		GL_CALL(glUseProgram(m_basic3D.GetId()));	
		m_modelViewMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.12f)) *
						 glm::scale(glm::mat4(1.0f), glm::vec3(0.12f, 0.0675f, 0.1f));		
		GL_CALL(glUniformMatrix4fv(m_basic3D.GetUniformLocation("u_ModelViewMat"), 1, false, glm::value_ptr(m_modelViewMat)));
		GL_CALL(glDrawArrays(GL_TRIANGLES, 0, 6));
	
		//Draw the buttons
		GL_CALL(glUseProgram(m_buttonShader.GetId()));
		GL_CALL(glUniformMatrix4fv(m_buttonShader.GetUniformLocation("u_PerspectiveMat"), 1, false, glm::value_ptr(this->m_perspectiveMat)));
		//Return to main menu button
		GL_CALL(this->m_gotoMenuButton.ActivateButtonTex());
		GL_CALL(this->m_gotoMenuButton.SetModelViewMat(this->m_modelViewMat));
		GL_CALL(glUniformMatrix4fv(m_buttonShader.GetUniformLocation("u_ModelViewMat"), 1, false, glm::value_ptr(this->m_modelViewMat)));
		GL_CALL(glUniform1i(m_buttonShader.GetUniformLocation("u_hovering"), this->m_gotoMenuButton.MouseHovering(this->m_gameWindow, this->m_mouseX, this->m_mouseY)));
		GL_CALL(glDrawArrays(GL_TRIANGLES, 0, 6));
		//Quit button
		GL_CALL(this->m_quitButton.ActivateButtonTex());
		GL_CALL(this->m_quitButton.SetModelViewMat(this->m_modelViewMat));
		GL_CALL(glUniformMatrix4fv(m_buttonShader.GetUniformLocation("u_ModelViewMat"), 1, false, glm::value_ptr(this->m_modelViewMat)));
		GL_CALL(glUniform1i(m_buttonShader.GetUniformLocation("u_hovering"), this->m_quitButton.MouseHovering(this->m_gameWindow, this->m_mouseX, this->m_mouseY)));
		GL_CALL(glDrawArrays(GL_TRIANGLES, 0, 6));
	}

	//GLFW stuff
	glfwSwapBuffers(m_gameWindow);
	glfwPollEvents();
}
