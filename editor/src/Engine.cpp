#include "Engine.h"
#include "Init/init.h"
#include <chrono>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>

//Main loop
void Geodash3::Engine::Run()
{
	//Set the background color of the window
	GL_CALL(glClearColor(0.0f, 0.8f, 1.0f, 1.0f));
	//Enable the depth test
	GL_CALL(glEnable(GL_DEPTH_TEST));
	GL_CALL(glDepthFunc(GL_LEQUAL));
	GL_CALL(glEnable(GL_CULL_FACE));

	GL_CALL(glEnable(GL_BLEND));
	GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	float avgFPS = 0.0f, //Average frames per second 
		  frameCount = 0.0f; //Number of frames passed

	while(!glfwWindowShouldClose(m_gameWindow))
	{
		//Begin of frame
		std::chrono::time_point<std::chrono::system_clock> begin = std::chrono::system_clock::now();
		this->m_Display();
		this->m_Update();
		//Output frames per second
		//std::cout << "Frames Per Second: " << 1.0f / this->m_secondsToDrawFrame << '\n';
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

//Constructor
Geodash3::Engine::Engine()
{
	//Initialize everything
	Geodash3::init(this->m_gameWindow, "Geodash 3D Level Editor");	

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
	GL_CALL(m_basicPyramid3D.CreateShader("res/editor-shaders/vert-3d.glsl", "res/editor-shaders/pyramid-frag.glsl"));
	GL_CALL(m_basic3D.CreateShader("res/editor-shaders/vert-3d.glsl", "res/editor-shaders/basic-frag.glsl"));
	GL_CALL(m_white.CreateShader("res/editor-shaders/vert-3d.glsl", "res/editor-shaders/white.glsl"));
	GL_CALL(m_button.CreateShader("res/editor-shaders/button-vert.glsl", "res/editor-shaders/button-frag.glsl"));
	
	//Set up the textures
	GL_CALL(m_ground = TextureObj("res/editor-textures/ground.png"));
	GL_CALL(m_blocks[0] = TextureObj("res/editor-textures/block1.png"));
	GL_CALL(m_blocks[1] = TextureObj("res/editor-textures/block2.png"));	
	GL_CALL(m_blocks[2] = TextureObj("res/editor-textures/block3.png"));	
	GL_CALL(m_blocks[3] = TextureObj("res/editor-textures/block4.png"));	
	GL_CALL(m_spike = TextureObj("res/editor-textures/spike.png"));
	GL_CALL(m_crosshair = TextureObj("res/editor-textures/crosshair.png"));
	GL_CALL(m_pauseScreen = TextureObj("res/editor-textures/pause.png"));
	GL_CALL(m_helpScreen = TextureObj("res/editor-textures/geodash3-editor-credits.png"));	

	//Set up the texture coordinates
	//Cube texture coordinates	
	GL_CALL(m_cubeCoords.GenBuffer());
	GL_CALL(m_cubeCoords.Data(&Geodash3::texCubeCoords[0], sizeof(Geodash3::texCubeCoords), 2));
	//Cube texture coordinates	
	GL_CALL(m_cubeCoordsTop.GenBuffer());
	GL_CALL(m_cubeCoordsTop.Data(&Geodash3::texCubeCoordsTop[0], sizeof(Geodash3::texCubeCoordsTop), 2));	
	//Pyramid texture coordinates	
	GL_CALL(m_pyrCoords.GenBuffer());
	GL_CALL(m_pyrCoords.Data(&Geodash3::texPyrCoords[0], sizeof(Geodash3::texPyrCoords), 2));
	//Rectangle texture coordinates
	GL_CALL(m_rectCoords.GenBuffer());
	GL_CALL(m_rectCoords.Data(&Geodash3::texRectCoords[0], sizeof(Geodash3::texRectCoords), 2));

	//Set up the buttons
	//Save button
	this->m_saveButton = Geodash3::Button([this]() { this->Write(); }, glm::vec2(-0.16f, 0.0f), glm::vec2(0.15f, 0.06f));
	GL_CALL(this->m_saveButton.SetButtonTex("res/editor-textures/savebutton.png"));
	//Save & quit button	
	this->m_saveQuitButton = Geodash3::Button([this]() { this->Write(); glfwTerminate(); exit(0); }, glm::vec2(-0.16f, -0.15f), glm::vec2(0.15f, 0.06f));
	GL_CALL(this->m_saveQuitButton.SetButtonTex("res/editor-textures/save-and-quitbutton.png"));	
	//Go back to main menu
	auto returnToMenu = [this]()
	{	
		this->m_paused = false;
		//Generate a random path for the file
		if(m_pathIndex == 0)
		{
			std::stringstream randPath;	
			srand(time(NULL)); //Generate seed
			randPath << "saves/custom-";	
			for(int i = 0; i < 16; i++)
				randPath << (char)(rand() % ((int)('z' - 'a') + 1) + (int)'a');
			randPath << ".lvl";
			m_levelPaths.insert(m_levelPaths.begin(), randPath.str());
			m_pathIndex++;	
		}

		this->Write();
		this->Read(m_path);
		//Reset the camera
		this->m_camera.position = glm::vec3(0.0f, 0.0f, 0.0f);
		this->m_camera.movement = glm::vec3(0.0f, 0.0f, 0.0f);
		this->m_camera.rotation = glm::vec3(0.0f, 0.0f, 0.0f);	
		this->Menu();
		glfwSetInputMode(this->m_gameWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);	
	};
	this->m_returnToMenu = Geodash3::Button(returnToMenu, glm::vec2(0.16f, -0.15f), glm::vec2(0.15f, 0.06f));
	GL_CALL(this->m_returnToMenu.SetButtonTex("res/editor-textures/menubutton.png"));		
	//Go back to main menu and discard all unsaved changes
	auto discardChanges = [this]()
	{	
		this->m_paused = false;
		this->Read(m_path);
		//Reset the camera
		this->m_camera.position = glm::vec3(0.0f, 0.0f, 0.0f);
		this->m_camera.movement = glm::vec3(0.0f, 0.0f, 0.0f);
		this->m_camera.rotation = glm::vec3(0.0f, 0.0f, 0.0f);	
		this->Menu();
		glfwSetInputMode(this->m_gameWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);	
	};
	this->m_discardChanges = Geodash3::Button(discardChanges, glm::vec2(0.16f, 0.0f), glm::vec2(0.15f, 0.06f));
	GL_CALL(this->m_discardChanges.SetButtonTex("res/editor-textures/discardbutton.png"));	
	//Button to pull up the help screen	
	//Display the help function
	this->m_helpButton = Geodash3::Button([this](){ this->m_help = true; }, glm::vec2(0.0f, -0.45f), glm::vec2(0.15f, 0.06f));
	GL_CALL(this->m_helpButton.SetButtonTex("res/editor-textures/helpbutton.png"));
	//Export	
	this->m_exportButton = Geodash3::Button([this](){ this->Export(); }, glm::vec2(-0.16f, -0.3f), glm::vec2(0.15f, 0.06f));	
	GL_CALL(this->m_exportButton.SetButtonTex("res/editor-textures/exportbutton.png"));
	//Unexport
	this->m_unexportButton = Geodash3::Button([this](){ this->UnExport(); }, glm::vec2(0.16f, -0.3f), glm::vec2(0.15f, 0.06f));	
	GL_CALL(this->m_unexportButton.SetButtonTex("res/editor-textures/unexportbutton.png"));	

	//Menu buttons
	m_editButton = Geodash3::Button([this](){ this->m_menu = false; }, glm::vec2(-0.0f, -0.4f), glm::vec2(0.15f, 0.06f));
	GL_CALL(m_editButton.SetButtonTex("res/editor-textures/editbutton.png"));
	auto changeLevel = [this](int amount)
	{
		this->m_pathIndex += amount;
		this->m_pathIndex += this->m_levelPaths.size();
		this->m_pathIndex %= this->m_levelPaths.size();
		bool success;
		this->m_level = Geodash3::LoadLevel(this->m_levelPaths.at(this->m_pathIndex), success);
		if(!success)
			std::cout << "Failed to open file: " << this->m_levelPaths.at(this->m_pathIndex) << '\n';
		this->m_path = this->m_levelPaths.at(this->m_pathIndex);
		this->m_showConfirmDelete = false;
	};
	m_nextButton = Geodash3::Button([this, changeLevel](){ changeLevel(1); }, glm::vec2(0.3f, -0.4f), glm::vec2(0.03f, 0.06f));
	GL_CALL(m_nextButton.SetButtonTex("res/editor-textures/nextbutton.png"));	
	m_prevButton = Geodash3::Button([this, changeLevel](){ changeLevel(-1); }, glm::vec2(-0.3f, -0.4f), glm::vec2(0.03f, 0.06f));
	GL_CALL(m_prevButton.SetButtonTex("res/editor-textures/prevbutton.png"));

	//Delete
	this->m_deleteButton = Geodash3::Button([this](){ this->m_showConfirmDelete = true; }, glm::vec2(-0.0f, 0.4f), glm::vec2(0.15f, 0.06f));	
	GL_CALL(this->m_deleteButton.SetButtonTex("res/editor-textures/deletebutton.png"));
	//Confirm delete
	auto deleteLevel = [this, changeLevel]()
	{
		this->Delete();
		this->UnExport();
		changeLevel(-1);
	};
	this->m_confirmDelete = Geodash3::Button(deleteLevel, glm::vec2(0.17f, 0.0f), glm::vec2(0.15f, 0.06f));	
	GL_CALL(this->m_confirmDelete.SetButtonTex("res/editor-textures/yes.png"));	
	//Cancel delete
	this->m_cancelDelete = Geodash3::Button([this](){ this->m_showConfirmDelete = false; }, glm::vec2(-0.17f, 0.0f), glm::vec2(0.15f, 0.06f));	
	GL_CALL(this->m_cancelDelete.SetButtonTex("res/editor-textures/no.png"));

	//Set up the user input
	//Set up key input
	glfwSetWindowUserPointer(this->m_gameWindow, this);
	auto keyInputFunc = [](GLFWwindow* win, int key, int scancode, int action, int mods)
	{
		static_cast<Engine*>(glfwGetWindowUserPointer(win))->m_HandleKeyInput(win, key, scancode, action, mods);
	};
	glfwSetKeyCallback(m_gameWindow, keyInputFunc);
	//Set up mouse input
	auto mouseInputFunc = [](GLFWwindow* win, int button, int action, int mods)
	{
		static_cast<Engine*>(glfwGetWindowUserPointer(win))->m_HandleMouseInput(win, button, action, mods);
	};
	glfwSetMouseButtonCallback(this->m_gameWindow, mouseInputFunc);
	//Set up scroll input
	auto scrollInputFunc = [](GLFWwindow* win, double xoffset, double yoffset)
	{
		static_cast<Engine*>(glfwGetWindowUserPointer(win))->m_HandleScrollInput(win, xoffset, yoffset);	
	};
	glfwSetScrollCallback(this->m_gameWindow, scrollInputFunc);
	//Set up window resizing
	//When the window gets resized, do this
	auto onWinResizeFunc = [](GLFWwindow *win, int newWidth, int newHeight)
	{
		static_cast<Engine*>(glfwGetWindowUserPointer(win))->m_OnWindowResize(win, newWidth, newHeight);				
	};
	glfwSetWindowSizeCallback(this->m_gameWindow, onWinResizeFunc);

	//Set up the camera
	this->m_camera.position = glm::vec3(0.0f, 0.0f, 0.0f);
	this->m_camera.movement = glm::vec3(0.0f, 0.0f, 0.0f);
	this->m_camera.rotation = glm::vec3(0.0f, 0.0f, 0.0f);

	//Hide cursor
	//glfwSetCursorPos(this->m_gameWindow, 0.0f, 0.0f);
	//glfwGetCursorPos(this->m_gameWindow, &this->m_mouseX, &this->m_mouseY);
	//glfwSetInputMode(this->m_gameWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Generate a random path for the file	
	std::stringstream randPath;	
	srand(time(NULL)); //Generate seed
	randPath << "saves/custom-";	
	for(int i = 0; i < 16; i++)
		randPath << (char)(rand() % ((int)('z' - 'a') + 1) + (int)'a');
	randPath << ".lvl";

	this->m_path = randPath.str();
	
	//test
	std::cout << randPath.str() << '\n';

	//Get the list of paths to the file levels
	std::ifstream pathListFile("saves/path-list.txt");
	//close the file if the file does not exist
	if(!pathListFile.is_open())
	{	
		std::cout << "Failed to open path list file!\n";	
		pathListFile.close();
	}
	//Read the list
	m_levelPaths.push_back(m_path);
	std::string path;
	while(std::getline(pathListFile, path))
		m_levelPaths.push_back(path);	
	pathListFile.close();
}
