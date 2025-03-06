#include "graphics/MeshEx.h"

ConfigableGroupModel::ConfigableGroupModel(std::vector<Model*> models, std::map<std::string, ConfigVariableBase*> params, std::function<void(std::vector<Model*>&, std::map<std::string, ConfigVariableBase*>&)> update)
    : GroupModel(models), update_models(update), params(params) {}

ConfigableGroupModel::ConfigableGroupModel() : GroupModel() {}

void ConfigableGroupModel::setFloatParam(std::string name, float value) {
    auto* param = dynamic_cast<ConfigVariable<float>*>(params[name]);
    if(param == nullptr) {
        return;
    }
    param->value = value;    
    updateModels();
}

void ConfigableGroupModel::setFloatParams(std::map<std::string, ConfigVariableBase*> params) {
    for(auto pair : params) {
        params[pair.first] = pair.second;
    }
    updateModels();
}

float ConfigableGroupModel::getFloatParam(std::string name) {
    return GET_CONFIG_VARIABLE(float, params[name]);
}

void ConfigableGroupModel::buildGUI() {
    Model::buildGUI();
    ImGui::SeparatorText("Config parameters");

    bool changed = false;
    for(auto pair : params) {
        if(pair.second->show_in_gui) {
            changed |= pair.second->buildGUI();
        }
    }

    if(changed) {
        updateModels();
    }
}

void ConfigableGroupModel::updateModels() {
    update_models(models, params);
}

//
//  ConfigVariable
//
template <>
bool ConfigVariable<float>::buildGUI(){
    return ImGui::DragFloat(name.c_str(), &value, 0.01f);}
template <>
bool ConfigVariable<int>::buildGUI(){
    return ImGui::DragInt(name.c_str(), &value, 1);}
template <>
bool ConfigVariable<bool>::buildGUI(){
    return ImGui::Checkbox(name.c_str(), &value);}
template <>
bool ConfigVariable<std::string>::buildGUI(){
    //this sucks
    char buffer[256];
    strncpy_s(buffer, value.c_str(), sizeof(buffer));
    buffer[sizeof(buffer) - 1] = 0;
    if (ImGui::InputText(name.c_str(), buffer, sizeof(buffer))) {
        value = std::string(buffer);
        return true;
    }
    return false;
}
template <>
bool ConfigVariable<glm::vec3>::buildGUI(){
    return ImGui::DragFloat3(name.c_str(), glm::value_ptr(value), 0.01f);}
template <>
bool ConfigVariable<glm::vec4>::buildGUI(){
    return ImGui::ColorEdit4(name.c_str(), glm::value_ptr(value));}
template <>
bool ConfigVariable<glm::vec2>::buildGUI(){
    return ImGui::DragFloat2(name.c_str(), glm::value_ptr(value), 0.01f);}

template <typename T>
bool ConfigVariable<T>::buildGUI(){
    return false;
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
        {"barRadius", new ConfigVariable<float>("bar radius", bar_radius)},
        {"arrowLength", new ConfigVariable<float>("arrow length", arrow_length)},
        {"arrowRadius", new ConfigVariable<float>("arrow radius", arrow_radius)}
    }, [](std::vector<Model*>& models, std::map<std::string, ConfigVariableBase*>& params) {
        float bar_radius = GET_CONFIG_VARIABLE(float, params["barRadius"]);
        float arrow_length = GET_CONFIG_VARIABLE(float, params["arrowLength"]);
        float bar_length = 1 - arrow_length;
        float arrow_radius = GET_CONFIG_VARIABLE(float, params["arrowRadius"]);
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
    group->setName("axis");

    return group;
}

ConfigableGroupModel* stairModel(float stair_height, float stair_length) {
    ConfigableGroupModel* group = new ConfigableGroupModel({
        new Primitive(PRIM_CUBE),
        new Primitive(PRIM_CUBE),
        new Primitive(PRIM_CUBE)
    }, {
        {"stepNumber", new ConfigVariable<int>("step number", 0)},  //create a pair with the key and a configVariable pointer
        NEW_CONFIG(float, "stairHeight", stair_height), // can do it this way too
        {"stairLength", new ConfigVariable<float>("stair length", stair_length)},
        {"top_thickness", new ConfigVariable<float>("top thickness", 0.02, false)},
        {"front_thickness", new ConfigVariable<float>("front thickness", 0.1, false)},
        {"stair_indent", new ConfigVariable<float>("stair indent", 0.02, false)}
    }, [](std::vector<Model*>& models, std::map<std::string, ConfigVariableBase*>& params) {
        float stair_height = GET_CONFIG_VARIABLE(float, params["stairHeight"]);
        float stair_length = GET_CONFIG_VARIABLE(float, params["stairLength"]);
        float top_thickness = GET_CONFIG_VARIABLE(float, params["top_thickness"]);
        float front_thickness = GET_CONFIG_VARIABLE(float, params["front_thickness"]);
        float stair_indent = GET_CONFIG_VARIABLE(float, params["stair_indent"]);
        int step_number = GET_CONFIG_VARIABLE(int, params["stepNumber"]);

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
    group->setName("stair");

    return group;
}

ConfigableGroupModel* arrow(float bar_radius, float bar_length, float arrow_radius, float size){
    float arrow_length = size - bar_length;
    ConfigableGroupModel* group = new ConfigableGroupModel(
        {new Primitive(PRIM_CYLINDER),
        new Primitive(PRIM_CONE)},
        {NEW_CONFIG(float, "barRadius", bar_radius),
        NEW_CONFIG(float, "arrowLength", arrow_length),
        NEW_CONFIG(float, "barLength", bar_length),
        NEW_CONFIG(float, "arrowRadius", arrow_radius),
        NEW_CONFIG(glm::vec4, "color", glm::vec4(1.))}
        , [](std::vector<Model*>& models, std::map<std::string, ConfigVariableBase*>& params){
            float bar_radius = GET_CONFIG_VARIABLE(float, params["barRadius"]);
            float arrow_length = GET_CONFIG_VARIABLE(float, params["arrowLength"]);
            float bar_length = GET_CONFIG_VARIABLE(float, params["barLength"]);
            float arrow_radius = GET_CONFIG_VARIABLE(float, params["arrowRadius"]);
            glm::vec4 color = GET_CONFIG_VARIABLE(glm::vec4, params["color"]);

            Model* bar = models[0];
            // z bar
            models[0]->setPosition(glm::vec3(0,0,bar_length/2));
            models[0]->setScale(glm::vec3(bar_radius,bar_length,bar_radius));
            models[0]->setAnglesDegrees(glm::vec3(0,-90,0));

            // z cone
            models[1]->setPosition(glm::vec3(0,0,bar_length + arrow_length/2));
            models[1]->setScale(glm::vec3(arrow_radius,arrow_length,arrow_radius));
            models[1]->setAnglesDegrees(glm::vec3(0,-90,0));
            
            //set colors
            for(Model* model : models) {
                model->setColor(color);
            }
        }
    );
    group->updateModels();
    group->setName("arrow");
    return group;
}

//
// MISC
//

// For iterating these demos
ConfigableGroupModel* _demoAxis() { return demoAxis(); }
ConfigableGroupModel* _stairModel() {   return stairModel(); }
ConfigableGroupModel* _arrow() {   return arrow(); }

const std::map<std::string, ConfigableGroupModel*(*)()> demoFuncList = {
    {"Axis", _demoAxis},
    {"Stair", _stairModel},
    {"Arrow", _arrow}
};

const std::map<std::string, ConfigableGroupModel*(*)()> getDemoConfigGroupModels() {
     return demoFuncList;
};