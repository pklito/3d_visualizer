#pragma once
#include "util.h"
#include <glm/glm.hpp>
#include <vector>

static class DH {
public:
    static glm::mat4 createTransformation(float a, float alpha, float d, float theta);
    static glm::mat4 forwardKinematics(const std::vector<glm::vec4>& dh_params);
};