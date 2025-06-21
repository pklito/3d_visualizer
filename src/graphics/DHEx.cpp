#include "DH.h"
#include "MeshEx.h"
#include "Logger.h"
#include <glm/gtx/string_cast.hpp>

SpiderLeg::SpiderLeg(float leg_direction, float body_radius, float hip_x , float hip_y , float thigh , float shin) : Kinematics({{0,glm::radians(-90.0f),0,leg_direction},
                                            {body_radius,0,hip_y,glm::radians(0.0f)},
                                            {hip_x,glm::radians(-90.0f),0,-0.6},
                                            {thigh,0,0,1.3}, 
                                            {shin,0,0,0}
                                        },
                                        {
                                            Kinematics::JointVisual(Kinematics::JOINT_AXIS, 0.03),
                                            Kinematics::JointVisual(Kinematics::JOINT_AXIS, 0.03),
                                            Kinematics::JointVisual(Kinematics::JOINT_AXIS, 0.03),
                                            Kinematics::JointVisual(Kinematics::JOINT_AXIS, 0.03),
                                            Kinematics::JointVisual(Kinematics::JOINT_SPHERE, 0.03, 0, glm::vec4(0.6f,0.6f,0.6f,1.0f))
                                        })
{
}

void SpiderLeg::setAngles(float hip, float leg1, float leg2){
    this->dh_params[1][3] = hip;
    this->dh_params[2][3] = leg1;
    this->dh_params[3][3] = leg2;
    updateMeshes();
}

glm::vec3 SpiderLeg::getAngles(){
    return glm::vec3(this->dh_params[1][3] , this->dh_params[2][3] , this->dh_params[3][3] );
}

void SpiderLeg::setLegLengths(float hip_x, float hip_y, float thigh, float shin){
    this->dh_params[2][0] = hip_x;
    this->dh_params[1][2] = hip_y;
    this->dh_params[3][0] = thigh;
    this->dh_params[4][0] = shin;
    updateMeshes();
}
void SpiderLeg::setLegLengths(const glm::vec4& lengths){
    setLegLengths(lengths[0], lengths[1], lengths[2], lengths[3]);
}

glm::vec4 SpiderLeg::getLegLengths(){
    return glm::vec4(
    this->dh_params[2][0],
    this->dh_params[1][2],
    this->dh_params[3][0],
    this->dh_params[4][0]
);
}

void SpiderLeg::setBodySize(float body_radius){
    this->dh_params[1][0] = body_radius;
    updateMeshes();
}
float SpiderLeg::getBodySize(){
    return this->dh_params[1][0];
}

Spider::Spider(float body_radius, float hip_x , float hip_y , float thigh , float shin) : ConfigableGroupModel({
        new SpiderLeg(glm::radians(30.0f), body_radius, hip_x, hip_y, thigh, shin),
        new SpiderLeg(glm::radians(90.0f), body_radius, hip_x, hip_y, thigh, shin),
        new SpiderLeg(glm::radians(150.0f), body_radius, hip_x, hip_y, thigh, shin),
        new SpiderLeg(glm::radians(210.0f), body_radius, hip_x, hip_y, thigh, shin),
        new SpiderLeg(glm::radians(270.0f), body_radius, hip_x, hip_y, thigh, shin),
        new SpiderLeg(glm::radians(330.0f), body_radius, hip_x, hip_y, thigh, shin)

    }, {
        NEW_CONFIG(float, "body radius", body_radius),
        NEW_CONFIG(float, "hip_x", hip_x),
        NEW_CONFIG(float, "hip_y", hip_y),
        NEW_CONFIG(float, "thigh length", thigh),
        NEW_CONFIG(float, "shin length", shin)
    }, [](std::vector<Model*>& models, std::map<std::string, ConfigVariableBase*>& params) {
        for(auto model : models){
            SpiderLeg* leg = dynamic_cast<SpiderLeg*>(model);
            if(leg == nullptr)
                continue;
            float body_radius = GET_CONFIG_VARIABLE(float, params["body radius"]);
            float hip_x = GET_CONFIG_VARIABLE(float, params["hip_x"]);
            float hip_y = GET_CONFIG_VARIABLE(float, params["hip_y"]);
            float thigh = GET_CONFIG_VARIABLE(float, params["thigh length"]);
            float shin = GET_CONFIG_VARIABLE(float, params["shin length"]);

            leg->setBodySize(body_radius);
            leg->setLegLengths(hip_x, hip_y, thigh, shin);
        }
    }) {
        updateModels();
        setName("Spider");
    }