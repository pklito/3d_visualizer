#include"graphics/Renderer.h"

// How i split the depths, all objects are in the 0.2-1f range, overlays are 0.1-0.2, and highlighting the selected object is 0-0.1.
// Convention: _depthDefault is maintained.
void _depthDefault(){
	glDepthRange(0.2f,1.f);
}
void _depthOverlay(){
	glDepthRange(0.1f,0.2f);
}
void _depthClosest(){
	glDepthRange(0.0f,0.1f);
}

Renderer::Renderer(GLFWwindow* window, int width, int height) : tex_shader("resources/default.vert", "resources/default.frag"),
																no_tex_shader("resources/default.vert", "resources/default_no_tex.frag"), 
																color_shader("resources/default.vert", "resources/color.frag"),
																highlight_shader("resources/highlight.vert", "resources/highlight.frag"),
																background_color(0.07f, 0.13f, 0.17f, 1.0f)
, width(width), height(height), window(window){
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, width, height);

	// Enable backface culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	_depthDefault();
}

void Renderer::clearFrame()
{
	// Specify the color of the background
	glClearColor(background_color.r, background_color.g, background_color.b, background_color.a);
	// Clean the back buffer and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::updateCamera(Camera& camera){
	camera_view = aspect_matrix * camera.getProjectionViewMatrix();
}

void Renderer::windowResizeCallBack(GLFWwindow* window, int width, int height){
	if(window == this->window){
		this->width = width;
		this->height = height;
		glViewport(0, 0, width, height);
	}
	aspect_matrix[0][0] = (float)height / width;
}

void Renderer::_renderPipeline(bool do_blend, int layer, Shader& shader, GLuint render_mode, GLsizeiptr indices_count, const VAO* const vao, const Texture* const texture, const glm::vec4& color, const glm::mat4& model_transform, const glm::mat4& normal_transform){
	//Preventing code repetition

	//Pass model transforms
	shader.activate();
	shader.setMat4("modelTransform", model_transform);
	shader.setMat4("normalTransform", normal_transform);
	shader.setMat4("cameraTransform", camera_view);
	shader.setVec4("color", color);

	//Draw.
	vao->bind();
	if (texture != nullptr)	texture->bind();
	glDrawElements(render_mode, indices_count, GL_UNSIGNED_INT, 0);
	vao->unbind();
	if (texture != nullptr)	texture->unbind();
	shader.deactivate();

}

void Renderer::renderModel(GLuint render_mode, GLsizeiptr indices_count, const VAO* const vao, const Texture* const texture, const glm::vec4& color, const glm::mat4& model_transform, const glm::mat4& normal_transform){
	//Decide which shader to use
	bool use_texture = texture != nullptr && texture->exists();

	bool use_color = render_mode == GL_LINES || render_mode == GL_LINE_STRIP || render_mode == GL_LINE_LOOP;
	Shader& shader = use_color ? color_shader : (use_texture ? tex_shader : no_tex_shader);

	_renderPipeline(false, 0, shader, render_mode, indices_count, vao, texture, color, model_transform, normal_transform);
}

void Renderer::renderModelShadeless(GLuint render_mode, GLsizeiptr indices_count, const VAO* const vao, const Texture* const texture, const glm::vec4& color, const glm::mat4& model_transform, const glm::mat4& normal_transform){
	//Decide which shader to use
	bool use_texture = texture != nullptr && texture->exists();

	Shader& shader = color_shader;

	//Pass model transforms
	shader.activate();
	shader.setMat4("modelTransform", model_transform);
	shader.setMat4("normalTransform", normal_transform);
	shader.setMat4("cameraTransform", camera_view);
	shader.setVec4("color", color);

	//Draw.
	vao->bind();
	if (texture != nullptr)	texture->bind();
	glDrawElements(render_mode, indices_count, GL_UNSIGNED_INT, 0);
	vao->unbind();
	if (texture != nullptr)	texture->unbind();
	shader.deactivate();
}


void Renderer::renderHighlight(GLuint render_mode, GLsizeiptr indices_count, const VAO* const vao, const Texture* const texture, const glm::vec4& color, const glm::mat4& model_transform, const glm::mat4& normal_transform){
	
	Shader& shader = highlight_shader;
	//Pass model transforms
	shader.activate();
	shader.setMat4("modelTransform", model_transform);
	shader.setMat4("normalTransform", normal_transform);
	shader.setMat4("cameraTransform", camera_view);
	shader.setFloat("time", glfwGetTime());
	shader.setVec4("color", color);

	//Draw.
	vao->bind();
	_depthClosest();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
glEnable( GL_BLEND );
	glDrawElements(GL_TRIANGLES, indices_count, GL_UNSIGNED_INT, 0);
	_depthDefault();
glDisable( GL_BLEND );


	vao->unbind();
	shader.deactivate();
}

void Renderer::renderOverlay(GLuint render_mode, GLsizeiptr indices_count, const VAO* const vao, const Texture* const texture, const glm::vec4& color, const glm::mat4& model_transform, const glm::mat4& normal_transform){
	Shader& shader = color_shader;

	//Pass model transforms
	shader.activate();
	shader.setMat4("modelTransform", model_transform);
	shader.setMat4("normalTransform", normal_transform);
	shader.setMat4("cameraTransform", camera_view);
	shader.setVec4("color", color);

	//Draw.
	vao->bind();
	_depthOverlay();
	glDrawElements(render_mode, indices_count, GL_UNSIGNED_INT, 0);
	_depthDefault();
	shader.deactivate();
}