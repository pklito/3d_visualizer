
#include "GUI.h"

#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"
#include"stdafx.h"

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
			ImGui::MenuItem("Edit Window", NULL, &show_edit_window);
			ImGui::MenuItem("Output Window", NULL, &show_output_window);
			ImGui::EndMenu();
		}
		//edit
		if (ImGui::BeginMenu("Render")) {
			// Open OBJ file to read
			
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void GUI::buildEditWindow(){
	ImGui::Begin("Edit Window", &show_edit_window);
	if (ImGui::MenuItem("Set mesh")){
		CFileDialog dlg(TRUE, _T(".obj"), NULL, NULL, _T("*.obj|*.*"));
		if (dlg.DoModal() == IDOK)
		{
			CT2CA pszConvertedAnsiString (dlg.GetPathName());
			// construct a std::string using the LPCSTR input
			std::string strStd (pszConvertedAnsiString);
			scene->getSelectedModel()->setModel(strStd);
			
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