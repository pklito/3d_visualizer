#include "DH.h"
#include "MeshEx.h"
#include "Logger.h"
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

Kinematics::Kinematics(const std::vector<glm::vec4>& dh_params) : GroupModel(){
    this->dh_params = dh_params;
    createMeshes();
}

#include <glm/gtx/string_cast.hpp>

void Kinematics::createMeshes(){
    setRenderPipeline(PL_OVERRIDDEN);
    mat4 current_matrix = mat4(1);
    Logger::getInstance().log(LOG_DEBUG, "Creating Kinematic render:");
    for(auto tuple : dh_params){
        current_matrix = current_matrix * DH::createTransformation(tuple[0], tuple[1], tuple[2], tuple[3]);
        Model* axis = demoAxis();
        axis->setScale(glm::vec3(0.1f,0.1f,0.1f));
        axis->setRenderPipeline(PL_MESH_SHADELESS);
        vec4 position = current_matrix * glm::vec4(0,0,0,1);
        Logger::getInstance().log(LOG_DEBUG, glm::to_string(position));

        axis->setPosition(vec3(position[0], position[1], position[2]));
        addModel(axis);
    }
}