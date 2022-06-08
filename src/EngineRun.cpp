#include "Engine.h"
#include <chrono>
#include <iostream>

//Main loop
void Geodash3::Engine::Run()
{
	//Set the background color of the window
	GL_CALL(glClearColor(0.0f, 0.8f, 1.0f, 1.0f));
	//Enable the depth test
	GL_CALL(glEnable(GL_DEPTH_TEST));
	GL_CALL(glDepthFunc(GL_LEQUAL));
	GL_CALL(glEnable(GL_CULL_FACE));
	//Transparency
	GL_CALL(glEnable(GL_BLEND));
	GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	float avgFPS = 0.0f, //Average frames per second 
		  frameCount = 0.0f; //Number of frames passed

	//Set the perspective matrix for all the shaders before the main loop
	//so that we minimize OpenGL calls within the main loop
	GL_CALL(glUseProgram(m_basicPyramid3D.GetId()));
	GL_CALL(glUniformMatrix4fv(m_basicPyramid3D.GetUniformLocation("u_PerspectiveMat"), 1, false, glm::value_ptr(this->m_perspectiveMat)));
	GL_CALL(glUseProgram(m_basic3D.GetId()));	
	GL_CALL(glUniformMatrix4fv(m_basic3D.GetUniformLocation("u_PerspectiveMat"), 1, false, glm::value_ptr(m_perspectiveMat)));
	GL_CALL(glUseProgram(m_shaded3D.GetId()));	
	GL_CALL(glUniformMatrix4fv(this->m_shaded3D.GetUniformLocation("u_PerspectiveMat"), 1, false, glm::value_ptr(this->m_perspectiveMat)));
	GL_CALL(glUseProgram(m_progressShader.GetId()));	
	GL_CALL(glUniformMatrix4fv(m_progressShader.GetUniformLocation("u_PerspectiveMat"), 1, false, glm::value_ptr(this->m_perspectiveMat)));
	GL_CALL(glUseProgram(m_buttonShader.GetId()));	
	GL_CALL(glUniformMatrix4fv(m_buttonShader.GetUniformLocation("u_PerspectiveMat"), 1, false, glm::value_ptr(this->m_perspectiveMat)));	
	//Set the spike texture so that we reduce OpenGL calls in the main cloop	
	GL_CALL(this->m_spike.ActivateTexture(GL_TEXTURE1));

	while(!glfwWindowShouldClose(m_gameWindow))
	{
		//Begin of frame
		std::chrono::time_point<std::chrono::system_clock> begin = std::chrono::system_clock::now();
	
		//Update the mouse coordinates
		if(this->m_paused)	
			glfwGetCursorPos(this->m_gameWindow, &this->m_mouseX, &this->m_mouseY);

		this->m_Display();
		//Don't update the game scene
		//if the game is paused
		if(!this->m_paused && !this->m_menu)
			this->m_Update();

		//Output frames per second
		std::cout << "Frames Per Second: " << 1.0f / this->m_secondsToDrawFrame << '\n';
		//End of frame
		std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
		//Calculate number of seconds to draw the frame
		std::chrono::duration<float> timePassed = end - begin;
		this->m_secondsToDrawFrame = timePassed.count();	
		//Calculate the average FPS
		frameCount++;
		if(frameCount != 1.0f)
			avgFPS *= ((frameCount - 1.0f) / (frameCount));
		avgFPS += (1.0f / this->m_secondsToDrawFrame) / frameCount;
	}

	//Output the average FPS in the game
	std::cout << "-----------------------------------\n";
	std::cout << "Average Frames Per Second: " << avgFPS << '\n';

	glfwTerminate();
}
