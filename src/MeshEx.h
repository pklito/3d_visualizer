#include"Mesh.h"
#include<map>
#include <functional>
class ConfigableGroupModel : public GroupModel {
private:
    //update models, gets the vector of models, and all the variables as a map.
    std::function<void(std::vector<Model*>&, std::map<std::string, float>&)> update_models;
    std::map<std::string, float> float_params;
public:
    ConfigableGroupModel(std::vector<Model*> models, std::map<std::string, float> float_params, std::function<void(std::vector<Model*>&, std::map<std::string, float>&)> update) : GroupModel(models), update_models(update), float_params(float_params) {}
    ConfigableGroupModel() : GroupModel() {}

    void setFloatParam(std::string name, float value){
        float_params[name] = value;
        update_models(models, float_params);
    }

    void setFloatParams(std::map<std::string, float> params){
        for(auto pair : params){
            float_params[pair.first] = pair.second;
        }
        update_models(models, float_params);
    }

    float getFloatParam(std::string name){
        return float_params[name];
    }

    virtual void buildGUI() override {
        Model::buildGUI();
        ImGui::Separator();
        for(auto pair : float_params){
            ImGui::SliderFloat(pair.first.c_str(), &float_params[pair.first], -10.0f, 10.0f);
        }
    }

};

ConfigableGroupModel* demoAxis(float bar_radius = 0.1, float bar_length = 0.5, float arrow_radius = 0.2, float size = 1.){
	float arrow_length = size - bar_length;
	//x
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
        {"bar_length", bar_length},
        {"arrow_radius", arrow_radius},
        {"arrow_length", arrow_length}
    }, [](std::vector<Model*>& models, std::map<std::string, float>& params){
        float bar_radius = params["bar_radius"];
        float bar_length = params["bar_length"];
        float arrow_radius = params["arrow_radius"];
        float arrow_length = params["arrow_length"];
        //x
        models[0]->setPosition(glm::vec3(bar_length/2,0,0));
        models[0]->setScale(glm::vec3(bar_radius,bar_length,bar_radius));
        models[0]->setAnglesDegrees(glm::vec3(0,0,90));
        models[0]->setColor(glm::vec4(1,0,0,1));

        models[3]->setPosition(glm::vec3(bar_length + arrow_length/2,0,0));
        models[3]->setScale(glm::vec3(arrow_radius,arrow_length,arrow_radius));
        models[3]->setAnglesDegrees(glm::vec3(0,0,-90));
        models[3]->setColor(glm::vec4(1,0,0,1));

        //y
        models[1]->setPosition(glm::vec3(0,bar_length/2,0));
        models[1]->setScale(glm::vec3(bar_radius,bar_length,bar_radius));
        models[1]->setAnglesDegrees(glm::vec3(0,0,0));
        models[1]->setColor(glm::vec4(0,1,0,1));

        models[4]->setPosition(glm::vec3(0,bar_length + arrow_length/2,0));
        models[4]->setScale(glm::vec3(arrow_radius,arrow_length,arrow_radius));
        models[4]->setAnglesDegrees(glm::vec3(0,0,0));
        models[4]->setColor(glm::vec4(0,1,0,1));

        //z
        models[2]->setPosition(glm::vec3(0,0,bar_length/2));
        models[2]->setScale(glm::vec3(bar_radius,bar_length,bar_radius));
        models[2]->setAnglesDegrees(glm::vec3(0,90,0));
        models[2]->setColor(glm::vec4(0,0,1,1));

        models[5]->setPosition(glm::vec3(0,0,bar_length + arrow_length/2));
        models[5]->setScale(glm::vec3(arrow_radius,arrow_length,arrow_radius));
        models[5]->setAnglesDegrees(glm::vec3(0,-90,0));
        models[5]->setColor(glm::vec4(0,0,1,1));

        models[6]->setScale(glm::vec3(bar_radius));
    });

	return group;
}