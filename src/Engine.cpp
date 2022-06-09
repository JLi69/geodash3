#include "Engine.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "File/OpenFile.h"

//Constructor
Geodash3::Engine::Engine()
{
	//When the window gets resized, do this
	auto onWinResizeFunc = [](GLFWwindow *win, int newWidth, int newHeight)
	{
		glViewport(0, 0, newWidth, newHeight);	
	};

	//Initialize everything
	Geodash3::init(this->m_gameWindow, "Geodash 3D", onWinResizeFunc);	

	//Set up the vertex buffers
	//Cube	
	GL_CALL(m_cube.GenBuffer());
	GL_CALL(m_cube.Data(&Geodash3::cubeVerts[0], sizeof(Geodash3::cubeVerts), 3));
	//Pyramid
	GL_CALL(m_pyramid.GenBuffer());
	GL_CALL(m_pyramid.Data(&Geodash3::pyramidVerts[0], sizeof(Geodash3::pyramidVerts), 3));
	//Rectangle
	GL_CALL(m_rect.GenBuffer());
	GL_CALL(m_rect.Data(&Geodash3::rect[0], sizeof(Geodash3::rect), 3));

	//Set up the shaders
	GL_CALL(m_basicPyramid3D.CreateShader("res/shaders/vert-3d.glsl", "res/shaders/pyramid-frag.glsl"));		
	GL_CALL(m_basic3D.CreateShader("res/shaders/vert-3d.glsl", "res/shaders/basic-frag.glsl"));			
	GL_CALL(m_shaded3D.CreateShader("res/shaders/vert-3d.glsl", "res/shaders/shaded-frag.glsl"));	
	GL_CALL(m_progressShader.CreateShader("res/shaders/vert-3d.glsl", "res/shaders/progress.glsl"));	
	GL_CALL(m_buttonShader.CreateShader("res/shaders/vert-3d.glsl", "res/shaders/button-frag.glsl"));	

	//Set up the textures
	GL_CALL(this->m_player = TextureObj("res/textures/player.png"));
	GL_CALL(this->m_ground = TextureObj("res/textures/ground.png"));
	GL_CALL(this->m_blocks[0] = TextureObj("res/textures/block1.png"));
	GL_CALL(this->m_blocks[1] = TextureObj("res/textures/block2.png"));
	GL_CALL(this->m_blocks[2] = TextureObj("res/textures/block3.png"));
	GL_CALL(this->m_spike = TextureObj("res/textures/spike.png"));
	GL_CALL(this->m_pauseScreen = TextureObj("res/textures/pause.png"));
	GL_CALL(this->m_title = TextureObj("res/textures/title.png"));

	//Set up the texture coordinates
	GL_CALL(this->m_cubeCoords.GenBuffer());
	GL_CALL(this->m_cubeCoords.Data(&Geodash3::texCubeCoords[0], sizeof(Geodash3::texCubeCoords), 2));
	GL_CALL(this->m_pyrCoords.GenBuffer());
	GL_CALL(this->m_pyrCoords.Data(&Geodash3::texPyrCoords[0], sizeof(Geodash3::texPyrCoords), 2));
	GL_CALL(this->m_rectCoords.GenBuffer());
	GL_CALL(this->m_rectCoords.Data(&Geodash3::texRectCoords[0], sizeof(Geodash3::texRectCoords), 2));

	//Set up the buttons
	//Return to main menu button
	auto goToMenuFunc = [this]()
	{
		this->m_menu = true;
		this->m_level = Geodash3::LoadLevel(this->m_resetLevels.at(this->m_currentLevel));
	};
	this->m_gotoMenuButton = Geodash3::Button(goToMenuFunc, glm::vec2(0.0f, -0.01f), glm::vec2(0.15f * 0.11f, 0.06f * 0.11f));
	GL_CALL(this->m_gotoMenuButton.SetButtonTex("res/textures/main-menubutton.png"));
	//Quit button
	this->m_quitButton = Geodash3::Button([]() { glfwTerminate(); exit(0); }, glm::vec2(0.0f, -0.025f), glm::vec2(0.15f * 0.11f, 0.06f * 0.11f));
	GL_CALL(this->m_quitButton.SetButtonTex("res/textures/quitbutton.png"));
	//Play button
	this->m_playButton = Geodash3::Button([this]() { this->m_paused = false; this->m_menu = false; this->m_playerCube = Geodash3::Player(glm::vec3(0.0f, -1.8f, -4.5f)); }, glm::vec2(0.0f, -0.04f), glm::vec2(0.15f * 0.11f, 0.06f * 0.11f));
	GL_CALL(this->m_playButton.SetButtonTex("res/textures/playbutton.png"));
	//Change level
	auto prevLev = [this]()
	{
		this->m_currentLevel--;
		this->m_currentLevel += this->m_resetLevels.size();
		this->m_currentLevel %= this->m_resetLevels.size();
		this->m_level = Geodash3::LoadLevel(this->m_resetLevels.at(this->m_currentLevel));
	};
	auto nextLev = [this]()
	{
		this->m_currentLevel++;
		this->m_currentLevel %= this->m_resetLevels.size();
		this->m_level = Geodash3::LoadLevel(this->m_resetLevels.at(this->m_currentLevel));	
	};
	//Previous level button	
	this->m_prevButton = Geodash3::Button(prevLev, glm::vec2(-0.04f, 0.0f), glm::vec2(0.03f * 0.11f, 0.06f * 0.11f));
	GL_CALL(this->m_prevButton.SetButtonTex("res/textures/prevbutton.png"));
	//Next level button	
	this->m_nextButton = Geodash3::Button(nextLev, glm::vec2(0.04f, 0.0f), glm::vec2(0.03f * 0.11f, 0.06f * 0.11f));
	GL_CALL(this->m_nextButton.SetButtonTex("res/textures/nextbutton.png"));

	//Set up key input
	glfwSetWindowUserPointer(this->m_gameWindow, this);
	auto keyInputFunc = [](GLFWwindow *win, int key, int scancode, int action, int mods)
	{
		static_cast<Engine*>(glfwGetWindowUserPointer(win))->m_HandleKeyInput(win, key, scancode, action, mods);
	};
	glfwSetKeyCallback(m_gameWindow, keyInputFunc);
	//Set up mouse button input
	auto mouseInputFunc = [](GLFWwindow *win, int button, int action, int mods)
	{
		static_cast<Engine*>(glfwGetWindowUserPointer(win))->m_HandleMouseInput(win, button, action, mods);	
	};
	glfwSetMouseButtonCallback(m_gameWindow, mouseInputFunc);

	//Load the level files into memory
	bool success;
	std::ifstream levelListFile = Geodash3::OpenFile("res/levels/level-list.txt", success);	
	//read the level list file
	std::string line;
	while(std::getline(levelListFile, line))	
		this->m_resetLevels.push_back(line);
	this->m_level = Geodash3::LoadLevel(this->m_resetLevels.at(this->m_currentLevel));
}
