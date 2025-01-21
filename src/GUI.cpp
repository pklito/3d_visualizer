
#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"
#include "GUI.h"

void createGUI(){
    
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

				if (ImGui::BeginPopupModal("About", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
					ImGui::Text("I'm a popup!");
					if (ImGui::Button("Close", ImVec2(60, 0)))
						ImGui::CloseCurrentPopup();
					ImGui::EndPopup();
				}

				ImGui::EndMainMenuBar();
		}

		ImGui::End();
}

void renderGUI(){
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void deleteGUI(){

    
    ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void initGUI(GLFWwindow* window)
{   
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}