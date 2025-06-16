#include "DH.h"

using namespace glm;
mat4 DH::createTransformation(float theta, float alpha, float d, float a){
    return transpose(mat4(
                cos(theta),             -sin(theta),                0,          a,
                sin(theta)*cos(alpha), cos(theta)*cos(alpha), -sin(alpha), -d*sin(alpha),
                sin(theta)*sin(alpha), cos(theta)*sin(alpha), cos(alpha),   d*cos(alpha),
                0,                          0,                        0,           1
            ));
}

mat4 DH::forwardKinematics(const std::vector<glm::vec4>& dh_params){
    mat4 result = mat4(1);
    for(auto tuple : dh_params){
        result = result * createTransformation(tuple[0], tuple[1], tuple[2], tuple[3]);
    }
    return result;
}