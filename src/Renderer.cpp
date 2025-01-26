#include"Renderer.h"
#include <stdexcept>

Renderer::Renderer(int width, int height){
    this->window = glfwCreateWindow(width, height, "Visualizer", NULL, NULL);

    if (window == NULL)
	{
		glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
	}
    
	// Error check if the window fails to create
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);
	// Enable backface culling
	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, width, height);

	// Enable backface culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
}