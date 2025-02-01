
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

//Expects ".obj", or ".png|.jpg", no spaces, split by vertical line.		
std::string popupExplorer(const std::string& file_type){
	CFileDialog dlg(TRUE, stringToLPCTSTR(file_type), NULL, NULL, _T("*.*"));
	if (dlg.DoModal() == IDOK)
	{
		CT2CA pszConvertedAnsiString (dlg.GetPathName());
		// construct a std::string using the LPCSTR input
		std::string strStd (pszConvertedAnsiString);
		return strStd;
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
			if(ImGui::MenuItem("Model", NULL)){
				Model* mesh = new Model();
				mesh->setModel(popupExplorer(".obj"));
				mesh->setTexture(popupExplorer(".jpg"));
				mesh->setRenderType(GL_TRIANGLES);
				scene->models.push_back(mesh);
			}
			if(ImGui::MenuItem("Camera")){

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

			if (ImGui::Button("Set mesh")){
				scene->getSelectedModel()->setModel(popupExplorer(".obj"));
			}
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Camera")){
			ImGui::DragFloat3("Position", glm::value_ptr(scene->getActiveCamera()->Position), 0.02f, -100.0f, 100.0f);
			ImGui::DragFloat3("Orientation", glm::value_ptr(scene->getActiveCamera()->Orientation), 0.02f, -100.0f, 100.0f);
			ImGui::DragFloat3("Up", glm::value_ptr(scene->getActiveCamera()->Up), 0.02f, -100.0f, 100.0f);

			ImGui::EndTabItem();
		}
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