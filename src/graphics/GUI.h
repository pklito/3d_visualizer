#pragma once
#include <glad/glad.h>
#include <string>

#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"
#include"graphics/Scene.h"
class GUI{
    protected:
        Scene* scene;
        bool show_edit_window = true;
        bool show_output_window = false;
        
        void buildMenuBar();
        void buildEditWindow();
        void buildOutputWindow();
    public:
        GUI(GLFWwindow* window, Scene* scene = nullptr);
        void setScene(Scene* scene);
        void render();
        void build();
        void destroy();

};