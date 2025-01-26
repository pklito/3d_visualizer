#include"Renderer.h"
#include <stdexcept>

Renderer::Renderer(int width, int height) : main_shader("resources/default.vert", "resources/default.frag"), background_color(0.07f, 0.13f, 0.17f, 1.0f)
, width(width), height(height){
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


void Renderer::clearFrame()
{
	// Specify the color of the background
	glClearColor(background_color.r, background_color.g, background_color.b, background_color.a);
	// Clean the back buffer and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
