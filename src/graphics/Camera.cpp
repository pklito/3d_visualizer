#include"graphics/Camera.h"
#include "imgui.h"
#include<glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>

Camera::Camera()
{
	ViewMatrix = glm::lookAt(Position, Position + Orientation, Up);
	ProjectionViewMatrix = ProjectionMatrix * ViewMatrix;
}

/**
 * fovWidth is degrees, aspect ratio is width/height
 */
void Camera::doPerspective(const float FOVWidth, const float nearPlane, const float farPlane, const float stretch)
{
	ProjectionMatrix = glm::perspective(glm::radians(FOVWidth), stretch, nearPlane, farPlane);
	InvProjectionMatrix = glm::inverse(ProjectionMatrix);
	ProjectionViewMatrix = ProjectionMatrix * ViewMatrix;
}

void Camera::lookAt(glm::vec3 position, glm::vec3 target, glm::vec3 up){
	Position = position;
	Orientation = glm::normalize(target - position);
	Up = up;
	ViewMatrix = glm::lookAt(Position, Position + Orientation, Up);
	ProjectionViewMatrix = ProjectionMatrix * ViewMatrix;
}


// void Camera::processInputs(Renderer& renderer){
// 	processInputs(renderer.getWindow(), renderer.getWindowShape().x, renderer.getWindowShape().y);
// }

void Camera::processInputs(GLFWwindow* window, int width, int height)
{
	//don't fly around if user is typing
	ImGuiIO& io = ImGui::GetIO();
	if(io.WantCaptureKeyboard)
		return;

	// Handles key inputs
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		Position += speed * Orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		Position += speed * -glm::normalize(glm::cross(Orientation, Up));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		Position += speed * -Orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		Position += speed * glm::normalize(glm::cross(Orientation, Up));
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		Position += speed * Up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		Position += speed * -Up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		speed = fast_speed;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	{
		speed = slow_speed;
	}

	// Handles mouse inputs
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		// Hides mouse cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		// Prevents camera from jumping on the first click
		if (firstClick)
		{
			glfwSetCursorPos(window, (width / 2), (height / 2));
			firstClick = false;
		}

		// Stores the coordinates of the cursor
		double mouseX;
		double mouseY;
		// Fetches the coordinates of the cursor
		glfwGetCursorPos(window, &mouseX, &mouseY);

		// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
		// and then "transforms" them into degrees 
		float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
		float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

		// Calculates upcoming vertical change in the Orientation
		glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

		// Decides whether or not the next vertical Orientation is legal or not
		if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
		{
			Orientation = newOrientation;
		}

		// Rotates the Orientation left and right
		Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

		// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
		glfwSetCursorPos(window, (width / 2), (height / 2));

	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		// Unhides cursor since camera is not looking around anymore
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// Makes sure the next time the camera looks around it doesn't jump
		firstClick = true;
	}

	
	ViewMatrix = glm::lookAt(Position, Position + Orientation, Up);
	ProjectionViewMatrix = ProjectionMatrix * ViewMatrix;
}

void Camera::buildGUI() {
	ImGui::DragFloatRange2("Move Speed (normal / shift)", &slow_speed, &fast_speed, 0.001f, 0.f, 100.0f);
	ImGui::Separator();
	ImGui::DragFloat3("Position", glm::value_ptr(Position), 0.02f, -100.0f, 100.0f);
	if(ImGui::DragFloat3("Orientation", glm::value_ptr(Orientation), 0.02f, -100.0f, 100.0f)){
		Orientation = glm::normalize(Orientation);
	}
	if(ImGui::DragFloat3("Up", glm::value_ptr(Up), 0.02f, -100.0f, 100.0f)){
		Up = glm::normalize(Up);
	}
}