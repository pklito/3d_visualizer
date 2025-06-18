#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "Mesh.h"
class DH {
public:
    static glm::mat4 createTransformation(float a, float alpha, float d, float theta);
    static glm::mat4 forwardKinematics(const std::vector<glm::vec4>& dh_params);
};

class Kinematics : public GroupModel{
//modified DH
private:
    std::vector<glm::vec4> dh_params;
public:

    typedef enum{
        JOINT_AXIS = 0,
        JOINT_SPHERE = 1
    } JointModelType;

    class JointVisual{
    public:
        JointModelType joint_model;
        float joint_scale;
        float rod_radius;
        glm::vec4 joint_color;
        glm::vec4 rod_color;
        JointVisual(JointModelType model = JOINT_AXIS, float joint_scale = 0.1f, float rod_radius = 0.005f,glm::vec4 joint_color = glm::vec4(1.0f), glm::vec4 rod_color = glm::vec4(1.0f)) : 
        joint_model(model), joint_scale(joint_scale), rod_radius(rod_radius), joint_color(joint_color), rod_color(rod_color){};
    };
    std::vector<JointVisual> joint_appearances;

    Kinematics(const std::vector<glm::vec4>& dh_params = std::vector<glm::vec4>(), const std::vector<JointVisual>& visuals = std::vector<JointVisual>());
    void createMeshes();
    void deleteMeshes();
    void updateMeshes();
    void updateVisualPreferences();
    void createVisualPreferences();
    virtual void buildGUI() override;

};

