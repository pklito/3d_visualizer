#pragma once
#include "Mesh.h"
#include <map>
#include <functional>
#include "GUIfuncs.h"

class ConfigableGroupModel : public GroupModel {
private:
    std::function<void(std::vector<Model*>&, std::map<std::string, float>&)> update_models;
    std::map<std::string, float> float_params;
    float min_slider = -10.0f;
    float max_slider = 10.0f;
    GUIEditType gui_edit_type = GUI_SLIDER;
public:
    ConfigableGroupModel(std::vector<Model*> models, std::map<std::string, float> float_params, std::function<void(std::vector<Model*>&, std::map<std::string, float>&)> update);
    ConfigableGroupModel();

    void setFloatParam(std::string name, float value);
    void setFloatParams(std::map<std::string, float> params);
    float getFloatParam(std::string name);
    virtual void buildGUI() override;

    void setSliderRange(float min, float max) { min_slider = min; max_slider = max; }
    void setGUIEditType(GUIEditType type) { gui_edit_type = type; }

    void updateModels();
};

ConfigableGroupModel* demoAxis(float bar_radius = 0.1, float bar_length = 0.5, float arrow_radius = 0.2, float size = 1.);