#include "graphics/GUI.h"

#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"
#include"stdafx.h"
#include "graphics/GUIfuncs.h"
#include "Logger.h"
#include "graphics/MeshEx.h"

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
			ImGui::Checkbox("Scene Window", &show_scene_window);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("New")) {
			if (ImGui::BeginMenu("Model")){
				if(ImGui::MenuItem("Load from file...", NULL)){
					std::string file = popupExplorer(".obj");
					if(file != ""){
						Model* mesh = new ObjModel(file);
						mesh->setTexture(popupExplorer(".jpg"));
						mesh->setRenderType(GL_TRIANGLES);
						scene->addModel(mesh);
					}
				}
				if(ImGui::BeginMenu("Primitives ")){
					//Iterate over all the defined primitives, add all the buttons
					for(auto pair : PRIM_MODEL_NAMES){
						std::string name = pair.second.c_str();
						name[0] = toupper(name[0]);
						if(ImGui::MenuItem(name.c_str())){
							Model* mesh = new Primitive((PRIM_MODEL)pair.first);
							scene->addModel(mesh);
						}
					}
					ImGui::EndMenu();
				}
				if(ImGui::BeginMenu("Config Models")){
					//Iterate over all the defined primitives, add all the buttons
					for(auto pair : getDemoConfigGroupModels()){
						std::string name = pair.first.c_str();
						name[0] = toupper(name[0]);
						if(ImGui::MenuItem(name.c_str())){
							Model* mesh = pair.second();
							scene->addModel(mesh);
						}
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
			ImGui::Checkbox("Highlight selected model", &scene->highlight_selected_model);
			if(renderer != nullptr){
				glm::vec4 color = renderer->getClearColor();
				color.a = 1.0f;
				ImGui::Text("Background Color"); ImGui::SameLine();
				if(ImGui::ColorEdit4("MyColor##3", glm::value_ptr(color), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel))
					renderer->setClearColor(color);
			}
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
			scene->buildModelEditGUI();
			ImGui::EndTabItem();
			
		}

		if (ImGui::BeginTabItem("Camera")){
			scene->getActiveCamera()->buildGUI();
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::End();
}

void GUI::buildSceneWindow(){
	ImGui::Begin("Scene window", &show_scene_window);
	scene->buildSceneGUI();
	ImGui::End();
}

void GUI::buildOutputWindow(){
	ImGui::Begin("Output Window", &show_output_window);
	Logger::getInstance().buildGUI();
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

	if(show_scene_window){
		buildSceneWindow();
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

GUI::GUI(GLFWwindow* window, Scene* scene, Renderer* renderer) : scene(scene), renderer(renderer)
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