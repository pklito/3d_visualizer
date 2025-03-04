#include "graphics/GUIfuncs.h"


LPCTSTR stringToLPCTSTR(const std::string& str) {
	#ifdef UNICODE
	std::wstring wstr(str.begin(), str.end());
	return wstr.c_str();
	#else
	return str.c_str();
	#endif
}
		
std::string popupExplorer(const std::string& file_type){
	OPENFILENAME ofn;       // common dialog box structure
	#ifdef UNICODE
	wchar_t szFile[260];    // buffer for file name
	#else
	char szFile[260];       // buffer for file name
	#endif

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = stringToLPCTSTR(file_type);
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Display the Open dialog box 
	if (GetOpenFileName(&ofn) == TRUE) {
		#ifdef UNICODE
			std::wstring ws(ofn.lpstrFile);
			return std::string(ws.begin(), ws.end());
		#else
			return ofn.lpstrFile;
		#endif
	}
	return "";
}

#include"imgui_internal.h"	//for ImGui::PushMultiItemsWidths
bool DragFloat3Lock(const char* label, glm::vec3& value, LockFloat3State& state, float v_speed, const float p_min, const float p_max, const char* format, ImGuiSliderFlags flags)
{

	float toggle_height = ImGui::GetFrameHeight();

    ImGuiContext& g = *ImGui::GetCurrentContext();
    int value_changed = 0;
    ImGui::BeginGroup();
    ImGui::PushID(label);
	float drag_width = glm::max(1.f, ImGui::CalcItemWidth() - ((toggle_height + g.Style.ItemInnerSpacing.x)));	//ensure its not too small.

    ImGui::PushMultiItemsWidths(3, drag_width);
    for (int i = 0; i < 3; i++)
    {
        ImGui::PushID(i);
        if (i > 0)
			ImGui::SameLine(0, g.Style.ItemInnerSpacing.x);
        if(ImGui::DragFloat("", &value[i], v_speed, p_min, p_max, format, flags))
			value_changed = 1+i;
        ImGui::PopID();
        ImGui::PopItemWidth();
		
    }
	if(value_changed == 0){
		state.util_val = value;
	}
	else if(state.lock){
		for(int i = 0; i < 3; i++){
			float denom = state.util_val[value_changed - 1];
			float nom = value[value_changed - 1];
			denom = denom == 0 ? 1 : denom;
			nom = nom == 0 ? 1 : nom;
			if(i != value_changed - 1){
				
				value[i] = state.util_val[i] * nom / denom;
			}
		}
	}

	ImGui::SameLine(0, g.Style.ItemInnerSpacing.x);
	ImGui::Checkbox("", &state.lock);
    ImGui::PopID();

    const char* label_end = ImGui::FindRenderedTextEnd(label);
    if (label != label_end)
    {
        ImGui::SameLine(0, g.Style.ItemInnerSpacing.x);
        ImGui::TextEx(label, label_end);
    }
	
    ImGui::EndGroup();
    return value_changed;
}