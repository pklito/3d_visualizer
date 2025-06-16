#pragma once
#include <glm/glm.hpp>
#include <vector>

class DH {
public:
    static glm::mat4 createTransformation(float a, float alpha, float d, float theta);
    static glm::mat4 forwardKinematics(const std::vector<glm::vec4>& dh_params);
};

class Kinematics {
//modified DH
private:
    std::vector<glm::vec4> dh_params;
public:
    Kinematics(std::vector<glm::vec4> dh_params = std::vector<glm::vec4>());
    

};