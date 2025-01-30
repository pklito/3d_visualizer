
#include "GUI.h"

#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"
#include"stdafx.h"

void GUI::build(){
    if(scene == nullptr){
		//No scene attached!
		return;
	}
		// Tell OpenGL a new frame is about to begin
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

    // ImGUI window creation
		ImGui::Begin("My name is window, ImGUI window");
		// Text that appears in the window
		ImGui::Text("Hello there adventurer!");
		// Checkbox that appears in the window
		// ImGui::Checkbox("Draw Triangle", nullptr);
		// // Slider that appears in the window
		// ImGui::SliderFloat("Size", nullptr, 0.5f, 2.0f);
		// // Fancy color editor that appears in the window
		// ImGui::ColorEdit4("Color", nullptr);
		// Ends the window
		

		if (ImGui::BeginMainMenuBar()) {
				// help
				bool openpopuptemp = false;
				if (ImGui::BeginMenu("Help")) {
					if (ImGui::MenuItem("About"))
						openpopuptemp = true;
					ImGui::EndMenu();
				}
				if (openpopuptemp == true) {
					ImGui::OpenPopup("About");
					openpopuptemp = false;
				}

				//edit
				if (ImGui::BeginMenu("Edit")) {
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
					ImGui::EndMenu();
				}

				ImGui::EndMainMenuBar();
		}

		ImGui::End();
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