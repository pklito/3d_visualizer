#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "Mesh.h"
#include "MeshEx.h"
class DH {
public:
    static glm::mat4 createTransformation(float a, float alpha, float d, float theta);
    static glm::mat4 forwardKinematics(const std::vector<glm::vec4>& dh_params);
};

class Kinematics : public GroupModel{
//modified DH
protected:
    std::vector<glm::vec4> dh_params;
public:

    typedef enum{
        JOINT_AXIS = 0,
        JOINT_SPHERE = 1,
        JOINT_PRISMATIC = 2,
        JOINT_CUBE = 3
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

class SpiderLeg : public Kinematics{
public:
    SpiderLeg(float leg_direction = 0, float body_radius = 0.15f, float hip_x = 0.05f, float hip_y = -0.01f, float thigh = 0.1f, float shin = 0.1f);
    void setAngles(float hip, float leg1, float leg2);
    glm::vec3 getAngles();

    void setLegLengths(float hip_x, float hip_y, float thigh, float shin);
    void setLegLengths(const glm::vec4& lengths);
    glm::vec4 getLegLengths();
    
    void setBodySize(float body_radius);
    float getBodySize();
};

class Spider : public ConfigableGroupModel{
public:
    Spider(float body_radius = 0.15f, float hip_x = 0.05f, float hip_y = -0.01f, float thigh = 0.1f, float shin = 0.1f);
};