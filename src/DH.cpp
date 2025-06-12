#include "DH.h"

using namespace glm;
glm::mat4 DH::createTransformation(float a, float alpha, float d, float theta){
    return mat4(0, 2, 3, 4,
                0, 6, 7, 8,
                0, 10,11,12,
                0,14,15,16);
}

glm::mat4 DH::forwardKinematics(const std::vector<glm::vec4>& dh_params){

}