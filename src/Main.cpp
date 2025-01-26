//------------------------------

#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"shaderClass.h"
#include"Mesh.h"
#include"Camera.h"
#include"GUI.h"
#include"Renderer.h"

#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"

const unsigned int width = 800;
const unsigned int height = 800;

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

	// Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
	Renderer renderer = Renderer(width, height);
	// Generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("resources/default.vert", "resources/default.frag");


	Model* mesh = new Model();
	mesh->setModel("meshes\\bunny.obj");
	mesh->setTexture("meshes\\UVMap.png");

	Texture brickTex("resources\\brick.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	brickTex.texUnit(shaderProgram, "tex0", 0);

	initGUI(renderer.getWindow());

	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// Creates camera object
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	// Main while loop
	while (!glfwWindowShouldClose(renderer.getWindow()))
	{
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		GUIParameters parameters;
		createGUI(parameters);

		if(parameters.path_name.length() > 0){
			mesh->setModel(parameters.path_name);
		}

		// Tell OpenGL which Shader Program we want to use
		shaderProgram.activate();

		// Handles camera inputs
		camera.processInputs(renderer.getWindow());
		// Updates and exports the camera matrix to the Vertex Shader
		camera.doMatrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");

		mesh->render();
		
		renderGUI();
		
		glfwSwapBuffers(renderer.getWindow());
		// Take care of all GLFW events
		glfwPollEvents();
	}

	// Deletes all ImGUI instances
	deleteGUI();

	// Delete all the objects we've created
	mesh->destroy();
	delete mesh;

	// Delete window before ending the program
	glfwDestroyWindow(renderer.getWindow());
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}