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