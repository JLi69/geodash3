#include "Engine.h"
#include <chrono>
#include <iostream>

//Main loop
void Geodash3::Engine::Run()
{	
	//Music	
	Geodash3::MusicBuffer* m_music = new Geodash3::MusicBuffer("res/sounds/music1.wav");	
	//Set up the sounds					
	m_soundIds["boing"] = m_sounds->AddSoundEffect("res/sounds/bounce.wav");
	m_soundIds["explode"] = m_sounds->AddSoundEffect("res/sounds/explosion.wav");		
	m_soundIds["click"] = m_sounds->AddSoundEffect("res/sounds/click.wav");	

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

	//Set the spike texture so that we reduce OpenGL calls in the main cloop	
	GL_CALL(this->m_spike.ActivateTexture(GL_TEXTURE1));

	m_music->Play();

	float timePassedLastOutput = 0.0f;
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
		if(timePassedLastOutput >= 1.0f)
		{
			std::cout << "Frames Per Second: " << floorf(1.0f / this->m_secondsToDrawFrame) << '\n';
			timePassedLastOutput = 0.0f;	
		}	
		//End of frame
		std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
		//Calculate number of seconds to draw the frame
		std::chrono::duration<float> timePassed = end - begin;
		this->m_secondsToDrawFrame = timePassed.count();
		timePassedLastOutput += this->m_secondsToDrawFrame;
		//Calculate the average FPS
		frameCount++;
		if(frameCount != 1.0f)
			avgFPS *= ((frameCount - 1.0f) / (frameCount));
		avgFPS += (1.0f / this->m_secondsToDrawFrame) / frameCount;
		
		m_music->UpdateBufferStream();	
		if(!m_music->isPlaying())
		{
			delete m_music;	
			m_music = new Geodash3::MusicBuffer("res/sounds/music1.wav");	
			m_music->Play();		
		}	
	}
	
	//Output the average FPS in the game
	std::cout << "-----------------------------------\n";
	std::cout << "Average Frames Per Second: " << avgFPS << '\n';
	
	delete m_music;		

	glfwTerminate();
}
