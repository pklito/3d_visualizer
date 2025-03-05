#pragma once
#include<vector>
#include<glm/glm.hpp>
#include"graphics/Renderer.h"
#include"graphics/Camera.h"
#include"graphics/Mesh.h"

class Scene{
    protected:
std::vector<Model*> models;
int selected_model;
std::vector<Model*> const_models;

bool render_grid = true;
bool highlight_selected_model = true;
Camera camera;

    public:
    Scene();
    void setupDemo();

    Model* getSelectedModel();

    void handleInputs(Renderer& renderer);
    void render(Renderer& renderer);
    void destroy();

    //For GUI
    Camera* getActiveCamera();

    void deleteSelectedModel();
    void addModel(Model* model);
    void cycleSelectedModel(int amount = 1);
    friend class GUI;
};