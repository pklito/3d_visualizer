#include "DH.h"
#include "MeshEx.h"
#include "Logger.h"
#include <glm/gtx/string_cast.hpp>

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

Kinematics::Kinematics(const std::vector<glm::vec4>& dh_params, const std::vector<JointVisual>& visuals) : GroupModel(){
    this->dh_params = dh_params;
    if(visuals.size() != dh_params.size()){
        if(visuals.size() != 0){
            Logger::getInstance().log(LOG_WARNING, "Kinematics joint visuals and DH param array sizes don't match, ignoring visual preferences. [visuals ~ dh" + std::to_string(visuals.size()) + " ~ " + std::to_string(dh_params.size()) + "]");
        }
        createVisualPreferences();
    }
    createMeshes();
    updateMeshes();
}

void Kinematics::createVisualPreferences(){
    this->joint_appearances = {};
    for(auto _ : dh_params){
        this->joint_appearances.push_back({JOINT_AXIS, 0.1f, 0.1f, glm::vec4(1,1,1,1), glm::vec4(0.3f,0.3f,0.4f,1.0f)});
    }
}

void Kinematics::createMeshes(){
    setRenderPipeline(PL_OVERRIDDEN);

    for(auto joint : joint_appearances){
        Model* model = nullptr;
        if(joint.joint_model == JOINT_AXIS){
            model = demoAxis();
        }
        else if (joint.joint_model == JOINT_SPHERE){
            model = new Primitive(PRIM_SPHERE);
        }

        if(model == nullptr){
            Logger::getInstance().log(LOG_ERROR, "Joint model type " + std::to_string(joint.joint_model) + " not implemented!");
            return;
        }

        model->setColor(joint.joint_color);
        addModel(model);
    }
}

void Kinematics::deleteMeshes(){
    GroupModel::destroy();
    models.clear();
}

void Kinematics::updateMeshes(){
    
    mat4 current_matrix = mat4(1);
    for(int i = 0; i < dh_params.size(); i ++){
        glm::vec4 tuple = dh_params[i];
        Model* axis = this->models[i];
        const JointVisual& visuals = this->joint_appearances[i];

        current_matrix = current_matrix * DH::createTransformation(tuple[0], tuple[1], tuple[2], tuple[3]);
        axis->setScale(vec3(visuals.joint_scale));
        axis->setWorldTransformation(current_matrix);
        axis->setRenderPipeline(PL_MESH_SHADELESS);
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

void Kinematics::updateVisualPreferences(){
    deleteMeshes();
    createMeshes();
    updateMeshes();
}
