#include "GUI.h"

#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"
#include"stdafx.h"

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

void GUI::buildMenuBar(){
	if (ImGui::BeginMainMenuBar()) {
		// help
		bool openpopuptemp = false;
		if (ImGui::BeginMenu("Help")) {
			if (ImGui::MenuItem("About"))
				openpopuptemp = true;
			ImGui::EndMenu();
		}
		//windows
		if (ImGui::BeginMenu("Windows")) {
			ImGui::Checkbox("Edit Window", &show_edit_window);
			ImGui::Checkbox("Output Window", &show_output_window);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("New")) {
			if (ImGui::BeginMenu("Model")){
				if(ImGui::MenuItem("Load from file...", NULL)){
					Model* mesh = new Model();
					mesh->setModel(popupExplorer(".obj"));
					mesh->setTexture(popupExplorer(".jpg"));
					mesh->setRenderType(GL_TRIANGLES);
					scene->addModel(mesh);
				}
				if(ImGui::BeginMenu("Primitives ")){
					if(ImGui::MenuItem("Cube")){
						Model* mesh = new Primitive(PRIM_CUBE);
						scene->addModel(mesh);
					}
					if(ImGui::MenuItem("Tetrahedron")){
						Model* mesh = new Primitive(PRIM_TETRAHEDRON);
						scene->addModel(mesh);
					}
					if(ImGui::MenuItem("Grid")){
						Model* mesh = new Primitive(PRIM_GRID);
						scene->addModel(mesh);
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
				}
			if(ImGui::MenuItem("Camera")){
				// ...existing code...
			}
			ImGui::EndMenu();
		}
		//edit
		if (ImGui::BeginMenu("Render")) {
			ImGui::Checkbox("Render Grid", &scene->render_grid);
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
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

void GUI::buildEditWindow(){
	ImGui::Begin("Edit Window", &show_edit_window);
	//Model
	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
	if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags)){
		//TAB 1
		if (ImGui::BeginTabItem("Model")){
			// Forward backwards buttons
			if (ImGui::Button("<")){
				scene->cycleSelectedModel(-1);
			}
			ImGui::SameLine();
			if (ImGui::Button(">")){
				scene->cycleSelectedModel(1);
				
			}
			ImGui::SameLine();
			ImGui::Text("Selected Model: %d/%d", scene->selected_model+1, scene->models.size());
			ImGui::Separator();
			//Update position without friend class
			glm::vec3 position = scene->getSelectedModel()->getPosition();
			if(ImGui::DragFloat3("Position", glm::value_ptr(position), 0.02f, -100.0f, 100.0f)){
				scene->getSelectedModel()->setPosition(position);
			}
			glm::vec3 orientation = scene->getSelectedModel()->getAngles();
			if(ImGui::DragFloat3("Yaw Pitch Roll", glm::value_ptr(orientation), 0.01f, -glm::pi<float>(), glm::pi<float>())){
				scene->getSelectedModel()->setAngles(orientation);
			}
			glm::vec3 scale = scene->getSelectedModel()->getScale();
			if (_DragFloat3LockAspect("Scale", scale, lock_model_scale)){
				scene->getSelectedModel()->setScale(scale);
			}

			ImGui::Separator();
			if (ImGui::Button("Set mesh")){
				scene->getSelectedModel()->setModel(popupExplorer(".obj"));
			}
			ImGui::SameLine();
			if (ImGui::Button("Set texture")){
				scene->getSelectedModel()->setTexture(popupExplorer(".jpg"));
			}
			ImGui::Separator();

			int render_type = scene->getSelectedModel()->render_type;
			ImGui::RadioButton("TRIANGLES", &render_type, GL_TRIANGLES); ImGui::SameLine();
			ImGui::RadioButton("LINES", &render_type, GL_LINES); ImGui::SameLine();
			ImGui::RadioButton("LINE_STRIP", &render_type, GL_LINE_STRIP);
			scene->getSelectedModel()->render_type = render_type;
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Camera")){
			ImGui::DragFloatRange2("Camera FOV", &scene->getActiveCamera()->slow_speed, &scene->getActiveCamera()->fast_speed, 0.001f, 0.f, 100.0f);
			ImGui::Separator();
			ImGui::DragFloat3("Position", glm::value_ptr(scene->getActiveCamera()->Position), 0.02f, -100.0f, 100.0f);
			if(ImGui::DragFloat3("Orientation", glm::value_ptr(scene->getActiveCamera()->Orientation), 0.02f, -100.0f, 100.0f)){
				scene->getActiveCamera()->Orientation = glm::normalize(scene->getActiveCamera()->Orientation);
			}
			if(ImGui::DragFloat3("Up", glm::value_ptr(scene->getActiveCamera()->Up), 0.02f, -100.0f, 100.0f)){
				scene->getActiveCamera()->Up = glm::normalize(scene->getActiveCamera()->Up);
			}
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::End();
}

void GUI::buildOutputWindow(){
	ImGui::Begin("Output Window", &show_output_window);
	ImGui::Text("outputs go here");
	ImGui::End();
}

void GUI::build(){
    if(scene == nullptr){
		//No scene attached!
		return;
	}
	// Tell OpenGL a new frame is about to begin
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	buildMenuBar();

	if(show_edit_window){
		buildEditWindow();
	}

	if(show_output_window){
		buildOutputWindow();
	}

}

void GUI::render(){
	if(scene == nullptr){
		return;
	}
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::destroy(){
    ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

GUI::GUI(GLFWwindow* window, Scene* scene) : scene(scene)
{   
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void GUI::setScene(Scene* scene){
	this->scene = scene;
}