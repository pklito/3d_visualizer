#include"Scene.h"

Scene::Scene() : camera(){
        Model* grid1 = new Primitive(PRIM_GRID);
        Model* grid2 = new Primitive(PRIM_GRID);
        grid2->setScale(glm::vec3(10,10,10));

        const_models.push_back(grid1);
        const_models.push_back(grid2);

        //Define the camera intrinsics I want.
        camera.doPerspective(45.0f, 800.0f/800.0f, 0.1f, 100.0f);
        camera.lookAt(glm::vec3(2,1,2), glm::vec3(0,0,0), glm::vec3(0,1,0));
    }

void Scene::setupDemo(){
    Model* mesh = new Model();
    mesh->setModel("resources\\fox.obj");
    mesh->setTexture("resources\\UVMap.png");
    mesh->setRenderType(GL_TRIANGLES);
    mesh->setScale(glm::vec3(1.,0.5,1.));
    mesh->setPosition(glm::vec3(0.0,0,0));

    Model* mesh_2 = new Primitive(PRIM_CONE);
    mesh_2->setPosition(glm::vec3(0,1.2,0));
    mesh_2->setScale(glm::vec3(0.3));
    mesh_2->setAnglesDegrees(glm::vec3(0,-30,0));

    GroupModel* group = new GroupModel();
    group->addModel(mesh_2);
    group->addModel(mesh);

    models.push_back(group);
}

void Scene::addModel(Model* model){
    models.push_back(model);
    selected_model = models.size()-1;
}

Model* Scene::getSelectedModel(){
    if (selected_model == -1 || models.size() <= selected_model){
        return nullptr;
    }
    return models[selected_model];
}

void Scene::cycleSelectedModel(int amount){
    if (models.size() == 0){
        return;
    }
    selected_model += amount;
    selected_model %= models.size();
    selected_model += models.size();
    selected_model %= models.size();
}

Camera* Scene::getActiveCamera(){
    return &camera;
}


void Scene::handleInputs(Renderer& renderer){
    camera.processInputs(renderer.getWindow(), renderer.getWindowShape().x, renderer.getWindowShape().y);

    //Set the uniforms of the shader program to the new camera position/orientation
    renderer.updateCamera(camera);
}


void Scene::render(Renderer& renderer){
    for(Model* model : models){
        model->render(renderer);
    }

    int i = 0;
    for(Model* model : const_models){
        if((i == 0 || i == 1) && !render_grid){
            i++;
            continue;
        }
        model->render(renderer);
    }
}

void Scene::destroy(){
    for(Model* model : models){
        model->destroy();
        delete model;
    }

    for(Model* model : const_models){
        model->destroy();
        delete model;
    }
}