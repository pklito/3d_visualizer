#pragma once
#include <glad/glad.h>
#include <string>

#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"
#include"Scene.h"
class GUI{
    protected:
        Scene* scene;

    public:
        GUI(GLFWwindow* window, Scene* scene = nullptr);
        void setScene(Scene* scene);
        void render();
        void build();
        void destroy();
};