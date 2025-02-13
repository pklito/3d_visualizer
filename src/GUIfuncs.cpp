#include "GUIfuncs.h"


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


bool _DragFloat3LockAspect(const char* label, glm::vec3& value, bool& lock){
	// Saving values
	glm::vec3 copy = value;
	bool changed = false;

	if(ImGui::DragFloat3(label, glm::value_ptr(value), 0.02f, -100.0f, 100.0f)){
		changed = true;
		if(lock){
			if(copy.x != value.x){
				float change = copy.x == 0 ? 1 : value.x / copy.x;
				value.y *= change;
				value.z *= change;
			}
			else if(copy.y != value.y){
				float change = copy.y == 0 ? 1 : value.y / copy.y;
				value.x *= change;
				value.z *= change;
			}
			else if(copy.z != value.z){
				float change = copy.z == 0 ? 1 : value.z / copy.z;
				value.x *= change;
				value.y *= change;
			}
		}
	}

	ImGui::SameLine();
	ImGui::Checkbox(" ", &lock);
	return changed;
}