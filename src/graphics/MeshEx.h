#pragma once
#include "graphics/Mesh.h"
#include <map>
#include <functional>
#include "graphics/GUIfuncs.h"

#define NEW_CONFIG(type, name, value) {name, new ConfigVariable<type>((name), (value))}
#define NEW_CONFIG_HIDDEN(type, name, value) {name, new ConfigVariable<type>((name), (value), false)}

#define GET_CONFIG_VARIABLE(type, cv) (dynamic_cast<ConfigVariable<type>*>(cv) == nullptr ? type() : dynamic_cast<ConfigVariable<type>*>(cv)->value)
class ConfigVariableBase {
    public:
        std::string name;
        bool show_in_gui;
        bool inherit_from_parents;
    public:
    ConfigVariableBase(std::string name, bool show_in_gui = true, bool inherit_from_parents = false) : name(name), show_in_gui(show_in_gui), inherit_from_parents(inherit_from_parents) {};
        virtual bool buildGUI() = 0;
};

template <typename T>
class ConfigVariable : public ConfigVariableBase {
    public:
        T value;
    public:
        ConfigVariable(std::string name, T value, bool show_in_gui = true, bool inherit_from_parents = false) : ConfigVariableBase(name, show_in_gui, inherit_from_parents), value(value) {};
        bool buildGUI() override;
};

class ConfigableGroupModel : public GroupModel {
private:
    // update the models based on the parameters.
    std::function<void(std::vector<Model*>&, std::map<std::string, ConfigVariableBase*>&)> update_models;
    // parameters
    std::map<std::string, ConfigVariableBase*> params;
public:
    // Pass models, parameters, and update function
    ConfigableGroupModel(std::vector<Model*> models, std::map<std::string, ConfigVariableBase*> params, std::function<void(std::vector<Model*>&, std::map<std::string, ConfigVariableBase*>&)> update);
    ConfigableGroupModel();

    void setFloatParam(std::string name, float value);
    void setFloatParams(std::map<std::string, ConfigVariableBase*> params);
    float getFloatParam(std::string name);
    virtual void buildGUI() override;

    void updateModels();
};

ConfigableGroupModel* demoAxis(float bar_radius = 0.1, float bar_length = 0.5, float arrow_radius = 0.2, float size = 1.);
ConfigableGroupModel* stairModel(float stair_height = 0.17, float stair_length = 0.29);
ConfigableGroupModel* arrow(float bar_radius = 0.1, float bar_length = 0.5, float arrow_radius = 0.2, float size = 1.);

const std::map<std::string, ConfigableGroupModel*(*)()> getDemoConfigGroupModels() ;