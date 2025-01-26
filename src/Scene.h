#pragma once
#include"Mesh.h"
#include<vector>
#include<glm/glm.hpp>
#include"Renderer.h"
#include"Camera.h"
#include"Mesh.h"

class Scene{
    protected:
std::vector<Model*> models;
int selected_model;
Camera camera;

    public:
    Scene();

    Model* getSelectedModel();

    void handleInputs(Renderer& renderer);
    void render();

};