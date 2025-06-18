#include "DH.h"
#include "MeshEx.h"
#include "Logger.h"
using namespace glm;
mat4 DH::createTransformation(float a, float alpha, float d, float theta){
    //a and alpha i-1, d and theta d. this is MODIFIED DH
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
    updateMeshes();
}

#include <glm/gtx/string_cast.hpp>

void Kinematics::createMeshes(){
    setRenderPipeline(PL_OVERRIDDEN);
    for(auto tuple : dh_params){
        addModel(demoAxis());
    }
}

void Kinematics::updateMeshes(){
    int i = 0;
    mat4 current_matrix = mat4(1);
    for(auto tuple : dh_params){
        current_matrix = current_matrix * DH::createTransformation(tuple[0], tuple[1], tuple[2], tuple[3]);
        Model* axis = this->models[i++];
        axis->setScale(vec3(0.1f,0.1f,0.1f));
        axis->setWorldTransformation(current_matrix);
        axis->setRenderPipeline(PL_MESH_SHADELESS);
        vec4 position = current_matrix * glm::vec4(0,0,0,1);
    }
}

void Kinematics::buildGUI(){
    Model::buildGUI();
	ImGui::Separator();
    
    int i = 1;
    bool changed = false;
    if(ImGui::BeginTable("dh param inputs",5, ImGuiTableFlags_Borders)){
        ImGui::TableSetupColumn(" i ", 0, 0.2);
        ImGui::TableSetupColumn(" a i-1");
        ImGui::TableSetupColumn("alpha i-1");
        ImGui::TableSetupColumn(" d { i }");
        ImGui::TableSetupColumn(" theta { i }");
        ImGui::TableHeadersRow();
        for(auto& param_vector : dh_params){
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text(std::to_string(i).c_str());
            ImGui::PushID(i);
            for(int j = 0; j < 4; j ++){
                ImGui::TableSetColumnIndex(j + 1);
                ImGui::PushID(j);
                changed |= ImGui::DragFloat("", &glm::value_ptr(param_vector)[j], 0.05);
                ImGui::PopID();
            }
            ImGui::PopID();
            ++i;
        }

        ImGui::EndTable();
    }
    if(changed)
        updateMeshes();

}