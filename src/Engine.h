#define INCLUDE_GLAD
#include <glad/glad.h>

#include "Init/init.h"
#include "GL-Utils/GetOpenGLError.h"
#include "GL-Utils/Shader.h"
#include "GL-Utils/VertexBufferObj.h"
#include "GL-Utils/Texture-Utils/TextureObj.h"
#include "GL-Utils/Texture-Utils/TextureCoords.h"
#include "Button/Button.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "GameObject/Player.h"
#include "Level/level.h"
#include "GameObject/DeathParticle.h"

#include <vector>

/*
 * RIP Code Quality
 * */

//Field of vision of the camera = 60 degrees
#define FOV 55.0f

namespace Geodash3
{
	class Engine
	{
		//Use this matrix to convert 3D coordinates into 2D
		glm::mat4 m_perspectiveMat = glm::perspective(FOV * 3.14159f / 180.0f, 1920.0f / 1080.0f, 0.1f, 1000.0f);
		//Use this matrix to translate the models
		//to the camera position
		glm::mat4 m_viewMatrix = glm::mat4(1.0f) * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.8f, 1.5f));
		//Use this matrix to rotate the models
		//to the camera rotation
		glm::mat4 m_rotationMatrix = glm::rotate(glm::mat4(1.0f), 25.0f * 3.14159f / 180.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		//Preallocated model view matrix
		glm::mat4 m_modelViewMat;

		//Game window
		GLFWwindow *m_gameWindow;
		//Buttons
		Geodash3::Button m_quitButton = Geodash3::Button(),
						 m_gotoMenuButton = Geodash3::Button(),
						 m_playButton = Geodash3::Button(),
						 m_prevButton = Geodash3::Button(),
						 m_nextButton = Geodash3::Button();

		//Vertex buffers
		VertexBufferObj m_cube = VertexBufferObj(), //Cube object
						m_pyramid = VertexBufferObj(), //Pyramid Object
						m_rect = VertexBufferObj(); //2D Rectangle
		
		//Textures
		TextureObj m_player,
				   m_ground,
				   m_blocks[4],
				   m_spike,
				   m_pauseScreen,
				   m_title;

		//Texture coordinates
		//Texture coordinates for a cube
		TextureCoords m_cubeCoords = TextureCoords(),
					  m_cubeCoordsTop = TextureCoords(),
					  m_pyrCoords = TextureCoords(),
					  m_rectCoords = TextureCoords();
		
		//Shaders
		Shader m_basic3D, //Basic cube shader
			   m_basicPyramid3D, //Basic pyramid shader
			   m_shaded3D,
			   m_progressShader, //Progress bar shader
			   m_buttonShader;

		//Game objects
		//Player object
		Geodash3::Player m_playerCube = Geodash3::Player(glm::vec3(0.0f, -1.8f, -4.5f));
		//Level
		short int m_currentLevel = 0, m_currentBlockType = -1;
		Geodash3::Level m_level; //current level
		std::vector<std::string> m_resetLevels; //When you want to reset a level,
												//set it equal to the corresponding level
												//in this vector
		//Particles for when the player dies
		std::vector<Geodash3::DeathParticle> m_deathParticles;

		//Whether the game is paused
		bool m_paused = true, m_menu = true;
		//Number of seconds to draw the frame
		float m_secondsToDrawFrame = 1.0f;

		//Handle key input
		void m_HandleKeyInput(GLFWwindow *win, int key, int scancode, int action, int mods);
		//Mouse Coordinates
		double m_mouseX = 0.0f, m_mouseY = 0.0f;
		//Handle mouse input
		void m_HandleMouseInput(GLFWwindow *win, int button, int action, int mods);

		//When the window gets resized, do this
		void m_OnWindowResize(GLFWwindow *win, int newWidth, int newHeight);
		//RGB enabled
		bool m_rgbEnabled = false;
		//Render game objects onto the screen
		void m_Display();
		//Update the game objects
		void m_Update();
	public:
		//Run the main game loop
		void Run();

		//Constructor
		Engine();
	};
}
