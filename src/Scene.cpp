#include"Scene.h"

Scene::Scene() : camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f)){
        
        Model* mesh = new Model();
        mesh->setModel("resources\\bunny.obj");
        mesh->setTexture("resources\\UVMap.png");

        models.push_back(mesh);
    }

Model* Scene::getSelectedModel(){
    if (selected_model == -1 || models.size() > selected_model){
        return nullptr;
    }
    return models[selected_model];
}


void Scene::handleInputs(Renderer& renderer){
    camera.processInputs(renderer.getWindow());
    // Updates and exports the camera matrix to the Vertex Shader
    camera.doMatrix(45.0f, 0.1f, 100.0f, renderer.getShader(), "camMatrix");
}


void Scene::render(){
    for(Model* model : models){
        model->render();
    }
}