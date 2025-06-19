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

Spider::Spider(float body_radius, float hip_x , float hip_y , float thigh , float shin) : ConfigableGroupModel({
        new SpiderLeg(glm::radians(30.0f), body_radius, hip_x, hip_y, thigh, shin),
        new SpiderLeg(glm::radians(90.0f), body_radius, hip_x, hip_y, thigh, shin),
        new SpiderLeg(glm::radians(150.0f), body_radius, hip_x, hip_y, thigh, shin),
        new SpiderLeg(glm::radians(210.0f), body_radius, hip_x, hip_y, thigh, shin),
        new SpiderLeg(glm::radians(270.0f), body_radius, hip_x, hip_y, thigh, shin),
        new SpiderLeg(glm::radians(330.0f), body_radius, hip_x, hip_y, thigh, shin)

    }, {
        NEW_CONFIG(float, "hip", 0), // can do it this way too
        NEW_CONFIG(float, "leg1", -0.6 ),
        NEW_CONFIG(float, "leg2", 1.2)
    }, [](std::vector<Model*>& models, std::map<std::string, ConfigVariableBase*>& params) {
        for(auto model : models){
            SpiderLeg* leg = dynamic_cast<SpiderLeg*>(model);
            if(leg == nullptr)
                continue;
            leg->setAngles(GET_CONFIG_VARIABLE(float, params["hip"]), GET_CONFIG_VARIABLE(float, params["leg1"]), GET_CONFIG_VARIABLE(float, params["leg2"]));
            
        }
    }) {
        updateModels();
        setName("Spider");
    }