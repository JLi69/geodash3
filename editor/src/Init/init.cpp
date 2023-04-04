#include <glad/glad.h>
#include "init.h"
#include <cstdlib>
#include <SOIL2/SOIL2.h>

void Geodash3::init(GLFWwindow *&win, const char *title)
{
	if(!glfwInit())
		exit(-1);

	win = glfwCreateWindow(1200, 675, title, NULL, NULL);
	if(!win)
	{
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(win);

	GLFWimage icons[1];
	icons[0].pixels = SOIL_load_image("res/geodash3.png", &icons[0].width, &icons[0].height, 0, SOIL_LOAD_RGBA);
	glfwSetWindowIcon(win, 1, icons);
	SOIL_free_image_data(icons[0].pixels);

	//Initialize glad
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		exit(-1);
}
