#pragma once
#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"
#include"stdafx.h"

#include<glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>

LPCTSTR stringToLPCTSTR(const std::string& str);
std::string popupExplorer(const std::string& file_type);
bool _DragFloat3LockAspect(const char* label, glm::vec3& value, bool& lock);