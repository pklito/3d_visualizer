#include"Scene.h"
#include "graphics/MeshEx.h"
#include "graphics/Spider.h"

Scene::Scene() : camera(){
        Model* grid1 = new Primitive(PRIM_GRID);
        Model* grid2 = new Primitive(PRIM_GRID);
        grid2->setScale(glm::vec3(10,10,10));

        const_models.push_back(grid1);
        const_models.push_back(grid2);

        //Define the camera intrinsics I want.
        camera.doPerspective(45.0f, 0.1f, 100.0f);
        camera.lookAt(glm::vec3(2,1,2), glm::vec3(0,0,0), glm::vec3(0,1,0));
    }

void Scene::setupDemo(){
    GroupModel* group = demoAxis(0.02, 0.95, 0.05, 1.);
    models.push_back(group);
    models.push_back(LegThigh());
    Model* calf = LegCalf();
    calf->setPosition(glm::vec3(0,0,0.2));
    models.push_back(calf);
    /*
    models.push_back(Leg());
    models.push_back(Leg());
    models.push_back(Leg());
    models.push_back(Leg());
    models.push_back(Leg());
    models.push_back(Leg());

    //stairs
    ConfigableGroupModel* stair = stairModel();
    stair->setPosition(glm::vec3(-0.4,0.0,0));
    models.push_back(stair);
    stair = stairModel();
    stair->setPosition(glm::vec3(-0.4,0.0,0));
    stair->setFloatParam("stepNumber", -1);
    models.push_back(stair);
    stair = stairModel();
    stair->setPosition(glm::vec3(-0.4,0.0,0));
    stair->setFloatParam("stepNumber", 1);
    models.push_back(stair);
    */
}

void Scene::addModel(Model* model){
    models.push_back(model);
    selected_model = models.size()-1;
}

void Scene::deleteSelectedModel(){
    if (selected_model == -1 || models.size() <= selected_model){
        return;
    }
    models[selected_model]->destroy();
    delete models[selected_model];
    models.erase(models.begin() + selected_model);
    selected_model -= 1;
    selected_model = max(0, selected_model);
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

    if (selected_model >= 0 && selected_model < models.size() && highlight_selected_model){
        models[selected_model]->render(renderer, &Renderer::renderHighlight);
    }
}

void Scene::buildModelEditGUI(){
    // model list
    if (!models.empty()) {
        std::vector<std::string> model_names;
        for(int i = 0; i < models.size(); i++){
            model_names.push_back(models[i]->getName());
        }
        std::vector<const char*> model_name_ptrs;
        for(const auto& name : model_names){
            model_name_ptrs.push_back(name.c_str());
        }
        ImGui::ListBox("Models", &selected_model, model_name_ptrs.data(), models.size(), 4);
    } else {
        ImGui::Text("No models available");
        return;
    }
    // Forward backwards buttons
    if (ImGui::Button("<")){
        cycleSelectedModel(-1);
    }
    ImGui::SameLine();
    if (ImGui::Button(">")){
        cycleSelectedModel(1);
        
    }

    if(getSelectedModel() == nullptr){
        ImGui::Text("No selected model");
        return;
    }

    ImGui::SameLine();
    ImGui::Text("Selected Model: %d/%d", selected_model+1, models.size());
    ImGui::SameLine();
    if (ImGui::Button("Delete")){
        deleteSelectedModel();
        if(getSelectedModel() == nullptr)
            return;
    }
    ImGui::SameLine();
    ImGui::Text(getSelectedModel()->getName().c_str());
    ImGui::SameLine();
    static bool renaming = false;
    if(ImGui::Button("Rename")){
        renaming = true;
        ImGui::OpenPopup("Rename");

    }
    if (ImGui::BeginPopup("Rename"))
    {
        ImGui::Text("Enter new name");
        ImGui::SetNextItemWidth(ImGui::GetFontSize() * 30);
        std::string name = getSelectedModel()->getName();
                
        char buffer[256];
        strncpy_s(buffer, name.c_str(), sizeof(buffer));
        buffer[sizeof(buffer) - 1] = 0;
        if (ImGui::InputText("###Name", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
            ImGui::CloseCurrentPopup();
            getSelectedModel()->setName(std::string(buffer));
        }
        if(renaming)
            ImGui::SetKeyboardFocusHere(-1);
        ImGui::EndPopup();
    }
    
    ImGui::Separator();
    
    getSelectedModel()->buildGUI();

    GroupModel* group = dynamic_cast<GroupModel*>(getSelectedModel());
    if(group != nullptr){
        ImGui::SeparatorText("Children");
        group->buildChildrenDropdownGUI();
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