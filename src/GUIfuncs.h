#pragma once
#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"
#include"stdafx.h"

#include<glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>

struct LockFloat3State{
    bool lock;
    glm::vec3 value;
    glm::vec3 util_val;
} ;

LPCTSTR stringToLPCTSTR(const std::string& str);
std::string popupExplorer(const std::string& file_type);
bool DragFloat3Lock(const char* label, glm::vec3& value, LockFloat3State& state, float v_speed = 0.02, const float p_min = -100., const float p_max = 100., const char* format = "%.3f", ImGuiSliderFlags flags = 0);