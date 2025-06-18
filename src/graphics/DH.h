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
    Kinematics(const std::vector<glm::vec4>& dh_params = std::vector<glm::vec4>());
    void createMeshes();
    void updateMeshes();
    virtual void buildGUI() override;

};

