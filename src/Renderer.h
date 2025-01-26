#pragma once
#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"
#include "shaderClass.h"
#include<glm/glm.hpp>

#include<glad/glad.h>
#include<GLFW/glfw3.h>
class Renderer{
protected:
    GLFWwindow* window;
	Shader main_shader;
    glm::vec4 background_color;
public:
    Renderer(int width, int height);
    GLFWwindow* getWindow() {return window;};
    void clearFrame();
};