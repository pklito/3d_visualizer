//------------------------------

#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"graphics/shaderClass.h"
#include"graphics/Mesh.h"
#include"graphics/Camera.h"
#include"graphics/GUI.h"
#include"graphics/Renderer.h"
#include"graphics/Scene.h"

#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"
#include <stdexcept>
#include "Logger.h"

const unsigned int width = 800;
const unsigned int height = 800;

// GLFW needs to take a function to call, and renderer isn't static. so i'm using a static pointer as a trick.
static Renderer* _renderer_global_ptr = nullptr;

int main()
{
	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, "Visualizer", NULL, NULL);
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

	// Create a GLFWwindow object of 800 by 800 pixels
	Renderer renderer = Renderer(window, width, height);
	_renderer_global_ptr = &renderer;

	auto framebuffer_size_callback = [](GLFWwindow* window, int width, int height){
		_renderer_global_ptr->windowResizeCallBack(window, width, height);
	};
	
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	// Generates Shader object using shaders default.vert and default.frag

	Scene scene = Scene();
	scene.setupDemo();

	//Texture brickTex("resources\\brick.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	//brickTex.texUnit(shaderProgram, "tex0", 0);

	GUI gui(renderer.getWindow(), &scene, &renderer);

	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	Logger::getInstance().log(LOG_INFO, "Starting main loop");

	// Main while loop
	while (!glfwWindowShouldClose(renderer.getWindow()))
	{
		renderer.clearFrame();

		gui.build();

		//test move around the object
		// scene.getSelectedModel()->setPosition(glm::vec3(sin(glfwGetTime()),0,0));
		// scene.getSelectedModel()->setScale(+glm::vec3(1.+cos(0.3*glfwGetTime())/4.));


		scene.handleInputs(renderer);
		// Handles camera inputs
		scene.render(renderer);
		
		gui.render();
		glUseProgram(0);
		
		glfwSwapBuffers(renderer.getWindow());
		// Take care of all GLFW events
		glfwPollEvents();
	}

	// Deletes all ImGUI instances
	gui.destroy();

	// Delete all the objects we've created
	scene.destroy();

	// Delete window before ending the program
	glfwDestroyWindow(renderer.getWindow());
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}