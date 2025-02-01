#include"Scene.h"

Scene::Scene() : camera(){
        
        Model* mesh = new Model();
        mesh->setModel("resources\\bunny.obj");
        mesh->setTexture("resources\\UVMap.png");
        mesh->setRenderType(GL_TRIANGLES);

        Model* mesh_2 = new Primitive(PRIM_GRID);
        Model* mesh_3 = new Primitive(PRIM_GRID);
        mesh_3->setScale(glm::vec3(10,10,10));


        //
        models.push_back(mesh);
        models.push_back(mesh_2);
        models.push_back(mesh_3);



        //Define the camera intrinsics I want.
        camera.doPerspective(45.0f, 800.0f/800.0f, 0.1f, 100.0f);
        camera.lookAt(glm::vec3(2,1,2), glm::vec3(0,0,0), glm::vec3(0,1,0));
    }

Model* Scene::getSelectedModel(){
    if (selected_model == -1 || models.size() <= selected_model){
        return nullptr;
    }
    return models[selected_model];
}


void Scene::handleInputs(Renderer& renderer){
    camera.processInputs(renderer.getWindow(), renderer.getWindowShape().x, renderer.getWindowShape().y);

    //Set the uniforms of the shader program to the new camera position/orientation
    renderer.updateCamera(camera);
}


void Scene::render(Renderer& renderer){
    renderer.getMainShader().activate();
    for(Model* model : models){
        renderer.getMainShader().setMat4("modelTransform", model->getFullTransformation());
        renderer.getMainShader().setMat4("normalTransform", model->getFullNormalTransformation());
        model->render();
    }
}

void Scene::destroy(){
    for(Model* model : models){
        model->destroy();
        delete model;
    }
}