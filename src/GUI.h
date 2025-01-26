#pragma once
#include <glad/glad.h>
#include <string>

#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"

typedef struct {
    bool test_toggle;
    std::string path_name;
} GUIParameters;

void renderGUI();
void createGUI();

void createGUI(GUIParameters&);

void initGUI(GLFWwindow* window);
void deleteGUI();