#pragma once
#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"

#include<glad/glad.h>
#include<GLFW/glfw3.h>
class Renderer{
protected:
    GLFWwindow* window;
public:
    Renderer(int width, int height);
    GLFWwindow* getWindow() {return window;};
};