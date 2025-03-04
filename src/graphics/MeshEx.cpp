#include "graphics/MeshEx.h"

ConfigableGroupModel::ConfigableGroupModel(std::vector<Model*> models, std::map<std::string, float> float_params, std::function<void(std::vector<Model*>&, std::map<std::string, float>&)> update)
    : GroupModel(models), update_models(update), float_params(float_params) {}

ConfigableGroupModel::ConfigableGroupModel() : GroupModel() {}

void ConfigableGroupModel::setFloatParam(std::string name, float value) {
    float_params[name] = value;
    updateModels();
}

void ConfigableGroupModel::setFloatParams(std::map<std::string, float> params) {
    for(auto pair : params) {
        float_params[pair.first] = pair.second;
    }
    updateModels();
}

float ConfigableGroupModel::getFloatParam(std::string name) {
    return float_params[name];
}

void ConfigableGroupModel::buildGUI() {
    Model::buildGUI();
    ImGui::SeparatorText("Config parameters");

    bool changed = false;
    for(auto pair : float_params) {
        if(ImGui::DragFloat(pair.first.c_str(), &float_params[pair.first], 0.01f)) {
            changed = true;
        }
    }

    if(changed) {
        updateModels();
    }
}

void ConfigableGroupModel::updateModels() {
    update_models(models, float_params);
}

//
//  EXAMPLES
//

ConfigableGroupModel* demoAxis(float bar_radius, float bar_length, float arrow_radius, float size) {
    float arrow_length = size - bar_length;
    ConfigableGroupModel* group = new ConfigableGroupModel({
        new Primitive(PRIM_CYLINDER),
        new Primitive(PRIM_CYLINDER),
        new Primitive(PRIM_CYLINDER),
        new Primitive(PRIM_CONE),
        new Primitive(PRIM_CONE),
        new Primitive(PRIM_CONE),
        new Primitive(PRIM_SPHERE)
    }, {
        {"bar_radius", bar_radius},
        {"arrow_length", arrow_length},
        {"arrow_radius", arrow_radius}
    }, [](std::vector<Model*>& models, std::map<std::string, float>& params) {
        float bar_radius = params["bar_radius"];
        float arrow_length = params["arrow_length"];
        float bar_length = 1 - arrow_length;
        float arrow_radius = params["arrow_radius"];
        // x bar
        models[0]->setPosition(glm::vec3(bar_length/2,0,0));
        models[0]->setScale(glm::vec3(bar_radius,bar_length,bar_radius));
        models[0]->setAnglesDegrees(glm::vec3(0,0,90));
        models[0]->setColor(glm::vec4(1,0,0,1));

        // x cone
        models[3]->setPosition(glm::vec3(bar_length + arrow_length/2,0,0));
        models[3]->setScale(glm::vec3(arrow_radius,arrow_length,arrow_radius));
        models[3]->setAnglesDegrees(glm::vec3(0,0,-90));
        models[3]->setColor(glm::vec4(1,0,0,1));

        // y bar
        models[1]->setPosition(glm::vec3(0,bar_length/2,0));
        models[1]->setScale(glm::vec3(bar_radius,bar_length,bar_radius));
        models[1]->setAnglesDegrees(glm::vec3(0,0,0));
        models[1]->setColor(glm::vec4(0,1,0,1));

        // y cone
        models[4]->setPosition(glm::vec3(0,bar_length + arrow_length/2,0));
        models[4]->setScale(glm::vec3(arrow_radius,arrow_length,arrow_radius));
        models[4]->setAnglesDegrees(glm::vec3(0,0,0));
        models[4]->setColor(glm::vec4(0,1,0,1));

        // z bar
        models[2]->setPosition(glm::vec3(0,0,bar_length/2));
        models[2]->setScale(glm::vec3(bar_radius,bar_length,bar_radius));
        models[2]->setAnglesDegrees(glm::vec3(0,90,0));
        models[2]->setColor(glm::vec4(0,0,1,1));

        // z cone
        models[5]->setPosition(glm::vec3(0,0,bar_length + arrow_length/2));
        models[5]->setScale(glm::vec3(arrow_radius,arrow_length,arrow_radius));
        models[5]->setAnglesDegrees(glm::vec3(0,-90,0));
        models[5]->setColor(glm::vec4(0,0,1,1));
        // ball
        models[6]->setScale(glm::vec3(bar_radius));
    });
    group->updateModels();

    return group;
}

ConfigableGroupModel* stairModel(float stair_height, float stair_length) {
    ConfigableGroupModel* group = new ConfigableGroupModel({
        new Primitive(PRIM_CUBE),
        new Primitive(PRIM_CUBE),
        new Primitive(PRIM_CUBE)
    }, {
        {"STEP_NUMBER", 0},
        {"STAIR_HEIGHT", stair_height},
        {"STAIR_LENGTH", stair_length},
        {"top_thickness", 0.02},
        {"front_thickness", 0.1},
        {"stair_indent", 0.02}
    }, [](std::vector<Model*>& models, std::map<std::string, float>& params) {
        float stair_height = params["STAIR_HEIGHT"];
        float stair_length = params["STAIR_LENGTH"];
        float top_thickness = params["top_thickness"];
        float front_thickness = params["front_thickness"];
        float stair_indent = params["stair_indent"];
        float step_number = params["STEP_NUMBER"];

        Model* stair_front = models[0];
        stair_front->setPosition(glm::vec3(-front_thickness/2 - stair_indent, (stair_height - top_thickness) / 2,0));
        stair_front->setScale(glm::vec3(front_thickness, stair_height - top_thickness, 1.));

        Model* stair_top = models[1];
        stair_top->setPosition(glm::vec3(-stair_length/2, stair_height - top_thickness/2, 0));
        stair_top->setScale(glm::vec3(stair_length, top_thickness, 1.));
        
        Model* stair_bottom = models[2];
        stair_bottom->setPosition(glm::vec3(-front_thickness/2 - stair_indent, - top_thickness/2, 0));
        stair_bottom->setScale(glm::vec3(front_thickness + stair_indent, top_thickness, 1.));
        //offset based on stair_number
        for(Model* model : models) {
            glm::vec3 pos = model->getPosition();
            pos[0] -= step_number * stair_length;
            pos[1] += step_number * stair_height;
            model->setPosition(pos);
        }

        //set colors
        for(Model* model : models) {
            model->setColor(glm::vec4(0.7,0.8,0.4,1));
        }

    });
    group->updateModels();

    return group;
}