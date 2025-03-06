#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

#include"graphics/shaderClass.h"

class Camera
{
public:
	// Stores the main vectors of the camera
	glm::vec3 Position = glm::vec3(0,0,2);
	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::mat4 ProjectionMatrix;
	glm::mat4 ViewMatrix;

	// For projecting normals
	glm::mat4 InvProjectionMatrix;
	// In order to not recalculate every frame
	glm::mat4 ProjectionViewMatrix;

	// Prevents the camera from jumping around when first clicking left click
	bool firstClick = true;

	// Adjust the speed of the camera and it's sensitivity when looking around
	float slow_speed = 0.01f;
	float fast_speed = 0.04f;
	float speed = 0.1f;
	float sensitivity = 100.0f;

	// Camera constructor to set up initial values
	Camera();

	// Updates and exports the camera matrix to the Vertex Shader (stretch is a constant stretch of the camera, aspect ratio is handled by renderer)
	void doPerspective(const float fovx, const float zNear, const float zFar, const float stretch = 1.0);
	// Handles camera inputs
	void processInputs(GLFWwindow* window, int window_width, int window_height);

	glm::vec3 getPosition() {return Position;};
	glm::vec3 getOrientation() {return Orientation;};
	void lookAt(glm::vec3 position, glm::vec3 target, glm::vec3 up);
	
	glm::mat4 getProjectionMatrix() {return ProjectionMatrix;};
	glm::mat4 getViewMatrix() {return ProjectionMatrix;};
	glm::mat4 getProjectionViewMatrix() {return ProjectionViewMatrix;};

	void buildGUI();

};
#endif