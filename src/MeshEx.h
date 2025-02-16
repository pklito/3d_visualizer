#pragma once
#include "Mesh.h"
#include <map>
#include <functional>

class ConfigableGroupModel : public GroupModel {
private:
    std::function<void(std::vector<Model*>&, std::map<std::string, float>&)> update_models;
    std::map<std::string, float> float_params;
public:
    ConfigableGroupModel(std::vector<Model*> models, std::map<std::string, float> float_params, std::function<void(std::vector<Model*>&, std::map<std::string, float>&)> update);
    ConfigableGroupModel();

    void setFloatParam(std::string name, float value);
    void setFloatParams(std::map<std::string, float> params);
    float getFloatParam(std::string name);
    virtual void buildGUI() override;
};

ConfigableGroupModel* demoAxis(float bar_radius = 0.1, float bar_length = 0.5, float arrow_radius = 0.2, float size = 1.);