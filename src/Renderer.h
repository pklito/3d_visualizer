#pragma once
#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"
#include "shaderClass.h"
#include<glm/glm.hpp>

#include"Camera.h"
#include"VAO.h"
#include"Texture.h"


#include<glad/glad.h>
#include<GLFW/glfw3.h>
class Renderer{
protected:
    GLFWwindow* window;
	Shader tex_shader;
	Shader no_tex_shader;

    glm::vec4 background_color;
    int width, height;
public:
    Renderer(GLFWwindow* window, int width, int height);
    GLFWwindow* getWindow() {return window;};
    glm::vec2 getWindowShape() {return glm::vec2(width, height);};
    //Shader& getMainShader() {return tex_shader;};

    void renderModel(GLuint render_mode, GLsizeiptr indices_count, VAO* vao, Texture* texture, const glm::vec4& color = glm::vec4(1), const glm::mat4& model_transform = glm::mat4(1), const glm::mat4& normal_transform = glm::mat4(1));

    void updateCamera(Camera& camera);
    void updateCamera(const glm::mat4& projection, const glm::mat4& view);
    void clearFrame();
};